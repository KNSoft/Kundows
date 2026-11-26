# CC-only 回收候选：保留、写回与退休必须成对

本轮按用户要求，不把 ReactOS 多 CPU 来宾能否稳定启动作为门槛；新增代码的共享状态、锁顺序和交错在代码层面审查，并以 1 vCPU 来宾中的事件屏障验证。**真实卷卸载已经在 Hyper-V 的独立 FAT32 测试盘上执行，不是尚待模拟的条件。**

原始增量候选为 [`cc-ownership-candidate.patch`](cc-ownership-candidate.patch)，仅改 `ntoskrnl/cc/pin.c`、`ntoskrnl/cc/view.c` 和内部 `cc.h`。下面的来宾记录来自该独立候选：FastFAT 保持不含 `fastfat-volume-dirty-runs.patch` 整卷兜底的原测试驱动，`fastfat-clean-after-flush.patch` 等无关修复不变。

这一轮默认队列改用 [`cc-bcb-ownership.patch`](../../../../Upstream/ReactOS/Patches/Runtime/cc-bcb-ownership.patch)，保留 `fastfat-clean-after-flush.patch`。被替代的三个补丁现作为 [FAT32 调查附件](../../FAT32-Volume-Flush/README.md#superseded-patch-files)留存。原增量候选只用于核对本次实测，不能叠加到新队列。

The 54/54 result below belongs to the earlier ReactOS candidate. The [consolidated investigation](../../FAT32-Volume-Flush/README.md) explains which assumptions the Windows tests overturned and which fix went into the validated BootCD.

## 原先的七个失败具体是什么

[Keep+Find 实验](../KernelTests/Results.md)让最后一个 pin / repin 释放后仍保留脏 BCB，避免 `FatFlushFat` 漏掉它。但每个 BCB 也持有一个 VACB 引用。之后发生下面的正常过程：

1. CC/MM 完成真实分页写，内容已经保存，VACB 已干净。
2. BCB 的旧 `Dirty` 标记仍为 1；实验代码没有对应的成功退休出口。
3. 调用方 pin / repin 都已经归零，BCB 却留在链表里，继续占有 VACB。
4. `CcPurgeCacheSection` 发现视图还有额外引用，返回 `FALSE`；截断也无法回收该视图。

这不是“仍有文件打开，所以不能卸载”的正常拒绝。诊断驱动已明确解除它自己的 pin / repin。七个失败分别在普通释放、repin 释放、写失败后恢复、持 pin 时先刷后改、写入中再次修改、lazy write、成功后截断之后出现；活动的干净 pin 另有对照，持有时拒绝 purge、释放后允许。

## 不能把 Dirty 简单清零

原有 `Dirty` 兼有“调用方已 premark、之后还可能修改”的含义：

```text
pin → mark dirty → flush A → 调用方写 B → unpin
```

如果 flush A 一成功就把唯一的脏位清掉，最后 unpin 不再重新发布脏位，B 又可能漏写。因此候选把几件事分开：

- `Pinned`：BCB 属于 pin 还是 map，不再以当前 `PinCount == 0` 猜类型。零 pin 的已有 pinned BCB 仍可重新命中。
- `Dirty`：调用方的 premark 意图；有活动 pin 时保留，最后 unpin 仍按旧修复重新发布。
- `DirtyPending`：缓存还拥有一条待写记录。最后调用方引用归零时，仅在选定元数据式 stream 上保留它。
- `DirtyGeneration`、`DirtyPublishers`：写回只可清理其快照之前、且已经完整发布到 MM/脏链表的状态。更新较新或仍在发布，就继续保留并排队重试。
- `BcbFlushActive`：该元数据视图正在写回。另一个 CC 刷写不能把瞬时的“视图不在脏表”解释为已完成，直接返回 `STATUS_DEVICE_BUSY (0x80000011)`，不在文件系统锁下等待。

成功写回后，旧 pending 状态可清除；没有调用方引用且没有新 pending 的 BCB，先在锁内摘链，再在锁外销毁资源、释放 VACB 引用。失败不清 pending，恢复后可以再次命中、重试并退休。不是以 `VACB.Dirty == FALSE` 作为写完的证明。

保留策略按公共文件头、空 `FsContext2`、pin 支持和 `DO_MODIFIED_WRITE` 位区分元数据式 stream，不硬编码盘号、FAT 页坐标或 stream 大小；普通缓存文件的生命周期不一律改成“留 BCB”。

## 一个额外反例：失败的 purge 不能先忘记脏页

第一版回收候选已在 Hyper-V 通过原 39 项，并使旧 FastFAT 上的独立 `fatflush.exe` 通过。但继续审查发现：

```text
同一个 VACB：
  页 A：脏 BCB，调用方引用已归零
  页 B：另一个仍被调用方 pin 住的页

purge 全视图 → 必须因 B 的活动 pin 返回 FALSE
```

如果为了腾出引用，先丢掉 A 的 idle BCB，再发现 B 导致 purge 失败，A 的数据虽仍在缓存里，其脏 BCB 却已经被忘记。后续 `PIN_IF_BCB(A)` 会未命中，原问题又被引入。

新增 case 9，在同一诊断驱动和 EXE 下比较：

| 私有 CC 内核 | 原 39 项 | 新增失败 purge 的 5 项 | 合计 |
| --- | ---: | ---: | ---: |
| 第一版回收候选 `27071F26…55412` | 39/39 | **4/5**，A 查找失败 | **43/44** |
| 不预先删除正常 purge 记录的修订 `D4C0140C…06110` | 39/39 | **5/5** | **44/44** |

逐 case 计数及失败检查号在 [`CcOwnership.tsv`](../KernelTests/CcOwnership.tsv)。这两轮使用相同的 V2 驱动和控制器，均为零执行错误；不是将第一版 39 项的旧通过记录算到新增条件上。

最终候选保持正常 purge 的原引用检查，不让它预先删除 pending BCB。只有**已停止调用方 pin 的 cache map 最终删除**才走独立 discard；底下的 VACB 最终写回职责不变。成功写回的正常回收已经在写回完成出口做完，不需要把普通 purge 当补救 GC。

## SMP 代码层面的审查

不宣称整个 ReactOS 的 SMP 已修好。下面是本补丁新增状态的同步依据：

| 情况 | 同步与所有权 |
| --- | --- |
| 查找与释放相撞 | 同一个 `BcbSpinLock` 内查找并加 caller 引用，或从链表摘除。获得引用的线程在等待 ERESOURCE 时仍拥有对象；摘链后的对象不能被新查找命中。 |
| 普通 unpin、repin 与回收相撞 | 引用、pin 计数、脏状态、发布者计数都由同一锁保护。unpin 读取 premark 意图也取锁，不依赖单核上的可见性。引用计数改为 `ULONG`，不沿用 16 位计数。 |
| 新资源的初始化/重复 BCB | 未发布的新 BCB 先在锁外获取其私有资源；重复查找获胜者加引用后，先放 list spin lock，再等待获胜者资源。没有持 spin lock 等待 ERESOURCE。 |
| MM 与脏链表发布中间被刷写 | 发布者计数跨越这两步；写回完成时不能将尚未发布完的 BCB 判干净。起止代次也更新，避免中间快照误覆盖。 |
| A 写回中出现 B 修改 | B 的代次比 A 快照新，A 完成不能清 B 的 pending。仍保留 premark 意图，最后 unpin 会发布实际完成的修改。 |
| 同视图两个 CC 写回 | 锁内获取无等待 reservation；碰撞者失败返回，首个 I/O 完成、重新排队与锁外退休都做完后才放 reservation。不会出现旧的“重新标脏”落到第二次成功之后。 |
| 自旋锁与全局缓存锁顺序 | 只在 BCB 锁内决定/摘链。MM、`CcRosMarkDirtyVacb`、`CcRosReleaseVacb`、资源销毁都在锁外；不建立 BCB 锁 → master/cache-map 锁的嵌套。 |
| 64 位代次在 x86 上撕裂 | 所有读写都在锁内；比较用模 2^64 差值，受控用例覆盖跨零回绕。假定一次 I/O 不跨越 2^63 次标脏。 |
| cache map 删除 | 先按原流程隔离 VACB，再放 master/cache-map 锁，丢弃无 caller 引用的 BCB，最后做原有 VACB 最终处理。活动 caller/flush 不可被强行退休；删除 map 前检查 BCB 链为空。 |

16 个宿主受控交错用例执行完整生产函数，覆盖发布中刷写、旧 I/O 完成后新脏数据、新 BCB 出生、锁/驻留失败、map 与 pin 分类、epoch 回绕、碰撞、活动引用及 teardown。x86 / x64 均为 **16/16**；等待、MM、VACB 释放的 stub 还检查当时未持有 BCB spin lock。既有整套回归两架构均通过。

在这些条件下，未发现本候选新增状态的无锁更新、等待前缺引用或新增锁反转。结论依赖正常的 CC 调用方契约：caller 持有有效 BCB 引用才使用它，文件系统在 cache map 最终删除之前停止活动 pin，MM 的成功刷新代表相应写入完成。没有以“多 CPU VM 启动成功”代替上述审查，也不声称穷尽所有并发交错。

**独立边界：** 旧 MM 从非 CC 路径进入的重叠写回仍是已有 `mm-flush-collision.patch` 调查项；本候选只为元数据 stream 的 CC 入口加 reservation，不声称修复整个 MM 的 page-out/shutdown 并发或 `MmDisableModifiedWriteOfSection` 桩。这个区别也必须保留在 SMP 结论里。

## 真实 FAT32 卷卸载

新增独立 [`fatcycle.exe`](../ApiTests/fatcycle.c)。只接受原 512 MiB、分区 1、1 MiB 起点、FAT32 双表、卷标与标记均匹配的测试盘；拒绝系统卷，物理磁盘句柄只读。没有测试驱动参与这一步。

每一轮：

1. 用正常缓存文件 I/O 写一个自己的 8 MiB 模式文件，刷文件和卷。
2. 文件仍打开时申请 `FSCTL_LOCK_VOLUME`，必须因打开文件被拒绝，而非 API 不支持。
3. 关闭文件，锁卷，执行真实 `FSCTL_DISMOUNT_VOLUME`。
4. 在旧卷句柄上查询 `FSCTL_IS_VOLUME_MOUNTED`，必须不再 mounted；关闭这个锁所有者。
5. 用路径重新打开卷/文件，核对卷标、序列号、设备身份、EOF，以及全部 8 MiB 每一个字节。
6. 下一轮换模式再做，连续三轮后删除自己创建的文件。

第一版、第二版及最终回收内核上均分别实测三轮成功：打开文件时锁卷返回 Win32 `5`；关闭后锁定、卸载成功；旧卷查询为未挂载、Win32 `21`；重新挂载后各轮 8,388,608 字节全部匹配，合计每版 **24 MiB 逐字节核对**。同次运行前，原来那个 7168 字节 `fatflush.exe` 的两份 FAT 各 **128/128**，刷卷前均为零。三版没有混用同一轮结果。

这证明实际卷路径没有因为本次回收改变而无法锁定、卸载或读回；不等于所有内核 pool 使用都经过长期压力审计。

## 最终候选与复核

最终私有内核将 unpin 的 dirty 读取也纳入锁，并扩宽引用计数。SHA-256：

`659D65C08958A0148F5ACBDBD8DD4B830171442E1D0092E9C4926C72701330F3`

最终候选已完成独立来宾复核，不仅是构建通过：

| 检查 | 最终候选结果 |
| --- | --- |
| V3 诊断工具，十个条件 | **54/54**，执行错误 0、退出 0 |
| 新增 case 10：lazy WRITE pending 时显式 flush | 在完成旧 IRP **之前**返回 `STATUS_DEVICE_BUSY`；旧写完成后重试成功，后端为 `0x3a`，可 purge |
| 原 `fatflush.exe`，原 FastFAT 无 `fastfat-volume-dirty-runs.patch` | FAT1、FAT2 各 **128/128**，退出 0 |
| 独立 `fatcycle.exe` | 三次真实锁定 / 卸载 / 重新挂载成功，**24 MiB** 逐字节一致，退出 0 |
| x86 / x64 所有权受控检查 | 各 **16/16** |
| 默认源码及最终候选 overlay 的既有宿主回归 | 两种源码、两架构均通过 `mm-flush-status.patch` 21/21、`cc-duplicate-bcb.patch` 12/12、`fastfat-clean-after-flush.patch` 10/10、`fastfat-volume-dirty-runs.patch` 16/16、`user-init-unlock.patch` 5/5、`monitor-relation-completion.patch` 8/8、`monitor-list-completion.patch` 14/14 |
| 默认补丁集应用 / 反向识别 / 回退 | 19、20、21、22 份四种组合全部通过；不含本候选 |

V3 的 case 3 实际出现一次分页写错误、恢复重试后成功；case 5 仍有两次受事件控制的写入；case 6、10 各观察到一次实际 lazy 回调。诊断后端的内容检查仍仅为模式页首字节，不借此宣称它做了整页逐字节核对；逐字节检查由独立真实卷工具完成。

本轮产物 SHA-256：

| 产物 | SHA-256 |
| --- | --- |
| 原 FastFAT，无 `fastfat-volume-dirty-runs.patch` | `C31CCEC07F365CCE759B6DC3550C689A0BFA6D4B8FD2B3E99481D822450713F5` |
| V3 `cccycle.sys` | `AF06DE9AB092DF9ED72FADE70511B97DE7949E7E2B8272493E9DDC71417F5E43` |
| V3 `cccycle.exe` | `556784FB59D358C104C834E45D5B5BE148F9DCC44F1511C8E546828E5A5EBFC1` |
| 原 `fatflush.exe` | `09C2AB157C56173FE0AD36DBA023A44D051DAC25A7A932FF7F29C459F407E986` |
| `fatcycle.exe` | `A23813874929A716C49B6D43F18CB8F5E38342335469FE1ECE3EABBB270A2C27` |

两轮最终证据系统盘均已**只读**复核内核、FastFAT、各自工具的哈希，以及盘内日志与控制器日志一致。独立数据盘标记匹配，两个自建临时文件均已删除；正常关机后的完整 FAT1 / FAT2 哈希相同：

`EE736BD65BA2538980687520E7E2904C153C065C4D673BBDA1E853F1FA2D3E16`

这项关机后核对不代替原 EXE 在刷卷返回后的立即读回。VM 最后恢复为 **Off、1 vCPU、1 GiB、单一基线盘、无 DVD / NIC**；没有在宿主安装诊断驱动，未改封存故障盘。

```text
python Docs/Notes/HyperV-Desktop-QA/Review/Checks/run.py Upstream/ReactOS/Source --overlay <私有候选根> --suite bcb-owner
python Docs/Notes/HyperV-Desktop-QA/KernelTests/summarize.py <控制器日志>
```

上述历史增量候选在原默认 CC 修复之上应用；它重写了若干旧补丁的目标函数，**不能不处理依赖和反向识别就直接塞进自动补丁队列**。候选验证时未撤默认 `fastfat-volume-dirty-runs.patch`；随后的整理改为合并这些 BCB 修复，而不是叠加原增量文件。

原始构建、响应文件、内核/PDB、来宾日志、保存内存和恢复回执分别在 `OutDir/CcOwnership-4fbb/`、`OutDir/CcOwnership2-4fbb/`、`OutDir/CcOwnership3-4fbb/`。最终原始输出为 `BeforeOwnership3-01.log`、`BeforeOwnership3Volume01.log`，只读核对为 `Verify-result.txt`；逐 case 汇总另保存到上述 TSV。保存的补丁 hunks 已与最终私有编译单元逐字节（规范化换行后）核对，并在隔离副本验证应用和回退。

## 按修复点整理

这里的依赖原来不只是“先应用哪一个”：旧两份 CC 补丁和候选都改同一批函数。即使按顺序能够应用，后一个重写了前一个的 hunks，也会使 `Configure.cmd` 无法通过单独反向检查识别前一个已经应用。

本轮整理为：

| 修复点 | 整理结果 |
| --- | --- |
| BCB 保留、查找、引用及回收 | 新 `cc-bcb-ownership.patch` 合并旧 `cc-pinned-dirty.patch`、`cc-duplicate-bcb.patch` 和已验证候选；字段、各出口和写回完成处理作为完整单元 |
| MM 写错误传播、CC 有界重试 | `mm-flush-status.patch` 保留，避免把不同责任揉成一个大补丁 |
| FAT 卷失败后不能标清洁 | 保留 `fastfat-clean-after-flush.patch` |
| FastFAT 整卷补刷 | [`fastfat-volume-dirty-runs.patch`](../../FAT32-Volume-Flush/Assets/fastfat-volume-dirty-runs.patch) 只在文档中归档 |
| 独立 MM 写碰撞 | `mm-flush-collision.patch` 仍仅由 `--image-fixes` 选择，未扩大本轮范围 |

新 ownership 补丁直接以固定上游为基准，不要求先叠加旧两份 CC 补丁。它与 `mm-flush-status.patch` 的 hunks 不相互覆盖；二者仍有行为上的配套关系：MM 必须正确报告写失败，CC 才能判断是否允许成功退休。

迁移当前源码前，已将四个相关文件与原补丁组合在独立索引中生成的内容核对一致，保存字节备份后，仅反向撤掉已识别的三份旧补丁，再应用新 ownership 补丁。迁移后的三个 CC 文件与此前实测候选逐字节（规范化换行后）一致；其他子模块修改未重置、未清理。

轻量检查：新默认 / ARM64、带 / 不带可选修复的 **17 / 18 / 19 / 20** 份组合均通过应用、逐份反向识别、完整回退；CC ownership / MM status 交换顺序也通过，结果树一致。x86 / x64 宿主检查均通过：`mm-flush-status.patch` 21/21、`cc-duplicate-bcb.patch` 12/12、`fastfat-clean-after-flush.patch` 10/10、所有权 16/16，以及新的生产 CC → 原 `FatFlushVolume` 联合检查 **18/18**；其他既有检查也通过。

旧 `fastfat-volume-dirty-runs.patch` 的 16 项保留在 `Checks/fat-volume-legacy.c`，只用于复核历史 workaround，不再冒充新 CC 路径的测试。当前 `--suite fat-volume` 在有 ownership 修复时执行 `cc-volume.inc` 的 18 项，调用真实生产 pin / unpin 与未修改的 FastFAT 扫描；MM/设备边界仍是宿主桩，不替代上面的真实来宾记录。

用户随后明确要求本轮先整理、不做复杂构建和验证。已停止构建链：在收到该指示前，Configure 和 Build Solution 已完成（1296 成功、0 失败、123 无需更新、14 跳过）；当时正在进行的 MakeISO 已终止，未继续运行虚拟机。发布位置的 ISO 仍是原 `FE13B2AC…BC09`，不把未完成的介质步骤报告为成功。用户其后要求仅提交本地 commit、不 push；不得把此前私有候选的来宾结果写成新默认介质已经实测。

本轮备份、迁移回执和轻量检查日志在 `OutDir/CcRuntime-4fbb/`；旧 ISO 另保存于其 `before/`。三个同树构建入口、MSVC / RosBE / CMake、默认架构配置及导出级别未改。

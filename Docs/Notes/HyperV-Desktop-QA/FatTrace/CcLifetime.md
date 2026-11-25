# C2 根因下钻：脏 BCB 在最后一个 repin 释放时被摘链

**本次漏刷已经定位到 ReactOS CC 的 BCB 生命周期，而不是移植的 FastFAT 扫描本身。** 观察版内核在同一个独立 [`fatflush.exe`](../ApiTests/README.md) 上记录到：普通 pin 释放后，目标仍有一个 repin 引用；`CcUnpinRepinnedBcb(FALSE)` 释放最后引用时，BCB 和缓存视图仍脏，但记录被摘链。随后 `FatFlushFat` 以 `PIN_IF_BCB` 查询同一整页，没有找到任何重叠 BCB，返回成功而没有刷这一页。

进一步只在目标测试页改变 CC 的保留和查找行为，**不换 FastFAT**，即可使两份 FAT 从 0/128 变为 128/128；只改其中一项仍失败。下面将负对照、因果干预与 Windows 静态证据分别列出。

这是对[整卷漏刷](FlushBoundary.md)的内部定位，不是一个新的通用 CC 修复。本页探针和因果干预不进入正式补丁队列；当时保留的 FastFAT 整卷兜底，已在后续补丁整理中改由完整 CC 方案替代，用户要求仅作本地提交、未发布新介质。

**后续进展：** [完整 CC 所有权候选](../Review/CcOwnership.md)已补齐成功写回后的退休，在原 FastFAT 上通过 54 项内核检查、原 EXE 两份 FAT 各 128/128，以及三次真实卷卸载 / 重挂、24 MiB 逐字节读回。随后已按修复点整理为工作区的 `cc-bcb-ownership`，不再叠加旧两份 CC 补丁；新默认介质尚未验证，不要把它与本页的 Keep+Find 因果干预混为同一补丁。

## 同一次来宾运行的证据

ReactOS 固定版本 `4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6`，原 Hyper-V Gen 1 / 1 vCPU / 1 GiB 夹具。与无探针的修前对照使用同一份桌面安装副本、同一份 512 MiB FAT32 空盘模板、同一个 7168 字节 EXE。保留不含 C2 的 `fastfat.sys`，只换一个独立链接的观察版内核。

- 实际安装名为 `ntoskrnl.exe` 的内核是 SMP 构建 `ntkrnlmp`；原 SHA-256 `CCAB9C71469DDD631BAA456E0DF1D7C686BBA157A38B6735AFB12ECE4886CF9F`，观察版 `CD5E7315CF09F8E9E40C0861DB7CAEB5B8D3BDC2B090111D1730A560AA8A44F0`。
- FastFAT 保持 `C31CCEC07F365CCE759B6DC3550C689A0BFA6D4B8FD2B3E99481D822450713F5`；EXE 保持 `09C2AB157C56173FE0AD36DBA023A44D051DAC25A7A932FF7F29C459F407E986`。
- `FlushFileBuffers(volume)` 成功；返回后的两次整盘读取各足额 512 字节，两份各为 **0/128 正确、128/128 为零**，EXE 退出 `1`。不是仅凭脏标志断言盘面。
- 模板保留区 `3,156,992` 字节，每份 FAT `518,656` 字节。目标项 69632 的卷内位置为 `3,435,520`，所在页为 **`3,432,448 + 4096`**。整盘读回位置仍为 `4,484,096 / 5,002,752`，与 8 GiB 安装盘实验的坐标不同。

[`CcLifetime.tsv`](CcLifetime.tsv) 是从暂停的来宾内存读出的全部 25 条事件，包含启动读入和测试文件清理。主要的分配、刷卷事件均在 `FATFLUSH.EXE` 的线程 884、同一个 cache map `M1`。BCB / map 标签由完整指针的相等关系生成，不是仅比较低地址；地址复用不保证对象跨释放后的身份。

| 顺序 | 目标 BCB B3 的操作 | RefCount | PinCount | BCB / 视图脏位 |
| ---: | --- | ---: | ---: | --- |
| 8 | 页固定成功，范围 `3432448 + 4096` | 1 | 1 | 0 / 1 |
| 9–10 | repin，标脏 | 2 | 1 | 1 / 1 |
| 11–13 | 普通 unpin 再发布脏位，释放 pin 和普通引用 | 1 | **0** | 1 / 1 |
| 14 | `CcUnpinRepinnedBcb` 入口，`WriteThrough=FALSE` | 1 | 0 | 1 / 1 |
| 15 | 最后引用减到零，即将 `RemoveEntryList` | **0** | 0 | **1 / 1** |
| 16 | `PIN_WAIT | PIN_IF_BCB` 查询相同整页 | — | — | 未命中；**重叠数 0，覆盖数 0** |

在这版 FastFAT 中只有 `FatFlushFat` 使用 `PIN_IF_BCB`；FAT32 分支的查询长度就是整页。顺序 16 之前的普通 pin 范围与它完全相同，排除了“已有 BCB 只覆盖子页”的解释。也不是只剩 repin 的 BCB 仍在表里但被 `PinCount > 0` 排除：顺序 15 已记录了摘链，顺序 16 在自旋锁内遍历整条 BCB 链，连重叠记录都没有。

观察器只把数据写入固定大小的内核内存表，不打印、不增加磁盘 I/O、不改变查找或回收条件。EXE 已完成返回后读回、退出并进入批处理的 `pause` 后，宿主保存 RAM，再继续来宾正常关机并还原空盘检查点。记录不是 Windows 的动态跟踪，也没有把保存时的盘面代替 API 返回时的读回。

## 2×2 因果实验：只换 CC，不换 FastFAT

在观察版之外再独立编译三个内核。仍从同一桌面基盘和空测试盘模板复制，四轮 EXE、FastFAT、数据盘参数及分配区间全部相同，均未启用 C2 整卷兜底。两处干预只对观察器选定的范围生效：

- **Keep：** `RefCount` 减到零且目标 BCB / VACB 仍标脏时，暂不摘链和释放；覆盖普通 unpin 与 unpin-repinned 两条出口。
- **Find：** pin 查找允许上述目标脏 BCB 的 `PinCount == 0`。

| 选定范围的 CC 行为 | 刷卷时重叠 / 覆盖 BCB 数 | pin 查询命中 | FAT1 / FAT2 正确项 | EXE 退出 |
| --- | --- | --- | --- | ---: |
| 仅观察，无干预 | 0 / 0 | 否 | 0/128、0/128 | 1 |
| 只 Find | 0 / 0 | 否，记录已经释放 | 0/128、0/128 | 1 |
| 只 Keep | **1 / 1** | 否，零 pin 仍被排除 | 0/128、0/128 | 1 |
| **Keep + Find** | **1 / 1** | **是** | **128/128、128/128** | **0** |

四轮 API 都返回成功，刷卷前的两份扇区都还是零；每次返回后的整盘读回均为 512 字节。结果、精确内核哈希和查找事件号在 [`CcLifetimeCausal.tsv`](CcLifetimeCausal.tsv)；`after_fat*` 是正确项数，`tested_entries=128`。

成功轮的[全部 31 条 CC 事件](CcLifetimeBoth.tsv)进一步对应了机制：事件 15 从链表中命中原 B3（Ref=0、Pin=0、两个脏位都为 1）；16 重新 pin；22 进入 `CcUnpinRepinnedBcb(TRUE)`；23 已完成缓存刷写、VACB 脏位为 0，才摘链释放。随后 EXE 实际读到正确的两份链。这不是背景恰好先写完：刷卷前仍是零，CC 记录又明确出现了此次 write-through 路径。

完整干预在 [`cc-lifetime-causal.patch`](cc-lifetime-causal.patch)，叠加在观察器之后。只保留它的查找 hunk 得到 Find 轮，只保留两条释放 hunk 得到 Keep 轮（均保留辅助判定函数）。这些 hunks 与实测的独立翻译单元一致。**不是修复候选：** 用整个 VACB 的瞬时脏位决定选定 BCB 的寿命，只适合本次无并发重写的因果实验；后台刷写后如何回收零引用 BCB、卷卸载等出口没有实现。它证明了两项 CC 行为共同导致这次漏刷，不证明这种简化策略可推广。

## Windows 静态对照：脏记录的寿命不同

用 x64 Windows SDK CDB **离线**打开本机模块，加载匹配的微软公开 PDB。模块文件版本均为 **10.0.26100.9974**，不等同于 API 测试中报告的 Windows 系统版本 `10.0.26300.9974`。

| 模块 | SHA-256 | PDB 缓存标识 |
| --- | --- | --- |
| `ntoskrnl.exe` | `F6F275F67C7EF44FC2DF2D828658FB31EAFFD41018AC5A0FD9440E08D9912C30` | `ntkrnlmp.pdb/CE884F70F5A9025D53DB35D8BE8BFF791` |
| `fastfat.sys` | `C046046AE5D4A5C5DEFC63237B07F8027DD059D83660D90AC431348C90676D39` | `fastfat.pdb/D16835C5AC209E557FD6E2A4A1D6BE4A1` |

以下偏移只对应这两个映像。`_BCB` 的 `Dirty=+0x002`、`BcbLinks=+0x010`、`PinCount=+0x040`、`SharedCacheMap=+0x0b0`、`BaseAddress=+0x0b8` 均由 PDB 类型核对。

1. **不是所有 stream 都用同一套脏跟踪。** `CcInitializeCacheMapInternal+0x6ba` 检查 `FileObject->FsContext` 所指公共头的 `Flags2 & FSRTL_FLAG2_DO_MODIFIED_WRITE`；未置位且 `FsContext2 == NULL` 时，`+0x88b` 调 `MmDisableModifiedWriteOfSection`，然后把 shared cache map 的 `Flags` 第 9 位设为 1。FastFAT 的虚拟卷文件使用零初始化的 VCB 公共头、空 CCB，经 `FatInitializeCacheMap` 调 CC。这里串起的是静态构造条件，不是一次来宾断点命中。
2. **普通 unpin 与 repin 的零计数都会进入脏位判断。** `CcRepinBcb+0x13` 在 BCB 锁内增加 Windows 的 `PinCount`；它不等于 ReactOS 只增加 `RefCount` 的实现。`CcUnpinData` / `CcUnpinDataForThread` 和 `CcUnpinRepinnedBcb(FALSE)` 都以 mode `0` 调 `CcUnpinFileDataEx`。后者 `+0xf1` 减计数，`+0x161` 检查是否归零，`+0x166` 再检查 Dirty。`+0x2a6` 只控制资源锁释放的模式，仍回到上述公共路径。
3. **零 pin、仍脏时不摘 BCB 链。** 这条分支可以在 `+0x179..+0x1af` 释放 VACB 映射并清 `BaseAddress`，随后返回。零 pin 且干净才进入 `+0x1f3` 的摘链路径，最终到 `+0x291` 的 `CcDeallocateBcb`。`CcSetDirtyPinnedData` 在 shared cache map 第 9 位的分支里设置 `BCB.Dirty`；另一分支用脏页 mask，不能把结论泛化为所有缓存文件。
4. **查找不要求原来还有一个 pin。** `CcPinFileData+0x241..+0x288` 按范围查找已有 BCB，不以 `PinCount > 0` 为前提；找到而 `BaseAddress==NULL` 时，`+0x2c7` 取回其范围，`+0x2f2` 增加计数并继续映射。`PIN_IF_BCB` 的 bit 3 在未找到记录的 `+0x42b` 才使请求失败。新建范围还有 `+0x591` 的页边界规范化。
5. **刷卷驱动仍使用相同的扫描办法。** Windows `FatFlushFat` 的 FAT32 路径以 `Length=0x1000`、`Flags=9` 调 `CcPinRead`，随后标脏、repin、普通 unpin、write-through unpin。无需假设 Windows 另外全表读取才能解释该扫描。

Windows write-through 的内部顺序还不同：`CcUnpinRepinnedBcb` 可先以 mode `2` 把 BCB 脏页交给 MM，再 `MmFlushSection`；失败后走重新标脏的分支。因此“看到某刻 Dirty 为零”等于“已落盘”也不是通用结论。

**证据边界：** 上述已确认映像中存在这些可达分支及其条件，未动态观察 Windows 在那次 `fatflush.exe` 上实际经过哪条分支。Windows 的无探针 EXE 结果证明外部行为；ReactOS 的保存内存证明这次内部过程，二者不互相替代。若需要 Windows 同次动态对应，下一步才需要接到 Windows 测试 VM 的 WinDbg；不需要接管宿主内核。

## 为什么不能只改一个判断

[`bcb.c`](../Review/Checks/bcb.c) 新增独立 `bcb-life` 观察模式，直接运行完整生产函数，不启用后台写。固定版和现有正式补丁树在 x64 的 13 个状态观察相同；正式树的 x86 也相同：

| 受控状态 | BCB 在链表内 | MM 仍脏 | `PIN_IF_BCB` |
| --- | --- | --- | --- |
| 持有普通 pin，整页覆盖 | 是 | 按输入 | 命中 |
| 普通 pin 已放，只留 repin | 是，Ref=1 / Pin=0 | 按输入 | **未命中** |
| 最后普通引用已放 | 否 | 脏输入时仍脏 | 未命中 |
| 最后 repin 已放，不 write-through | 否 | 脏输入时仍脏 | 未命中 |
| 最后 repin write-through 成功 | 否 | 否 | 未命中 |
| 最后 repin write-through 失败 | **否** | **是** | 未命中 |
| 持有整页 pin，只查询其中一扇区 | 是 | 是 | 命中 |
| 只持有子页 pin，查询整个页 | 是，但不覆盖 | 是 | 未命中 |

这些是实现现状的观察，**不计成“13 个兼容性用例通过”**，也不把错误的 miss 固化为未来 CC 必须遵守的断言。既有 C1 引用/锁回归仍为 x86、x64 各 12/12。

本轮新增观察模式后，既有整套宿主回归在 x86、x64 均通过，包括 M1 21/21、C1 12/12、F1 10/10、C2 整卷 16/16、G3 5/5、G1 8/8 和 G4 私有查询 14/14；默认产物与正式 ISO 未加入探针或因果干预。

正式修复至少要一起解决：

- 普通 pin 与 repin 的最后释放、零 pin BCB 的重新查找；只改查找不能复活已释放的对象，只保留记录又仍被 `PinCount` 排除。
- 每个 BCB 的脏区间与整个 256 KiB VACB 的写回代次。当前 `BCB.Dirty` 是“unpin 需要再发布”的粘性标记，`CcRosFlushVacb` 成功后并不逐个清理 BCB；简单 `if (Dirty) return` 会留下无人回收的对象和 VACB 引用。
- 后台写、显式 `CcFlushCache`、write-through 成功、失败和重新修改的所有权；VACB 被摘出脏链表可能只表示另一写回正在进行，不能以这个瞬时位判定安全释放。
- 文件大小变化、purge、卸载和 cache map 删除。现有删除路径遍历 VACB，不拥有一套“待写 BCB”的回收机制；增加缓存持有引用后要连同这些出口验证。
- modified-no-write stream 的分类。ReactOS `CcRosInitializeFileCache` 只有 `PinAccess` 等现有状态，没有上述 Windows 模式切换；`MmDisableModifiedWriteOfSection` 还是返回 `FALSE` 的桩，不能把所有支持 pin 的文件等同于此类元数据流。

因此这一阶段没有发布一份只动 `PinCount` 或最后释放条件的 Runtime 补丁，而是继续保留当时已验证的 FastFAT 整卷兜底。后续完整方案另记；单文件、其他文件系统和历史单份 FAT 分歧，不能由这一轮结果一并宣布修复。

### 后续正式方案：只修 CC，保留 FastFAT

用户指定的目标是 **CC-only**，不是把整卷补刷永久放在 FastFAT。2×2 实验已证明这条方向可行。正式版本需在不含 C2 整卷兜底的相同 FastFAT 上，通过 BCB 写回/失败重试/回收的受控回归和同一个 EXE 的来宾前后对照；之后才从默认队列撤下 `fastfat-volume-dirty-runs`。已有 F1 等无关修复不因这一目标而撤销。

本页第一阶段交付的是根因证据与定点干预，**不能直接当作可替代 C2 的通用 CC 生命周期补丁**。尤其不能将实验的 `BCB.Dirty && VACB.Dirty` 判定直接推广到后台并发写、purge 或卸载。后续候选及各出口的所有权、重试验证另见上面的进展链接，而不是只以单个正常刷卷 EXE 通过作结论。

随后已用[独立内核诊断驱动在 Hyper-V 验证](../KernelTests/Results.md)：相同的两个内核各执行 8 个条件 / 39 个检查。Keep+Find 补回了全部 5 个脏记录命中，但在显式/后台写回、失败后重试、受控两线程修改和截断之后，7 个回收检查全部失败；对照的这 7 个均通过。两轮均无测试执行错误。因此“还缺回收”已是来宾实测，不再只是代码审查的风险推断。

## 复核与原始材料

```text
python Docs/Notes/HyperV-Desktop-QA/Review/Checks/run.py Upstream/ReactOS/Source --suite bcb-life
python Docs/Notes/HyperV-Desktop-QA/Review/Checks/run.py Upstream/ReactOS/Source --stock --suite bcb-life
python Docs/Notes/HyperV-Desktop-QA/FatTrace/read-cc-lifetime.py <observed.vmrs> --provider <x64-vmsavedstatedumpprovider.dll>
```

前两条在对应架构的 MSVC 开发者环境运行；第三条在 x64 Windows 上只读访问保存的来宾内存。读取器核对活动表自指针、容量、事件完整性和目标范围；序号与 QPC 只用于该次来宾内的顺序，不与宿主时钟相减。

观察器源码在 [`cc-lifetime-probe.patch`](cc-lifetime-probe.patch)，只适用于本夹具和已有正式 CC 补丁；不依赖 C2 整卷补丁。实际实验从已有 MSBuild 编译/链接记录生成独立 `pin.obj` 和内核，未覆盖默认产物、未改源码树。修正后的观察单元以 `/WX` 编译。第一份观察器因未声明的取进程名函数导致指针截断、启动蓝屏；该失败介质没有执行 EXE，不作为 CC 故障证据。后来改为读取已声明的进程结构字段，重新编译并从全新安装副本运行，才得到本页数据。

`OutDir/CcContract-4fbb/` 保存 `observed.vmrs`、`events.tsv`、`Life-result.txt`、`BeforeLifeObs.log`、桌面/关机回执、独立内核/PDB、编译链接响应文件，以及 `windows-{pin,unpin,init,fat,...}.txt` 的原始 CDB 输出。`find/keep/both` 各有对应独立构建目录、`*-result.txt`、`*-observed.vmrs` 和 `*-events.tsv`。所有运行后均已正常关机并还原夹具；默认 ISO SHA-256 仍为 `FE13B2AC52AF5AE6BD121EF39032744F77FBB33894C3BFC423E4FAC51111BC09`。

离线核对命令使用 `.reload /f; lmvm ntoskrnl; dt ntoskrnl!_BCB; uf ntoskrnl!CcUnpinFileDataEx; uf ntoskrnl!CcUnpinRepinnedBcb; uf ntoskrnl!CcPinFileData; uf ntoskrnl!CcInitializeCacheMapInternal`，FastFAT 同理另开其映像。

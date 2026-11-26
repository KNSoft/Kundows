# 独立 SMP 审查：固定 Case1 idle BCB 增量

**结论：保留最后一次 dereference / 重映射的所有权设计；集成前修正截断路径。** 在 map 有效、调用者持有合法引用、按整视图写回的前提下，没有发现该增量导致 BCB 重复退休、两次命中泄漏 VACB，或最后放掉 VACB 后继续使用它。发现一项有新旧对照的行为回退，以及两处不能靠 BCB 自旋锁或文件独占来证明的既有 map 寿命问题。

## P1：截断点前的脏页阻止整个后缀失效（增量引入，宿主已确认）

`fs.c:328–340` 的 `CcSetFileSizes` **先发布缩小的尺寸**，再对 `[新 AllocationSize, EOF)` 调 purge，忽略布尔返回值。`pin.c:168–190` 的新增检查按 **BCB 所在整视图** 判重叠。于是即使脏范围全部在应保留的前缀内，只要截断点也在这个视图，`fs.c:218–221` 就提前返回；后面的独立干净视图不摘，`MmPurgeSegment` 也不调用。此例满足独占、无 caller pin、无活动 flush，不涉及违规调用。

私有 x64 对照：两个 VACB；第一个的第 0 页有 idle pending 记录，第二个干净、仅链表引用；allocation 从两个视图缩至 4096。取生产 `CcSetFileSizes`、`CcPurgeCacheSection`、新版 `CcpCanPurgeBcbRange`；MM 边界只记录请求，锁为单线程替身。旧版对第一个视图执行 `continue`（起点在截断点前，`fs.c:238–242`），保留它和前缀；旧版并不需要拒绝整个请求。

| 实跑 | 发布的 section size | 前缀 dirty | 干净尾视图残余引用 | MM purge 调用 |
|---|---:|---:|---:|---:|
| 79db84f 默认 | 4096 | 1 | 0 | 1 |
| + Case1 | 4096 | 1 | 1 | 0 |

确认的是**跳过失效且上层收不到失败**；没有测量后来扩展或重用时的磁盘内容。`CcUninitializeCacheMap(TruncateSize)` 也忽略 purge 结果（`fs.c:382–398`）。建议区分两件事：将被摘除的整视图是否仍负责 pending 记录；MM 按页失效的区间是否覆盖了 pending 数据。保留前缀，不能因同视图内的不相交脏页跳过后续全部视图；直接删掉 guard 则可能错丢脏数据。另补“截断落在脏范围内”及非页对齐边界的处理，明确哪些字节保留、哪些可按截断语义丢弃。在缩小已对外生效的路径上，失败必须有实际处置。

## 引用与线性化点

| 操作 | 固定源码位置及审查结果 |
|---|---|
| 最后一次 release 与 `PIN_IF_BCB` | `pin.c:236–252`：在 `BcbSpinLock` 内 `--RefCount`，若为零就把 `Bcb->Vacb` 移交局部变量并清空槽位；同一临界区决定保留 pending 记录或摘链。查找与 `++RefCount` 之间不放锁（401–410、258–269）。命中先发生则旧映射不能被拆；最后 release 先发生则命中者只能看到 idle 槽位或 miss。flush 先把 idle 记录摘链则后来的 `PIN_IF_BCB` 返回 FALSE，不创建记录。命中预留引用先发生则 flush 可清 pending，不能回收该记录。实际 pin 的资源获取和 `PinCount++` 在此之后（412–425）；预留引用不是已取得 pin。 |
| 两次命中 / 其中一个取图失败 | `pin.c:263–286`：两份临时 BCB 引用各算一次。锁外各取一份 VACB 引用，锁内只填空槽，败者在锁外释放多取的那一份。失败者撤销自己的 BCB 引用，不碰另一线程的临时 VACB；若它最后退出，则按当前槽位和 pending 状态统一清理。两次失败且期间已成功写回，最后撤销才释放干净记录。资源 no-wait 失败也走同一撤销路径；resident 失败由 finally 放 pin 或 map 引用（457–476、569–584）。 |
| miss 后发现重复 | `pin.c:304–347,349–373`：新记录插链或将输入引用移交给空的重复槽；若重复槽已有映射则在锁外退输入引用。资源获取失败退重复记录引用；局部 `Vacb=NULL` 区分引用已转移。分配失败由被调用者退输入引用；两个调用者已去掉第二次 release（450–455、553–560）。临时 BCB 未发布，其资源单独销毁。 |
| 分离后谁还能访问对象 | `Free=FALSE` 的最后 dereference 解锁后只放局部 VACB，**不再读 BCB**；另一线程此时可以退休记录。`Free=TRUE` 时 BCB 已独占摘链，放 VACB 后只销毁该 BCB 的锁/存储（39–46）。`CcRosReleaseVacb(FALSE,FALSE)` 最后的动作是减引用（`view.c:686–709`）。成功 pin/map、repin、发布者则均在有效 caller 引用覆盖下读槽位或 map（`pin.c:461–483,573–591,721–742,767–784,795–822`；`fs.c:CcGetFileObjectFromBcb`）。仅凭记住一个过去返回的 BCB 地址不算引用。 |

`BcbSpinLock` 保护的是记录链、引用/槽位、发布状态和 flush 计数，不保护 SCM 存储寿命。VACB 的发布/查找及脏链变更按 master → `CacheMapLock`，引用原子增减；首次创建也在锁内二次查重（`view.c:727–753,765–800,834–847,978–1044`）。链表基准引用、脏链引用和 BCB 活动引用彼此独立。BCB 锁外才调用 VACB 获取/释放、MM 或等待资源；不会为了重映射在该锁内反取 master。`Map->FileObject` 的对象引用方向也不是 FILE_OBJECT 反保 SCM（`view.c:1438–1469,351–356`）。

## 同一视图内其他范围会不会被错误退休？

`pin.c:93–135` 扫的是**同一 map 的链表**，比较 64 位起点对齐后的视图键，而非已清空的 `Bcb->Vacb`。在合法、可表示且限于单个 VACB 的 BCB 范围内（392–399、529–535），同视图内不同页被选中是有意的：即使显式请求很小，`view.c:203–206` 仍向 MM 提交整 VACB。失败（含部分写后错误）不清任何 pending；成功也须无进行中的 publisher 且记录 generation 不晚于 begin。新标脏先递增 generation/发布者，MM 和脏链更新后再推进 generation、递减发布者（`pin.c:727–742`）；较新范围留待再刷。清完 pending 仍须 `RefCount==0` 才摘链。未见仅由“两个记录共用视图”造成的错误退休。

这依赖 **成功状态确实代表这一整视图内应完成的写已完成**；不是逐 BCB 的写入回执。固定 MM 的 `section.c:5180–5195` 只查 dirty 位，独立在途写可已清该位（5246–5251）。不能由上述匹配规则证明 MM 在途写已经完成。若后续改为子范围 I/O，也必须同步改变完成范围判定。此项是接口前提，不是对后续候选的测量。

## purge、trim、teardown：代码保证的边界

- **普通 purge（FALSE）。** [公开调用条件](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccpurgecachesection)要求文件独占、任何线程均无映射或 pin；并非靠 `CcpCanPurgeBcbRange` 排斥新写者。该函数只在 BCB 锁内拍一次快照。已开始的写回在 clear pending 前通常使 guard 拒绝；对实际选中要摘的 VACB，`fs.c:228–270` 在 master/map 锁下按基准 + 可选脏链引用检查额外引用，显式 lookup 和 lazy 工作引用均计入。guard 成功不等于 purge 原子成功：部分视图会跳过，MM 仍有自己的写在途/分享检查（`section.c:4968–4983`）。不能据此授权持 pin purge。
- **P1，继承的 `UninitializeCacheMaps=TRUE` 寿命缺口。** 这是公开允许的分支。`fs.c:178,194–204` 保存 SCM，循环 uninitialize 私有 map；最后一次可同步走 `fs.c:398` → `view.c:1396–1402,351–356` 释放 SCM。下一次循环判空已没有存活保证，之后新增的 `fs.c:220` 同样依赖旧指针。即使独占、没有 pin 也可能是最后一个 private map。原循环先有问题，不应归因于 Case1，但不能据 `FALSE` 分支的结果签收 `TRUE`。建议遍历/执行前取得有定义的 SCM 寿命引用，或每次在受保护的入口重新取当前 map，处理最后 map 消失并转 MM；覆盖最后 private map，无需引入并发析构实验。
- **trim。** `view.c:553–625,874–897` 在 master/map 锁下只摘仅有基准引用的视图。idle dirty 即使已无 BCB 活动引用仍有脏链引用；写回摘脏链前已有工作/lookup 引用，失败或有较新发布时先重挂再退，trim 不能在这两个阶段抢走视图。但 **P2，既有 DBG 迟放对 SCM 的依赖**：trim 已从所有公共链摘下干净视图后，私有 FreeList 可跨锁甚至跨一次 flush，再在 646–656 放最后引用。此时另一线程正常关闭最后 private map；析构看不到该视图。`view.c:79–83,1179–1183` 的 DBG trace 仍读取 `Vacb->SharedCacheMap->Trace`；原子 VACB 引用不能保护这个字段。`CcRosFreeOneUnusedVacb` 的锁外释放也有类似窗口。建议保存释放所需的不可变诊断状态，或使延迟释放拥有 SCM 寿命；不要恢复 idle BCB 的长驻映射来掩盖它。此为源码路径判断，未做故障实验。
- **最终 teardown。** 懒写在 master 锁下同时增加 VACB 引用和 `OpenCount`，先结束操作再退 SCM（`view.c:413,440–457,463–477`），正常 last-close 不能越过它。显式 `CcFlushCache` 取 SCM 后只获取其内嵌 `FlushCacheLock` 和 VACB 引用（`view.c:1219–1290`），与最终删除之间没有对应的 SCM rundown。最终删除先摘视图/取消发布 SCM，再 `CcpDeleteBcbs`、断言 BCB 链空、最终释放 map（`view.c:269–305,314–356`）。`ActiveBcbFlushes` 在 begin/complete 成对加减；delete 对非零计数只**跳过**记录，既不等待也不阻止释放 SCM，空链时甚至不能从链空断言推得计数为零。因此 last-close 必须晚于合法 pin/repin、重映射临时引用及显式 flush 整个调用退出；对可能没有私有 map 引用的显式调用者，应由上层串行化或独立的 map 引用保证。不能把这一前提写成增量已经实现的并发析构保证。

## 固定输入与复核

私有目录 `OutDir/case1-smp-review-phssrmf5/`。从 ReactOS `4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6` 用 `git show <pin>:<path>` 写出 CC、`internal/cc.h` 和 `mm/section.c`；在空的私有 Git 目录依次 `git apply` 外层 `79db84f092e0ebc1e5aa93029e921bc2cd75b671` 的 `Runtime/ReactOS-{mm-flush-status,cc-bcb-ownership}.patch` 及交付的 `cc-idle-mapping-delta.patch`。行号均指该重建件。

| 输入/产物 | SHA-256 |
|---|---|
| 独立 delta 原字节 | `990f1948b30e9d4e904d16ed2a30174f4b9d4d8e73925ad8192fa1d9d297baa2` |
| `cc/pin.c` | `ee0ede6cb581d6eba4232bb0597b367f8c5e8d0a7b6fd36c489be4f3406617b2` |
| `cc/fs.c` | `3e9b15ae6217fd7aa2c5f6860fc7273a9b4ee82c546c3d9a90046ae1617a0e4b` |
| `cc/view.c` | `c37e08de8c3040a7472ce85248b00f429d725a435a348dbad14d82df3178d85f` |
| `internal/cc.h` | `a3d2caa1c38143dd5d273664074a53fe649974deda0f8fe641e7c9fc9870888f` |
| `mm/section.c` | `cc608d17f651aa41a5b054d8d7ec7ecc6b839f63bc6359be2a32b629c31c64ab` |

截断对照在该目录的 `shrink/`，两套提取体 `old.inc` / `delta.inc` 与 `check.c` 留存。旧队列两份补丁未改变旧 `fs.c` 的这两个函数。复现（x64 MSVC 开发者提示符，工作目录为 `shrink`）：

```cmd
cl /nologo /TC /DOLD /Feold.exe check.c && old.exe
cl /nologo /TC /Fedelta.exe check.c && delta.exe
```

原始输出 `shrink/result.txt`。这是控制流 A/B；未模拟 MM 页表或测量磁盘。不重复此前写回契约测试。交付给写入者的优先顺序：先处理有 A/B 证据的截断回退；将 `TRUE` purge 的 SCM 寿命单列修复；保留重映射的锁内预留和锁外释放，并把整视图完成及 map 调用期寿命作为后续接口的明确前提。

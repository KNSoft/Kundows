# 文件截断：保留有效前缀，清理缓存尾部

[原 P1](CcIdleMapping-SmpReview-phssrmf5.md) 是默认 ownership 补丁引入的回归：文件缩短后，仅因同一视图前部还有待写数据，后面的干净视图和 MM 页便没有失效，调用方也没有收到错误。已有一份针对交接树 `be9f26ff` 的[修复增量](Evidence/CcTruncate/cc-truncate.delta.patch)，已合并进[默认补丁](../../../../Upstream/ReactOS/Patches/Runtime/cc-bcb-ownership.patch)。

**后续 [Windows 内核实测及已集成实现](../../FAT32-Volume-Flush/README.md)取代本页关于错误和关闭通知的结论。** 本页成绩属于第一份修复；[初次 IDA MCP / WinDbg 对照](CcTruncate-Windows.md)另存。

## 改动和适用范围

- 对 `CcSetFileSizes` 的 **allocation 缩小**及 `CcUninitializeCacheMap(TruncateSize)` 单独处理。先检查受影响视图的引用、BCB 使用者和元数据写回状态，再调整所有权。普通 `CcPurgeCacheSection` 仍可拒绝清理待写数据。
- 截断前的 BCB 保留。跨边界的空闲 BCB 缩短到有效字节，保留相应待写页位；完全落在已截断后缀的空闲 BCB 摘链，在锁外销毁。新取得的锁按 master → cache map → BCB 顺序，锁内不做分页 I/O。
- MM 逐个访问有页表的区间，清理完整的后缀页。对于非页对齐的末页，保留有效前缀和原脏状态，只清零其后字节。若末页尚被映射，只接受能核对为所持缓存视图的唯一映射；其他仍被使用的页保守返回失败。
- 视图忙、页仍在读写或 MM 只清了部分尾页时，不把这次 resize 报成成功。缓存图记录待清的最低边界；同尺寸可重试，在清理完成前拒绝重新增长。关闭路径仍执行释放、通知等待者，返回值同时反映清理及释放结果。`CcGetFileSizes` 按缓存图分别返回分配长度、文件长度和有效数据长度。

这份修复以**文件系统已决定截断，并串行化尺寸变更**为前提。未改变仅减小 EOF、分配不变时由文件系统安排后续清理的策略。MM 可以返回部分清理失败；这里没有提供“回滚文件系统已释放空间”的事务。仍被引用的后缀页也不会被强制摘除。关闭时文件系统若忽略 `FALSE`，不能凭事件已触发推断清理成功。

## 实测

| 检查 | 结果 |
|---|---|
| [截断宿主用例](Checks/truncate.c) | [x64](Evidence/CcTruncate/host-x64.txt)、[x86](Evidence/CcTruncate/host-x86.txt) 各 **16/16**。执行生产 CC/MM 函数；文件对象、内存页、锁和关闭回调为宿主替身。检查了保留前缀、跨页记录、同页字节边界、稀疏尾部、活动引用、失败后同尺寸重试、扩展及关闭通知。 |
| 原有回归 | 每架构：写回 21/21，BCB ownership 7/7，BCB 12/12，CC/FastFAT 18/18，显式 pin/flush 及 repin 重试各 1/1。 |
| Windows 普通文件对照 | [本机输出](Evidence/CcTruncate/windows-file-api.json)：Windows build 26300，三个临时文件分别截至 0、4096、4219 字节，关闭重开后逐字节核对前缀，再扩回 166400 字节。三次前缀均一致，新增区在本次环境均读为零。该观察不能确定 Windows 使用同一内部算法。接口依据：[CcSetFileSizes](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccsetfilesizes)、[CcUninitializeCacheMap](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccuninitializecachemap)。 |
| [补丁队列](Evidence/CcTruncate/queue.txt) | 17 / 18 / 19 / 20 项均可应用、逐项反向识别并完整回退。`mm-flush-status.patch` 的函数声明 hunk 增加一行函数名上下文，未改其产码；两种 cache 补丁顺序已核对得到相同树。 |
| 活动树内核 | x64 Debug、x86 Release 的 UP/MP 四份均链接；[x64 链接身份](Evidence/CcTruncate/active-links.json)。x86：UP `8a3a4b44e394e1f8df6dd22c2fe9e436de4aa2f6d57b8898d82a4ede83ea8633`，MP `34732888225481168d8cb839ad136aaa7309440072a0eb3d2b21f05104605c04`。 |
| 活动树 x64 Debug 全 Solution | `Build.cmd x64 Debug` 退出 0；165 成功、0 失败、1254 最新、14 按配置跳过（含 BootCD）。 |
| 独立 x64 Debug 构建与介质 | 在干净目录从 17 项默认队列重建，1419 项成功、0 失败、0 最新、14 跳过；随后 MakeISO 成功。[既有产物核验](Evidence/CcTruncate/clean-media.json)确认三个盘内文件与该次链接输出相同。第一次核验因未识别 MakeISO 输出的带引号路径失败，修正核对器后对同一份原始日志、ISO 和副本重验通过。该 ISO 属于**复核前**的实现。 |

宿主回归可在相应 MSVC 开发者提示符从仓库根重跑：

```bat
python -I -B Docs\Notes\HyperV-Desktop-QA\Review\Checks\run.py Upstream\ReactOS\Source --suite truncate
python -I -B Docs\Notes\HyperV-Desktop-QA\Review\Checks\patch-sets.py
```

**源码身份。** 上游 pin 为 `4fbbefd614b`。活动树继承之前已应用的两项 ARM64 适配，等于 19 项队列重建树 `722a5747f044087e4e8997bf24e796d4738a17a7`。纯 x64 默认 17 项重建树为 `212a93e8678db6904a261e4fdb39cf9451bff573`；两者的 CC、此次涉及的 MM 源码/头文件和 FastFAT 相同。树号都不是新提交。ownership 补丁 SHA-256 为 `3ce5ae39b709aadde37642382d54a7ca2369723e88fdc139935bbe4595b1d52a`，相对 `be9f26ff` 的增量为 `b18430e45c8dd8d7eecfbdcff47f59b8b6e07a2cc062311376184ae6c9a67fd7`。

工作日志目录：`OutDir/CcTruncate-be9f26ff-01/`。真实来宾 FAT 回归及最终发布仍按[介质验收入口](Checks/final-media/README.md)单独记录。

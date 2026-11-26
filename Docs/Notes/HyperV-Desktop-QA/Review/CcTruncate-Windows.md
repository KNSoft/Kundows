# 截断修复的 Windows 接口复核

**后续集成：** [当前实现和 Windows 内核 DDI 实测](CcSizeCompatibility.md)；[较早候选、Windows NTFS/FAT32 的普通文件回归](CcSizeDefault.md)。下文保留第一轮草稿的身份。

**`722a5747` 的截断宿主回归通过，但公开接口与参考 Windows 有可见差异，不作为最终候选。** [修订增量](Evidence/CcTruncate/public-ddi-review.delta.patch)单独保存；它不在现有 BootCD 中。

## 实际核对了什么

IDA MCP 核对 `ntoskrnl.exe` **10.0.26100.9974**。WinDbg 通过项目已有连接读取专用参考机中已加载的 **10.0.26100.9457** 内核；目标状态为 `LiveKernel / Stopped / AMD64`。两者不是同一映像。来宾 `lmvm nt` 的 PDB 身份 `C29EBFB06B78B3C020DCA66D99713F9E1` 与原始来宾记录一致。另按导出表检查了保存的两份映像；[身份和入口比对](Evidence/CcTruncate/resize-abi-by-build.json)、[原始回执索引](Evidence/CcTruncate/windows-reference.json)可复核。

| 可观察行为 | Windows 证据 | `722a5747` 与修订 |
|---|---|---|
| `CcSetFileSizes` 的失败如何传给调用者 | 两版的公开入口都只调用 `CcSetFileSizesEx` 后返回，没有测试 `NTSTATUS` 并再次抛异常。`.9974` 的 Ex 实现中，部分清理失败返回 `STATUS_PURGE_FAILED`；扩展失败有抛异常出口。不能把两者合成“所有失败都抛出”。 | 已构建候选未导出 Ex，把尾部清理失败变成 `STATUS_UNABLE_TO_DELETE_SECTION` 异常。修订增加按 Vista+ 配置导出的 Ex；清理失败返回状态，扩展失败保留异常，旧入口仅转交。 |
| 截断以什么长度为准 | `.9974` 在共享缓存分支将输入 `FileSize` 用作范围边界，而 `AllocationSize` 参与扩展判断；没有共享缓存但有数据段时，也存在刷新/清理路径。来宾 live WinDbg 确认 Ex 的相应调用存在；调用列表不能证明这些分支在来宾被执行过。 | 旧候选只在 `AllocationSize` 缩小时进入截断。修订处理 EOF 缩小而分配不变，并补上无共享缓存、有数据段分支。 |
| 关闭完成事件 | `.9974` 的 `CcUninitializeCacheMap` 有立即通知和登记完成事件的不同分支。归档的 Case1 WinDbg 快照也见过尚未通知的事件；Case1 向关闭入口传的是 `TruncateSize=NULL`。 | 旧候选在返回前一律置事件；修订允许事件留在共享缓存图上，随最终清理通知。仍需核对共享缓存多使用者的返回值和通知顺序。 |

本轮通过 WinDbg 的原生正式回复读取了 `CcSetFileSizes` 入口、`CcSetFileSizesEx` 调用列表及 `nt` 模块身份。没有使用新客户端抢占连接。原有 29 件 Windows 封存材料字节数和哈希均匹配。Case1 测量的是正常脏数据写回与普通 purge；Case4 测量显式写回等待活动 pin。两者都不是“文件缩短后清理失败、随后恢复”的运行记录。读取已加载代码也不是一次文件操作的执行记录。

## 第一轮候选及当时的验收缺项

[修订身份](Evidence/CcTruncate/ddi-identities.json)：增量相对活动树 `722a5747`，同样可应用于纯 x64 队列树 `212a93e8`。后者套用后的树为 `4b3340bf`。改动局限于 ReactOS 的尺寸接口、关闭通知和对应导出。`TruncatePending` 是本地的失败恢复策略，**没有证据证明 Windows 采用同一策略**。

独立目录 `OutDir/Qddi` 的实际工作树已复算为 `4b3340bf`。x64 Debug、x86 Release 的 UP/MP 四个内核均已编译、链接，并经 `dumpbin /exports` 核实各自都导出 `CcSetFileSizes` 和 `CcSetFileSizesEx`。[四份产物身份](Evidence/CcTruncate/ddi-build.json)。没有用第一版的宿主行为成绩或 ISO 签收这一增量。

需要补充的 Windows 运行观察：分配量不变而 EOF 缩小时调用方看到的结果；尾部清理未能当次完成时 Ex 的返回及后续一次合法尺寸更新；还有缓存使用者时，关闭返回与完成事件谁先发生。验收应比较调用方可见的长度、数据和通知，不以私有结构布局相同为目标。上述条件未核实前，ReactOS 来宾验收暂停，先前的 16/16 不记到修订版上。

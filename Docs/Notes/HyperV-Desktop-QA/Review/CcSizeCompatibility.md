# 文件尺寸与关闭事件：Windows 对照

默认 [CC 补丁](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-cc-bcb-ownership.patch)基于 ReactOS `4fbbefd614b`。纯 x64 默认队列的源码树为 `bd620b9b5a8c386e1bc981726bed3943425e4b74`。以下结果按调用方能看到的返回值、事件状态、文件长度和数据核对。

## Windows 内核实际返回了什么

用 IDA MCP 核对 `ntoskrnl.exe` **10.0.26100.9974**，用 WinDbg 核对 Hyper-V 中已加载的 **10.0.26100.9457**（[两版入口](Evidence/CcTruncate/resize-abi-by-build.json)、[调试回执索引](Evidence/CcTruncate/Native-9457/reference.json)）。在后者上又用[内核对照程序](Checks/size-ddi.c)调用公开接口。它只操作自己提供的零填充文件；尺寸请求期间映射仍在使用的 4096 字节前缀不会被截掉。直接读取 DDI 返回值，关闭事件在调用返回后立即读取。[Windows 记录](Evidence/CcTruncate/Native-9457/windows-ddi.json)。另一次执行中，WinDbg 在该测试驱动的记录函数观察到 [34 次有序采样](Evidence/CcTruncate/Native-9457/windbg-trace.json)，28 个不同字段和驱动回执完全一致。

| 条件 | Windows 实测 |
|---|---|
| 有两个 private cache map，关闭其中一个并传入未置位的事件 | 返回 `FALSE`；返回时该事件已置位，另一文件对象仍有 private map。 |
| 关闭最后一个干净的 private map，也传入事件 | 返回 `TRUE`；返回时事件已置位。 |
| 仅有数据段，保持 EOF 不变、降低 VDL | 成功。读过跨 EOF 的最后一个有效页，再做同一操作，也成功。 |
| 文件映射着前 4096 字节，向 `CcSetFileSizesEx` 提交 8192 的拟议 EOF | 无 shared cache map 和有 shared cache map 两路均返回 `0xc0000435`（`STATUS_PURGE_FAILED`）。有共享图时，返回后的缓存 EOF 已是 8192。 |
| 解除映射并关闭 section，直接通知增长 | 两路均返回 `STATUS_SUCCESS`；有共享图时最终缓存 EOF 为 73605。 |

在独立 Windows 参考副本中，[NTFS](Evidence/CcTruncate/Default-9457/guest-ntfs.json) 和 [FAT32](Evidence/CcTruncate/Default-9457/guest-fat-file-size-contract.json) 的普通文件测试也一致：EOF `6144 → 4219` 而分配量仍为 `8192`；持有映射时文件系统拒绝缩小并保持 EOF；解除映射后可直接增长到 `14336`，另一句柄和重开后的前缀读回正确。这里文件系统返回 `0xc0000243`（`STATUS_USER_MAPPED_FILE`）；上表 `0xc0000435` 来自直接调用 CC 的另一项测量。

## 修正了哪些可观察的行为

- `CcSetFileSizes` 只转交给 Ex。遇到 CC 视图暂不能解映射时，尺寸通知仍可成功；真正的 MM 清理失败才由 Ex 返回错误。扩展失败走异常出口，非预期的底层错误归一为 `STATUS_UNEXPECTED_MM_EXTEND_ERR`。
- 以 EOF 决定要清理的尾部，以分配长度决定是否扩展段。有数据段但没有共享缓存图时，先刷非对齐的边界页，再清理后续整页：仅仅映射了有效末页不妨碍 VDL 更新；有外部段引用时也不能仅凭后缀没有驻留页就宣布能缩短。
- 关闭时先摘下本文件对象的 private map。还有其他 private map 时，通知当前调用方；只剩内部工作时，保留完成事件，直到真正清理或映射被重新使用。干净且带事件的最后关闭可以同步完成；需要写回的公开关闭将最后一个引用交给 cache worker。完成事件和布尔返回值都不是磁盘 I/O 的状态码。

## 保留的实现差异及影响

- ReactOS 自己持有待写的 BCB 页记录，MM 也可能只完成一部分尾部清理。因此记住未完成后缀，重试时跳过文件系统已经声明有效的新数据。pin-access 流还会清掉已失效的**空闲**元数据记录。这可能增加一次清理，但既不能销毁活动的 pin，也不能让旧失败否决本次合法增长。该记录不属于 Windows ABI。
- 最终释放沿用 ReactOS 的 cache worker、VACB 脏页记账和内存分配器。若排队所需的工作项分配失败，转为同步完成，调用方可能在关闭中等得更久。测试区分“返回时是否完成”和“最终是否交付”，不承诺两种实现的调度延时相同。

宿主用[实际 CC/MM 函数](Checks/truncate.c)测试两类清理拒绝、部分页、稀疏尾部、仍活动的写入、保留有效字节、关闭立即通知、延后通知和重开取消等待等。x64 和 x86 的尺寸/通知各 **34/34**，原写回、BCB 和 FAT 检查均通过。[宿主与队列记录](Evidence/CcTruncate/Native-9457/checks.json)。正式来宾的比较方法是离线核对安装的 MP 内核、FastFAT 和程序身份，然后从硬盘冷启动，对照同一程序的返回值和输出。

## 最终 Hyper-V 验收

独立目录从默认 17 项补丁和固定上游源码干净构建 x64 Debug：**1419 成功、0 失败**。以 `native-final/test.iso` 在 6 GiB 空系统盘上全新安装，正常关机后核对盘上的 MP 内核与 `fastfat.sys`。移除 DVD 和软盘，接入全新的 512 MiB FAT32 夹具副本，冷启动运行与 Windows 相同的两份 DDI 驱动和三个文件测试程序。

| 项目 | 安装后 ReactOS 对 Windows |
|---|---|
| 两次关闭、两种缓存状态下的尺寸更新 | 28 个字段逐项一致，含两次 `STATUS_PURGE_FAILED`、返回后可见的 EOF、解除映射后直接增长及事件状态。 |
| 非页对齐 EOF 的独立对照 | 旧候选错误地拒绝仅更新 VDL；修订后和 Windows 一样成功。 |
| 普通文件尺寸、刷卷和重挂 | 三个程序退出码均为 0，标准输出逐行一致；刷卷返回时两份 FAT 各 128/128，三轮卸载重挂共核对 24 MiB。 |

[安装组件身份、关机后取回的字段与文件输出](Evidence/CcTruncate/Native-9457/reactos-guest.json)。可运行 `python Docs/Notes/HyperV-Desktop-QA/Review/Checks/compare-size-runs.py` 重做比较。x86 的覆盖为宿主用例与 UP/MP 内核链接；真实来宾覆盖为 x64。

验收通过的同一份 BootCD 已更新到 `OutDir/bootcd-x64-Debug.iso`，SHA-256：`2bffbc71fc69e84ccd1fc0c38e0150c44f2aa4709848de3bf6fb26f839508bc8`。

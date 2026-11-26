# 默认尺寸更新与关闭通知：Windows 参考及集成

本页记录 `2eef1040`。**当前实现及内核实测见[下一轮对照](CcSizeCompatibility.md)**；下文的关闭事件结论只代表旧候选。

`CcSetFileSizesEx`、旧入口转交、基于 EOF 的截断和关闭完成通知现已合并进[默认补丁](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-cc-bcb-ownership.patch)。不再需要单独叠加上一轮的 DDI 草稿。[源码树及增量](Evidence/CcTruncate/Default-9457/source.json)；[相对 `722a5747` 的补丁](Evidence/CcTruncate/Default-9457/default-ddi.delta.patch)。

## 可见行为与证据

在独立 Windows 参考副本中，实测内核 `10.0.26100.9457`，分别对 [NTFS](Evidence/CcTruncate/Default-9457/guest-ntfs.json) 和 [FAT32](Evidence/CcTruncate/Default-9457/guest-fat-file-size-contract.json) 执行了同一份普通文件程序，结果一致。参考 FAT 盘为 `KWFATQA` 模板副本。

| 操作 | Windows 实测 |
|---|---|
| 两个共享句柄打开同一文件，将 EOF 从 6144 缩到 4219 | 调用成功，分配量仍为 8192；第二个句柄立即看到新 EOF，前 4219 字节不变。 |
| 保留只读映射时再次请求缩短 | 文件系统调用返回 `0xc0000243`（`STATUS_USER_MAPPED_FILE`）；第二个句柄看到的 EOF 仍为 4219。 |
| 解除映射后直接扩展到 14336 | 成功，无需先重复被拒的尺寸；读回的有效前缀正确，新增区在此次运行均为零。 |
| 关闭第一个句柄，继续使用第二个，再全部关闭、重开 | 第二个句柄仍可用；重开后的 EOF 和有效前缀均匹配。 |

[Windows FAT 刷卷](Evidence/CcTruncate/Default-9457/guest-fat-fatflush.json) 在 API 返回时两份 FAT 各 128/128；[三轮卸载、重挂](Evidence/CcTruncate/Default-9457/guest-fat-fatcycle.json)逐字节核对合计 24 MiB。程序走的是真实文件系统入口。上表的映射拒绝不是 `CcSetFileSizesEx` 返回 `STATUS_PURGE_FAILED` 的测量，关闭一个文件句柄也不等于触发特定缓存完成事件。

## 实现选择

- **错误出口和长度。** 扩展段失败走异常；Ex 清理失败返回状态；旧 void 入口不把返回值再次转换成异常。分配增长与逻辑 EOF 缩小分开判断。以 [IDA 及来宾 WinDbg 的导出核对](CcTruncate-Windows.md)为依据，实现和导出表均已改变。
- **失败后的新请求。** 删除了旧草案中“以前失败过就拒绝任何 EOF 增长”的条件。ReactOS MM 可以在处理了若干页后发现后续页仍在使用，所以在缓存图上保留尚需清理的位置；收到新的增长通知时，把清理起点限制到文件系统报告的新 VDL 之外，避免重放旧清理而丢掉刚刚初始化的数据。只在本次实际清理仍未完成时返回失败。这是对本地 MM 能力的补偿，不把这个内部字段当作 Windows 的实现事实。
- **关闭通知。** 非最后一个使用者登记调用方事件；最终清理摘下整条通知链，在刷写与缓存图释放后逐一通知。最后关闭是否同步完成和 I/O 的 `NTSTATUS` 是两个问题，公开布尔值不再是二者的逻辑与。内部写回路径仍有原错误状态。正常关闭另一句柄及卷卸载有 Windows 用户态实测；有多个已登记事件时的顺序在宿主中执行了实际生产关闭/析构函数。

## 修订版回归

[x64](Evidence/CcTruncate/Default-9457/host-x64.txt) 和 [x86](Evidence/CcTruncate/Default-9457/host-x64_x86.txt) 每架构：截断和通知 25/25、写回 21/21、BCB 所有权 7/7、BCB 12/12、CC/FastFAT 18/18，另两项同步契约各 1/1。测试覆盖失败后直接增长、已初始化区保护、旧入口异常语义、无缓存图分支以及三个真实关闭调用的事件交付；底层 I/O 和释放边界为内存夹具。

[补丁队列](Evidence/CcTruncate/Default-9457/queue.log)四种配置均能应用和回退；[四份内核](Evidence/CcTruncate/Default-9457/kernels.json)均已链接。独立 x64 Debug 的 17 项默认队列树为 `2eef1040dba89e2e5899023abf5fec664a2f92d9`，干净构建 1419 成功、0 失败；[测试介质回执](Evidence/CcTruncate/Default-9457/clean-media.json)核对了 ISO 和盘内的两个内核、FastFAT。ISO SHA-256：`d854b07ea83ce5aea3bc49e02eb8625d174971924c22c36002997c6151789579`。

## 同一组程序的 Windows / ReactOS 来宾对照

从上述 BootCD 全新安装，移除 DVD 和软盘后冷启动。安装到 `system32/ntoskrnl.exe` 的是该 ISO 的 MP 内核，已按内容而非文件名核对；`fastfat.sys` 也与本次链接输出一致。验收使用独立的第二块 FAT32 盘。三份 EXE 均与 Windows 参考机实跑的原件 SHA-256 相同。

| 验收项 | Windows FAT32 / ReactOS FAT32 |
|---|---|
| 缩短、持映射拒绝、解除映射后增长、关闭与重开 | 两侧退出码均为 0；每一行长度、状态和数据校验结果相同。 |
| 刷卷返回时立即读取两份 FAT | 两侧均由 0/128 变为各 128/128，退出码 0。 |
| 锁卷、卸载、重挂和回读 | 两侧均完成三轮，合计 24 MiB 字节一致，退出码 0。 |

[来宾原始结果及可执行文件身份](Evidence/CcTruncate/Default-9457/reactos-fat-guest.json)。第一次执行得到上述输出，但脚本把退出码重定向成了空文件。正式回执来自**另一份全新夹具**上的第二次执行：关机后只读取回三个独立退出码文件，均为 `0`，并逐行比对 Windows FAT32 的标准输出。两个 Windows 文件系统都观测到的 `STATUS_USER_MAPPED_FILE` 是文件系统的返回值；未将它写成 CC 内部清理失败的动态证据。

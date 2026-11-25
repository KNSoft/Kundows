# 从用户态验证内核和驱动修复

[`fatflush.c`](fatflush.c) 测的是 FAT32 **卷句柄**的 `FlushFileBuffers`。程序不知道自己在哪个系统上运行，只用同一条判据：刷卷成功后，刚分配的连续簇链在两份 FAT 上是否都已可读。

## 同一 EXE 的实际结果

| x64 系统 | 刷卷 API | 返回后 FAT1 正确项 | 返回后 FAT2 正确项 | EXE 退出码 |
| --- | --- | ---: | ---: | ---: |
| Windows `10.0.26300.9974` | 成功 | 128/128 | 128/128 | 0 |
| ReactOS `4fbbefd614b`，不含 C2 整卷补丁 | 成功 | **0/128** | **0/128** | **1** |
| 同一 ReactOS 安装，含 C2 整卷补丁 | 成功 | 128/128 | 128/128 | 0 |

三次使用同一份空测试盘的副本、同一个 7168 字节 EXE（SHA-256 `09C2AB157C56173FE0AD36DBA023A44D051DAC25A7A932FF7F29C459F407E986`）。三个系统刷卷前读到的两份目标扇区均为全零。读取位置都是整盘字节偏移 `4484096` 和 `5002752`，每次均足额读到 512 字节；[18 次读回结果](FatFlush.tsv)列出正确项数和零项数。Windows 分配的是簇 10–73737，两个 ReactOS 都是 7–73734，目标 128 项完整落在三者的连续分配范围内。

两个 Hyper-V 测试均从同一份已正常关机的安装盘复制，冷启动时没有安装光盘。两边保留相同的其他修复，安装的系统组件只换了 `fastfat.sys`；这是对单个补丁的反向对照，不是从头未打任何补丁的系统。修前驱动 `C31CCEC07F365CCE759B6DC3550C689A0BFA6D4B8FD2B3E99481D822450713F5`，修后 `46452AF4151E66CAFABF5DB9E652FD8FD5B60906E02A8ED3C0D8A76ACBBEA4E8`。均从无探针的正式介质取出。关机后以只读方式再次核对 EXE、驱动哈希、两份日志以及临时分配文件已删除。原始日志在 `OutDir/ApiFlush-4fbb`：`Windows-result.txt`、`BeforeC.log`、`AfterC.log`。

## 运行 C2 测试

1. 从 x64 MSVC 开发者命令行运行本目录的 `build.cmd`，产物在 `OutDir\ApiTests\fatflush.exe`。使用同一个 x64 EXE 测试 x64 Windows 和 x64 ReactOS。没有测试驱动，不要求调试版内核或符号。
2. 在启用 Hyper-V 的 Windows 上，用提升权限的 PowerShell 运行 `.\New-FatFixture.ps1 -Path E:\QA\empty.vhdx`（目录须已存在，文件须不存在）。该脚本创建新的 512 MiB VHDX、一个从 1 MiB 开始的 MBR 分区、双表 FAT32、4 KiB 簇、卷标 `KWFATQA` 和 `KWC2.ID` 标记，之后卸载。每次测试使用这份空盘的一个独立副本。
3. Windows 挂载一个副本；ReactOS 关机时把另一个副本接为第二块 IDE 磁盘。以有权刷卷和只读打开物理磁盘的账户运行 `fatflush.exe X:`，`X:` 是**测试卷**的实际盘符。保存标准输出和退出码。

测试先确认目标不是 Windows/ReactOS 系统盘，核对卷标、标记、磁盘/分区大小和 FAT 参数。打开卷和只读、无中间缓冲的整盘句柄，确认目标 FAT 扇区的 128 项原本都空闲，然后在文件系统上新建 `KWC2.TMP` 并设 EOF 为 288 MiB。用 `FSCTL_GET_RETRIEVAL_POINTERS` 确认 FAT 项 69632–69760 在文件的同一连续分配区间内，所以期望值直接是 69632→69633，依次到 69759→69760，不需要内核探针提供答案。

保持文件和卷句柄打开，在刷卷前后直接读两份 FAT 各 512 字节；比较各项的低 28 位。没有整盘写入，程序结束时删除自己创建的文件，不使用关机或延迟读回来代替返回时的检查。退出码：`0` 通过，`1` 刷卷成功但盘面内容不符，`2` API 或测试执行错误，`77` 不符合夹具条件。若后台在刷卷前就写完了，`before` 也会显示 128/128；该次通过不能独立证明原驱动没有缺陷。

## 哪些修复能用这样的 EXE 区分？

| 修复 | 用户态可以观察什么 | 有区分力的测试条件 |
| --- | --- | --- |
| C2：FAT 整卷漏刷 | 卷刷写的返回值，以及两份 FAT 的实际内容 | 本目录的独立 EXE。读缓存文件本身或只比较两份 FAT 是否相同都不够，两份可能同样过期。 |
| M1：MM/CC 写回错误；F1：FAT 卷清洁标记 | 卷刷写的最终状态、失败后的卷脏位，以及恢复后重试是否保存数据 | 需要测试存储在**已经产生脏页之后**返回可控的写错误。预先将卷设为只读通常只测到写入口失败；正常盘刷写成功无法区分补丁。F1 的关机分支还要从卷外观察。 |
| C1：重复 BCB 的引用和锁；CC 最后一次 unpin 重新标脏；可选 MM 重叠写回 | 最终数据可从用户态核对，普通 API 无法指定内部 pin、引用或两个分页写的完成次序 | 用真实生产函数的受控时序回归，或移植为 `kmtests`。ReactOS 自己的 [`CcPinRead_drv.c`](../../../../Upstream/ReactOS/Source/modules/rostests/kmtests/ntos_cc/CcPinRead_drv.c) 也是在驱动里调 `CcPinRead`、`CcSetDirtyPinnedData`、`CcUnpinData`。 |
| G1/G4：显示设备查询；同步文件信息请求 | 图形启动、显示器枚举或普通文件操作是否可用 | 这些结果不能证明指定请求走了“已内联完成，但 IOSB / 输出尚未交付”的分支。G1/G4 的直接证据见[同盘更换 win32k 的对照](../Review/MonitorList.md)和完成例程回归。 |
| G3：GDI 初始化失败时释放 USER 锁 | 普通应用启动时，CSRSS 通常已经完成了这里的初始化 | 需要控制初始化失败分支并检查锁；成功启动一个窗口不验证失败出口。 |
| RTC 校准、串口调试器发现、引导加载器、安装阶段显示 | 来宾能否越过相应启动阶段 | 关键行为发生在普通 EXE 启动之前，适合从 VM 外部对照。 |

还有一个容易误判的入口：在此固定版本，`FlushViewOfFile` 调 `NtFlushVirtualMemory`；底层 [`MmFlushVirtualMemory`](../../../../Upstream/ReactOS/Source/ntoskrnl/mm/ARM3/virtual.c) 是返回成功的桩。单独断言这个 Win32 API 返回 `TRUE`，不能证明 M1 的 `MmFlushSegment` 错误传播。已有的[生产函数回归](../Review/Verified.md)和跨系统 Win32 行为测试回答的是不同问题。

F1 还有一个参照边界：这是 ReactOS 在文件系统已报错时保留卷脏状态的策略。[本机 Windows 符号对照](../Review/Verified.md#本机-windows-对照)并未证明 Windows 关机路径作相同的状态判断。即使有可控的失败磁盘，也应先记录 Windows 的实际结果，而不是把某个内部脏位预设成 Windows 的契约。

## 真实卷卸载 / 重新挂载

[`fatcycle.c`](fatcycle.c) 使用同一标记的 512 MiB FAT32 测试夹具；拒绝系统盘，物理磁盘只读。在 x64 MSVC 环境运行 `build-cycle.cmd`，产物 `OutDir/ApiTests/fatcycle.exe`，在一次性来宾里执行 `fatcycle.exe X:`。

每轮写自己的 8 MiB 模式文件：打开文件时锁卷应被拒绝；关闭后锁卷、`FSCTL_DISMOUNT_VOLUME`，确认旧卷句柄不再 mounted，再由路径重新挂载并逐字节核对全部内容、EOF、卷与设备身份。重复三轮；只删除自己创建的文件。不是把关闭文件句柄或关机当成卸载。

CC-only 候选上已完成的[实测和锁审查](../Review/CcOwnership.md)单独记录。它与本页原 `fatflush.exe` 测量互补：后者核对成功刷卷**返回后立即**的两份 FAT；卸载后读回不能替代那条时间边界。

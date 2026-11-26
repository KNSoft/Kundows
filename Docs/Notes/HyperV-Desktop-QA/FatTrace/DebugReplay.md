# 默认 Debug 灰屏时，fastfat 在报告哪个文件？

封存故障盘用原 `win32k.sys`，以及换入[通过普通入口复测的 `monitor-relation-completion.patch`/`monitor-list-completion.patch` 版](../Review/MonitorList.md)，从默认 Debug 入口启动都停在同一处。灰屏时的保存状态给出了文件名：**`\pagefile.sys`**。在不同的副本上逐项处理后，fastfat 又依次报告 **`SYSTEM.ALT`** 和 **`PALAI.TTF`**。三条坏链均由同一条代码路径检出。处理完三项的副本用默认 Debug 入口通过了[图形安装向导](../Images/first-disk-debug-recovered-gui.png)，另一份相同处理的副本完成安装、关机并[无介质冷启动到桌面](../Images/first-disk-debug-cold-desktop.png)。各次均未连接串口调试器。

## 从 CPU 取样到错误参数

1. CPU 当时可能正在 `kdcom` 轮询串口，也可能在它调用的 HAL 延时程序内。在保存栈上识别原版 `KdSendPacket` 等待 ACK 的帧，核对该帧中的包类型、实际消息长度和校验和。报告的是 `DbgKdExceptionStateChange`，第一次机会异常 `STATUS_ASSERTION_FAILURE`（`0xC0000420`）。
2. 包中的 PC 与异常地址一致，且和该虚拟 CPU 的 `KPRCB.ProcessorState.ContextFrame.Rip` 同为 `ntkrnlmp+0x7AF48`，即 `IoRaiseInformationalHardError+0xF8` 的 `int 2Ch`。原版的[这条 Debug 断言](../../../../Upstream/ReactOS/Source/ntoskrnl/io/iomgr/error.c#L710)专门检查 `STATUS_FILE_CORRUPT_ERROR`。
3. 从**断言发生时的 RSP** 读取被调函数保存的状态 `0xC0000102` 及 `UNICODE_STRING` 实参。核对这一帧的返回地址和实际导入调用目标，再向上一帧核对 `fastfat!FatPopUpFileCorrupt+0xC0`、`FatLookupFileAllocation+0x4A5`。[对应分支](../../../../Upstream/ReactOS/Source/drivers/filesystems/fastfat/allocsup.c#L829)在遇到空闲、保留或坏簇时报告链断裂。文件名由此取得，不靠从 RAM 中搜索字符串。

在这个版本，[`KdpReport`](../../../../Upstream/ReactOS/Source/ntoskrnl/kd64/kdtrap.c#L63)会把断言交给调试器；串口传输的[超时分支](../../../../Upstream/ReactOS/Source/drivers/base/kdcom/kddll.c#L393)只允许特定消息提前返回，异常状态变更包会继续等待确认。这解释了保存时 CPU 在串口或 HAL，而触发点却在文件系统。保留了这个报告行为。

触发点 `ntoskrnl/io/iomgr/error.c`、fastfat 的 `allocsup.c` 和 `fatdata.c` 在固定提交 `4fbbefd614b` 与此前核对的[上游提交 `a3772c9df2a1`](https://github.com/reactos/reactos/tree/a3772c9df2a1a6f486cf957721956b18ebb19776)之间 Git blob 相同。运行证据来自本库的 Hyper-V 镜像。

| 同一封存盘的不同副本，均从默认 Debug 启动 | 实测结果 |
| --- | --- |
| 不改文件和目录项，原安装二进制 | 灰屏，断言实参 `\pagefile.sys`。按持续灰屏取样；较早的一次定时取样还在 FreeLdr 装载阶段，不计作断言现场。 |
| 更换 `win32k.sys`，保留两份 FAT 和三个坏文件的原目录项 | 灰屏，断言实参 `\pagefile.sys`。 |
| 同一 `win32k.sys`，另外恢复字体已知的 29 个 FAT 项 | 灰屏，仍为 `\pagefile.sys`。 |
| 同一 `win32k.sys`，保留原两份 FAT 和损坏字体，仅把 `PAGEFILE.SYS` 目录项的首簇、文件长度归零 | 灰屏，断言实参变为 `\ReactOS\system32\Config\SYSTEM.ALT`。 |
| 在上述分页文件对照的基础上，核对主 `SYSTEM` 后把损坏的 `SYSTEM.ALT` 短名改为未占用的 `QAOLD.BAK`，不改 FAT | 灰屏，断言实参变为 `\ReactOS\Fonts\palai.ttf`。 |
| 同一组目录项对照，再按安装 CAB 核对并补回字体链的 29 项 | 默认 Debug 进入图形安装欢迎页。 |

保存状态 SHA-256（按上表前五行顺序）：`658A15C20F695FD9ACE3F564DD7D9432F73048DCC462D1CB1FF5BCECA9A2737F`、`068620639C076B2D0A913A88ADF2775E30656E1046E15657F31624B575B1EF0E`、`32ADE55F6F90A1DF74B04AA87C6BB88CBE7FBC0BBFD8B57B81565F54CE85AA7B`、`0785DC490B7FB55BF0897E6D94EF61F41F81ED746928524AC3377FA529F5CD7D`、`BEF98397BC49E92E324F2F50FB8B40E5A013AB0C4FB2139D119CCC7DB5DEB2AC`。封存盘已经历过故障和重启；这个实验定位的是从它**重放**的停止点。最初那次写入缺少当时的完成记录。

**安装到桌面的验证。** 在独立副本上重复末行的处理，保留原安装介质和内容哈希已核对的主 `SYSTEM`。从默认入口通过 10 个图形安装页面，进入桌面；卸载 DVD、软盘，核对 Ctrl+Alt+Del 窗口后正常关机。只从硬盘冷启动，再次到桌面、响应 Ctrl+Alt+Del 并正常关机。此时只读实测两份 FAT 全表哈希相等（`6e2728c21f9bfb4d554bb026ba941c3d4cfa679caf69ce9f5b1fb550938c05f1`），字体链为 68 / 68 簇；在用的 `SYSTEM.ALT` 为 417 / 417 簇，关机盘面上没有 `PAGEFILE.SYS`。改名留存的损坏备用文件没有据此恢复内容，目录操作仅为该副本的启动对照。宿主首次立即核验还原状态未通过；独立复核确认空盘检查点已恢复、VM 关机且无 DVD/软盘，随后释放了测试平面盘。

## 两个启动时文件的盘面对照

在同一原 ISO 的正常安装中分别保存首次硬盘菜单、图形安装欢迎页，再与封存故障盘作只读对照。以下链长以 **FAT1 / FAT2** 表示：

| 文件 | 硬盘菜单 | 正常欢迎页 | 封存故障盘 |
| --- | --- | --- | --- |
| `PAGEFILE.SYS` | 不存在 | 首簇 2031618；目录要求 393024 簇，链长 393025 / 393025，正常链尾 | 同一首簇、同一目录大小 1609826304 字节，链长 **1 / 1**，下一项都是空闲 |
| `SYSTEM.ALT` | 不存在 | 首簇 397383；目录要求 129 簇，链长 129 / 129，正常链尾 | 同一首簇，目录大小 520192 字节、要求 127 簇，链长 **1 / 1**，下一项都是空闲 |

这与字体损坏的盘面形态不同：字体在封存盘上是 **1 / 68**。该盘的 FAT2 虽与正常文本安装结束时的整份表逐字节相同，其中也没有这两个后来生成文件的有效链。不能通过选用 FAT2 来恢复这两个文件。

## 如何核对

- 在 Windows x64 上对保存状态运行 `python kd-file-error.py <guest.vmrs> --provider <SDK-x64-vmsavedstatedumpprovider.dll>`。[读取器](../Review/Checks/kd-file-error.py)使用本次原版二进制的函数指纹和 x64 帧布局；原始普通入口在另一断点上的保存状态不会被识别成这条断言。
- [`read-mirrors.py`](read-mirrors.py) 在只读挂载的副本上输出字体和上述两个文件的目录长度、两份链长与链尾。保留主 `SYSTEM` hive、原始镜像及封存盘；本页中的目录项对照只在离线测试副本上进行。

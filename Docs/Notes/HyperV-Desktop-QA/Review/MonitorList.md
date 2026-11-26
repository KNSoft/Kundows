# `monitor-list-completion.patch`：不修 FAT 的故障盘副本已从普通入口进入图形向导

环境：ReactOS `4fbbefd614b`，Hyper-V Gen 1，1 vCPU / 1 GiB / 8 GiB IDE，x64 Debug、`0x600`。[原现场](Verified.md#原盘副本两次显示器查询先后失败)已经在同一条 `IOCTL_VIDEO_ENUM_MONITOR_PDO` IRP 上证实：视频端口有一个列表指针、报告 8 字节；`win32k` 使用的输出槽和字节数仍是 Debug 填充值。

**[默认补丁](../../../../Upstream/ReactOS/Patches/Runtime/monitor-list-completion.patch)** 把这个内核内部、固定为一个指针大小的 `METHOD_BUFFERED` 查询收回到发起者管理。`EngpQueryMonitorList` 自行分配 IRP 和非分页系统缓冲区，复用 `monitor-relation-completion.patch` 的完成通知；完成例程停止展开，pending 时用不可警醒的内核等待。回到发起线程后，使用最终 NTSTATUS，核对返回尺寸和非空指针，把已引用的列表交给原有消费者，再释放请求和临时输出槽。这个查询没有 MDL。成功的列表及其中的设备引用按原方式释放。

修复没有把这个协议推广到任意 `EngDeviceIoControl`：其他 IOCTL 的缓冲长度、传输方式、错误时的字节数及清理契约并不相同。原来的通用函数作为[未改动的对照](Checks/ioctl.c)，仍可观察到它对延迟复制的依赖。

| 同一故障盘的独立副本 | 结果 |
| --- | --- |
| 含 `monitor-relation-completion.patch`、不含 `monitor-list-completion.patch`（上一轮重复两次） | 普通入口断言于 `EngpUpdateMonitorDevices` 的 `bytesWritten` 检查。 |
| 含 `monitor-relation-completion.patch`、`monitor-list-completion.patch`；普通入口 | [到达图形安装欢迎页](../Images/first-disk-after-g4-gui.png)。两份 FAT 逐字节与故障原样相同，仍差 29 项；损坏字体的活动链仍只有 1 簇。 |
| 同一改动，另一个副本；默认 Debug，无宿主调试器 | 125 秒内仍灰屏。另一次从灰屏的 [KD 消息和 fastfat 调用参数](../FatTrace/DebugReplay.md)定位到 `\pagefile.sys` 的簇链断裂。 |

对照只替换 `win32k.sys` 及该目录项的有效长度，复用原来的 612 簇，启动前校验原始 CAB 文件及两份 FAT 的哈希。`monitor-relation-completion.patch` 版 `win32k.sys` SHA-256 `05676C7324B1ECC98FF606A1BCC5EA16DFE134780A3C10EC4FBB14569C3BC3B1`，`monitor-list-completion.patch` 版 `3AF4D4B8117742901DB85D3E6497DE9B1F73670D7235278723ACA198DF24BFCB`。只替换此文件的普通入口对照验证到安装欢迎页。

[14 项回归](Checks/monitor-list.c)提取实际查询函数、完成例程和端口生产函数，检查内联、有/无子设备、pending 前/后完成、错误、取消及三处分配失败。用非空输出槽分别返回比指针少一字节、多一字节的长度，确认仅检查非空不足以通过用例。x64 / x86 MSVC 各 **14/14**，已有修复的 56 项也各自通过。复核：在 MSVC 开发者命令行运行 `python Docs\Notes\HyperV-Desktop-QA\Review\Checks\run.py Upstream\ReactOS\Source --suite monitor-list --suite ioctl`。四种配置的 18 / 19 / 20 / 21 个正式补丁已在临时 Git 索引中完整应用、逐个识别和逆序回退。

**正式镜像。** x64 Debug BootCD SHA-256 为 `1D6D0A0E27CFBD2B51CC2F73E655BC3C65B2005EE8567DE5D03A73558BA928BA`。默认 `Configure.cmd`、`Build.cmd`、`MakeISO.cmd` 同树完成，ISO 中的 `win32k.sys` 与上述坏盘测试的 `monitor-list-completion.patch` 二进制一致，`kernel32.dll` / `ntdll.dll` 与此前核对过的 `0x600` 版逐字节相同。从空盘安装到桌面、正常关机、无介质冷启动、Explorer 和记事本重开均成功。文本安装后的 1121 个文件未见短链，51 个字体与 CAB 一致；安装版应用 15/21（其中三项已修应用 14/14），旁加载候选 21/21，测试工具错误 0。虚拟机已还原到空盘关机检查点。

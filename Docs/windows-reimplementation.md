# Windows Reimplementation

## 目标

Kundows 以逆向分析为基础重新实现 Windows 功能，目标是尽可能匹配 Windows
的实际二进制接口和可观察行为。

有对应的微软公开源码时，尽量在源码层逐路径对齐 Windows，包括调用次序、分支条件、错误返回与异常、资源释放和通知时机。用匹配版本的 IDA 反编译、反汇编和 WinDbg 记录核对样例与实际驱动的差别。确因架构约束无法对齐的实现，在相应问题笔记中单列原因、调用方影响、验证范围及撤销条件。

**agent 根据任务自行判断分析深度、验证范围和所需工具，并区分事实、推断和未
验证假设。**

## 逆向分析与运行时调试

**IDA、Ghidra 和 WinDbg 应通过对应的 MCP 服务操作。**

**分析时同时参考反汇编和反编译结果。**反汇编用于确认真实的机器级行为，反编译
用于建立可读的语义模型；两者不一致时回到指令和运行时证据判断。

**WinDbg 可用于调试 Windows 用户态、内核态，以及在调试通道配置正确时调试
Hyper-V hypervisor、虚拟机内核和虚拟化相关路径。**应记录当前调试目标属于
guest、host 还是 hypervisor。

## 重新实现

以逆向和运行时证据指导实现，优先保持当前任务涉及的 ABI 和可观察行为。实现
细节由 agent 根据目标和证据自行判断；**未经验证的行为不得伪装成确定结论。**

## 逆向文档

对一个功能进行完整逆向时，应维护独立的分析文档，至少说明：

- 分析范围、目标模块、Windows build、架构和二进制身份；
- 模块关系、ABI、数据布局、主要流程和已确认行为；
- 使用的证据、验证范围、版本或架构差异；
- 未知语义、未实现路径和待验证事项。

**把已确认内容、较高可信度推断和未知内容分开记录；不要从单个调用点、字符串
或反编译命名推导出未经证实的完整语义。**静态发现、接口调用成功和测试通过
分别只能证明各自的范围。

## API Test

**API Test 既可用于测试 API 的正确性，也可用于确认 Kundows 与 Windows 二进制接口
兼容性和一致性。**

## Hyper-V 验证

Windows 宿主上的来宾验证只用 **Hyper-V**。如果普通 PowerShell 执行
`Get-VM` 时提示权限不足，用 `Start-Process powershell.exe -Verb RunAs -WindowStyle Hidden`
通过 UAC 在后台运行验证脚本。用 Hyper-V CIM 接口取帧和发送来宾按键，不占用宿主窗口焦点。
对照主干的安装结果时使用相同的固件和虚拟硬件：主干 x64 安装基线是
第一代（BIOS）、1 个虚拟 CPU、固定 1 GiB 内存、无网卡、串口断开。

### 初始化

配置两个逻辑角色：

- **Windows reference VM**：运行用于观察和对比的 Windows。
- **Kundows target VM**：运行当前 Kundows 构建。

为每个角色登记当前 VM 映射、guest 版本、架构、存储、控制通道、文件传输
通道和调试通道，并准备对应的干净 Hyper-V checkpoint。**VM ID 可以写入运行
记录，但不作为固定锚点。**

PowerShell Direct 可用于 guest 命令和文件传输；Guest Service 用于复制文件，
不负责执行文件。需要时配置网络、串口以及 WinDbg 的 guest 或 hypervisor
调试通道。

**只操作已登记的 disposable VM；无法确认逻辑角色或干净基线时停止。登记为
disposable fixture 的 VM 允许 agent 自主启动、修改、重启、调试、测试和还原。**

### 运行与恢复

运行前确认逻辑角色和干净 checkpoint，恢复基线后使用隔离目录部署测试内容。
测试中按需收集 guest 和 Hyper-V 两侧的状态。涉及磁盘破坏性操作时使用独立
VHDX，并确认 host 挂载和 guest 磁盘身份，不能只依赖磁盘编号。

验证安装盘时，让空白 IDE 硬盘位于 DVD 之前。选择文本安装和 FAT 快速格式化，
确认复制完成后出现的是**硬盘**的 FreeLdr 菜单。图形安装最终重启后，弹出 DVD 和
软盘，保存桌面截图，再通过来宾 `Msvm_Keyboard.TypeCtrlAltDel` 发送安全注意序列，
保存安全对话框截图。两张图共同验证桌面能够接收输入。

结束后清理本次运行创建的对象，恢复对应 checkpoint，并重新确认 VM、磁盘和
checkpoint 状态。

### 记录结果

把共用的安装步骤和夹具放在这里或[测试入口](../Upstream/ReactOS/Patches/Shared/README.md)。测试发现的问题按症状进入[问题索引](README.md#notes)：用同一输入记录修前、修后及 Windows 对照，注明观察的是来宾 API、宿主受控函数、调试器停点，还是关机后的盘面。一次运行遇到几个问题，可以共用证据，不必共用一篇结论。

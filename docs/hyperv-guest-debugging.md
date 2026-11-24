# Hyper-V 虚拟机安装验证、截图与故障取证

本指南来自 [ReactOS 原生 ISO 安装矩阵](reactos-native-iso-matrix.md)。最有用的习惯是：**看清当前画面再输入；记录上一次关机方式；在修改失败磁盘之前保存原件。** 三者决定了事后能否区分安装错误、掉电损坏和诊断操作造成的变化。

## 1. 后台观察和操作来宾

- 创建 VM 时记录配置名、VM GUID、目录、ISO SHA-256 和 VHDX 路径。控制器按 GUID 查找，再核对名称与所属目录。尤其在启动、停机、换盘和离线挂载前，确认接的是预期的盘。
- 不必打开 VMConnect 或抢宿主窗口焦点。Hyper-V 的 CIM 提供程序位于 `root/virtualization/v2`。下面是**只读**的取帧示例；`$vmId` 为本轮已核对的 VM GUID：

  ```powershell
  $ns = 'root/virtualization/v2'
  $vm = Get-VM -Id $vmId -ErrorAction Stop
  $system = Get-CimInstance -Namespace $ns -ClassName Msvm_ComputerSystem `
      -Filter "Name='$($vm.Id)'"
  $settings = Get-CimAssociatedInstance -InputObject $system `
      -Association Msvm_SettingsDefineState -ResultClassName Msvm_VirtualSystemSettingData
  $service = Get-CimInstance -Namespace $ns -ClassName Msvm_VirtualSystemManagementService
  $frame = Invoke-CimMethod -InputObject $service -MethodName GetVirtualSystemThumbnailImage `
      -Arguments @{TargetSystem=$settings; WidthPixels=[uint16]640; HeightPixels=[uint16]480}
  if ($frame.ReturnValue -ne 0) { throw "Hyper-V thumbnail: $($frame.ReturnValue)" }
  ```

- **`ImageData` 是像素，不是 PNG。** 本次 640×480 的解码方式为 RGB565，像素区恰好 `640*480*2` 字节。采集器接受该长度，或多 4 字节时跳过前 4 字节；其余长度报错。用 `Format16bppRgb565` 位图保存为 PNG。换宽度时应核对位图行跨度，不能原样套用连续复制。取帧失败时记录返回码、VM 状态、settings 实例及 video head，先排查采集链路，不要据此判定来宾黑屏。
- 输入走同一 `$system` 关联的 `Msvm_Keyboard`：单键 `TypeKey`，ASCII 文本 `TypeText`，安全注意序列 `TypeCtrlAltDel`。组合键先 `PressKey` 按下修饰键，再 `TypeKey`，在 `finally` 中逆序 `ReleaseKey`。检查方法返回码；成功只说明输入已发送，**下一张截图**才能说明来宾怎么响应。不要对宿主用 `SendKeys`。
- 原始 PNG 用配置名分目录，以时间戳和唯一后缀命名；放大图单独保存。阶段切换时关联保存操作、按键、VM 状态、uptime、磁盘、DVD、启动顺序和 COM 配置。控制器的精简输出可能出现 `State=null`；先看完整响应，不要把缺字段读成停机。

## 2. 安装通过的判据

本轮使用第一代 VM、单 CPU、固定 1 GiB 内存、无网卡、每次全新 8 GiB 动态 VHDX，正式测试断开 COM1/COM2。

1. 从 ISO 进入文本安装；读清分区和 **FAT 快速格式化**的高亮项，再确认。曾误选完整格式化，进度到 3% 时还未复制文件；那块盘作为中止的准备盘保留，换空盘重做。这种情况不应计为系统安装失败。
2. 文件复制完成后改为硬盘优先，标出**第一次从已安装硬盘启动**。否则可能重新进入光盘安装器，把循环安装看成磁盘启动失败。
3. 图形安装结束并重启后，保存桌面截图，再向来宾发送 Ctrl+Alt+Del，保存响应画面。出现桌面壁纸只证明画面已绘制；两张有因果关系的截图才能支持“桌面可交互”。LiveCD 和接调试器才成功的启动单独记。
4. 耐久性检查先请求来宾正常关机并确认 `Off`。每次记录是来宾关机、崩溃后停机还是强制断电。**曾强关的盘在下一次启动损坏，与全新安装首次磁盘启动失败是两种证据。** 不知道写坏发生在哪一阶段时，不要仅凭事后的磁盘状态归因。

## 3. 串口无输出、反复蓝屏或脚本不退出

| 现象 | 本次踩过的坑与处理 |
| --- | --- |
| KD 一直 `Waiting to reconnect` | 先核对实际启动项、COM 号、管道名，以及内核使用的是 KD 包协议还是文本串口。本轮 Win32 Release 使用文本后端，换用原始管道读取才看到异常。FreeLdr 打印调试横幅不能证明内核已连接。 |
| 抢不到启动菜单 | `TimeOut=0` 时，发键可能晚于菜单。停机后只在诊断副本上备份并修改 `freeldr.ini` 的默认项和超时。文件带隐藏/系统属性；本次用 `FileMode.Open` 写入、截断、刷新并恢复属性。 |
| 采集进程还在，日志却停了 | 崩溃后的串口可以一直保持连接而再无字节。设置连接超时、**异步读空闲超时**及字节上限；记录最后一条有效日志和停止原因。只设字节上限不能解除阻塞读。 |
| 子工具已退出，控制脚本仍在等 | `Start-Process -Wait` 可能连后代进程也一起等。短命启动器用返回的进程对象 `.WaitForExit()`；长驻调试器单独记录 PID 和管道，清理前核对身份。 |
| 蓝屏地址落在上下文捕获或断点处理 | 先保存**最早**的 first-chance 异常、模块基址、第一次 bugcheck 和参数，再看最后的停机画面。本轮最后显示 `RtlCaptureContext`，更早已有 Fastfat 展开异常和双重故障。Release 没有匹配 PDB 时，用与实际二进制核对过的 map 按 RVA 定位，不要强载 Debug PDB。 |

## 4. 留下可复查的失败现场

首次发现异常时先截图，记下启动阶段与磁盘历史。停机保留原 VHDX，用只读挂载的副本检查文件；需要改启动项、替换诊断内核或运行修复工具时另建可写副本。判读 FAT 时同时记录目录声明大小、所需簇数、实际链长、终止值及两份 FAT 是否一致：**两个镜像一致也可能同样错误**。比对安装文件时允许 Setup 的明确重命名，例如 `halmp.dll` 安装为 `hal.dll`，但仍要求内容哈希完全一致。

建议每次留下最小记录：`VM GUID / ISO 哈希 / 磁盘及其来源 / 启动阶段 / 上次关机方式 / COM 配置 / 操作前后截图 / 首个异常 / 结论及其适用范围`。后续人员应能仅凭这条记录区分正式验证与诊断启动，而不必找回原控制器会话。

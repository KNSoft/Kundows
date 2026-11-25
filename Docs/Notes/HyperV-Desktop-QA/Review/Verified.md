# 哪些缺陷已经修复

本页为 `2c9046e` 的验证基线，保留 G1 与随后出现的 G4 两个原始现场；**当前 G4 修复与同盘复测**另见[显示器列表完成](MonitorList.md)。

ReactOS `4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6`，x64 MSVC。下表统计的是**同一套宿主功能用例在修改前、修改后的结果**。它们执行从对应版本提取的生产函数，驱动写入、内存分配和锁的外部边界可控。原始 Hyper-V 故障盘的证据见[安装记录](../README.md#安装故障首次硬盘启动灰屏或蓝屏)。

| 默认补丁 | 原版 → 修后 | 用例验证的行为 |
| --- | ---: | --- |
| [G1：显示器目标关系请求的完成](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-monitor-relation-completion.patch) | 7/8 → 8/8 | 内联完成但交付 IOSB 的 APC 延迟时，仍能读到正确对象。另测正常内联、pending 返回前/后完成、错误、取消与分配失败。 |
| [M1：MM/CC 写回状态](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-mm-flush-status.patch) | 8/21 → 21/21 | 返回具体 I/O 错误，失败页保留脏位；范围部分成功、重新变脏、页换出、显式刷盘、后台和关机刷盘分别检查。 |
| [C1：重复 BCB 的所有权](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-cc-duplicate-bcb.patch) | 5/12 → 12/12 | 在两次查找之间发布已有 BCB；共享/独占固定都锁住返回的对象，不能等待时正确放弃。内存分配、页驻留失败和非固定映射也核对引用。 |
| [F1：FAT 卷清洁标记](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-fastfat-clean-after-flush.patch) | 6/10 → 10/10 | 文件系统已经返回失败时，普通卷刷盘不发出 `VolumeClean`、不清内存脏位；关机跳过 purge 和标清洁，仍向下层发关机请求。挂载时已脏、成功及关机异常路径均有对照。 |
| [G3：USER 初始化解锁](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-user-init-unlock.patch) | 4/5 → 5/5 | GDI 返回假时不调用 `UserInitialize`，走公共 `UserLeave`，原错误码不变。已注册、power 失败、USER 失败和完全成功均保持原行为。 |

**M1 的关键反例。** 三个脏页，中间有一页写失败：MM 在该页停止，只计入先前成功的一个脏页（4096 字节）；失败页和后续待写页留给下一次。成功 I/O 期间被重新修改的页仍然是脏页，因此不能用“写后是否还脏”推断 I/O 状态。`MmCheckDirtySegment` 给换出调用方的布尔值仍只回答“是否释放了页”。`Information` 延用原有的成功脏页计数单位；不等同于设备报告的传输字节，也不承诺 `CcFlushCache` 在错误时返回一个失败 VACB 内的部分字节。

仅改 MM 还不够：`CcRosFlushVacb` 会把自己摘下的失败视图挂回脏链表尾，而 `CcRosFlushDirtyPages` 每次从表头重新查找。用 `--mm-only` 把修后 MM 与原版 CC 配对，显式刷盘已正确返回错误，但后台在同一轮对同一未修改页发起了第九次写入，触发用例的停止条件。完整补丁在首次错误后，使用主锁保护的剩余脏视图数作为后续 I/O 尝试上限；健康视图仍有机会写回，下一轮可以重试。用一个持续失败的视图和一个健康视图分别走 lazy write 和 shutdown，检查队列、引用、有效数据长度与成功页数。[重叠写回补丁](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-mm-flush-collision.patch)仍是独立的 `--image-fixes`；只缩短了一行相邻调用的 diff 上下文，等待逻辑没变。两者叠加时，21 个状态用例和原有 7 个并发用例全部通过。

**C1 的清理契约。** `CcpGetAppropriateBcb` 返回 NULL 时，两个调用方 `CcpPinData`、`CcMapData` 都会放掉传入的 VACB 引用。原版在分配 BCB 失败时也放一次；只把 `iBcb->Lock` 改成 `DupBcb->Lock` 会漏掉这条错误路径，也会在锁竞争失败时泄漏临时 BCB。补丁统一为失败时调用方仍持有 VACB 引用，临时 BCB 的资源则由创建者销毁。本次没有更改全局 BCB 计数的同步策略。

**F1、G3 的边界。** F1 的前提是 `FatFlushVolume` **已经返回错误**。成功后的下层设备刷盘、定时清洁回调和已经被标清洁的卷不在修复范围；关机向上传统的 best-effort 状态不变。G3 保留已登记的 CSRSS 和前面已经完成的初始化步骤，不尝试重复初始化。现场没有证据表明 GDI 当时返回了假。

## G1：原故障盘普通入口的直接原因

在原有 Hyper-V 保存状态上离线回读第一次保护故障的陷阱帧。用于复核的 `.vmrs` 为 37,060,608 字节，SHA-256 `CAD3D076E69B231709B6EBAD035224E64EA5F1C4B01958934D394C577B95E9A9`；当时安装的 `win32k.sys` SHA-256 为 `AE71CD824C703307663CDC435CEEF209042C6A28868F8D77B34183A23E40D0CD`。

1. `EngpUpdateMonitorDevices+0xD0` 正要读 `Count`。它已把关系查询返回码 `0` 保存在栈上；取出的关系指针及陷阱帧 `RAX` 均为 `0xCCCCCCCCCCCCCCCC`。该 Debug 函数及查询助手的序言均用 `0xCC` 填充局部变量。
2. 由该版查询助手的栈帧布局还原局部 `Iosb` 和 `Event` 的**两个不同地址**（来宾地址分别以 `...75469088`、`...75469058` 结尾）。在故障线程的 IRP 链表中仅找到一项，`UserIosb` 和 `UserEvent` 同时匹配这两个完整地址。设备是 `\Driver\vga`，IRP 状态 `0`，`Information` 指向有效的 `Count=1` 关系，其唯一对象就是这次查询的功能设备对象。
3. 同一 IRP 的内嵌完成 APC 仍在这个线程的内核 APC 队列里。由此能区分“驱动没构造关系”和“驱动已完成，但结果尚未拷到调用方栈上”；本次属于后者。原陷阱帧保存的 `EFlags=0x10046`（IF 位为 0）；保存状态中的线程计数是 `KernelApcDisable=-1`、`SpecialApcDisable=0`。这些值不能定位屏蔽中断的源头，也不能归因于 USER 锁。

[补丁](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-monitor-relation-completion.patch)让查询方自行分配 IRP；完成例程通知事件并以 `STATUS_MORE_PROCESSING_REQUIRED` 取回所有权。pending 路径等待该事件，然后与内联路径一样从 IRP 读取最终状态和关系，再释放 IRP 一次。设备关系及所引用对象仍由原消费者释放。[回归](Checks/relation.c)用无害令牌标记未交付的 IOSB，不解引用坏指针。该证据定位的是**普通入口的这一次蓝屏**；它没有定位最初的 FAT 写坏路径。

### 原盘副本上的前后对照：越过 G1 后还有 G4

从同一份封存故障盘分别生成独立副本，同一 VM、同一旧安装光盘。只写 `win32k.sys` 已分配的 612 簇，必要时更新该目录项的文件长度。两份 FAT 的哈希与原盘完全相同，仍相差 29 项，`PALAI.TTF` 的活动链仍只有 1 簇。两种 `win32k.sys` 都含 G3；两次构建间唯一的源码变化是 G1。

| 故障盘副本 / 启动项 | 观察结果 |
| --- | --- |
| 未含 G1，普通入口 | 复现原 `STOP 0x1E / 0xC0000420`，断言地址仍在 `ntkrnlmp+0x1B4F09`。 |
| 含 G1，普通入口 | 通过目标关系查询和 `Count` 检查，但[仍然蓝屏](../Images/first-disk-after-g1-stop.png)。停止码相同，断言位置已变成 `win32k+0x1D93D2`，即 `EngpUpdateMonitorDevices+0x2B2` 的 **`bytesWritten == sizeof(pMonitorDevices)`**。再从独立副本复测，得到相同断言。 |
| 含 G1，默认 Debug，无串口调试器 | 165 秒内未到向导、仍为灰屏；保存时 CPU 在 `kdcom.dll+0x21F6`。 |

**G4 是紧接着的另一条 I/O 请求。** 第二次蓝屏的保存现场中，驱动是 `\Driver\vga`，调用参数是 `IOCTL_VIDEO_ENUM_MONITOR_PDO` (`0x230018`)，请求类型为 `IRP_MJ_DEVICE_CONTROL`。IO 控制助手的 IOSB、事件地址和输出槽同时与未交付的 IRP 匹配。驱动状态 `0`、报告 **8 字节**，系统缓冲区已有非空的列表指针；调用方却仍持有 `0xCCCCCCCCCCCCCCCC`，字节数为 `0xCCCCCCCC`。该 IRP 的完成 APC 仍在同一线程队列中。

[缓冲 IOCTL 检查](Checks/ioctl.c)调用固定版 `EngDeviceIoControl` 和 `VideoPortEnumMonitorPdo`：正常内联和 pending 时得到列表及 8 字节；内联返回而推迟复制时，同样返回成功，却拿不到列表和字节数。G1 的 PnP 关系放在 `Irp->IoStatus.Information` 中；原 G4 路径还依赖 I/O 管理器把系统缓冲区复制到调用方并释放它。只拷贝状态或删掉长度断言仍会使用无效输出。后续的[正式修法](MonitorList.md)限定在私有查询，通用 `EngDeviceIoControl` 的其他调用仍单独评估。

## 保留的调查项

| 项目 | 本轮可以确认的部分 | 为什么未接入 |
| --- | --- | --- |
| G2，视频端口的目标对象 | 原故障的 `\Driver\vga` 扩展确实同时持有 FDO 和一个不同的非空 PDO，`NextDeviceObject` 等于后者；返回关系却引用 FDO。[固定版分派用例](Checks/video.c)还覆盖了传统无 PDO 和分配失败分支。 | 只改成 `PhysicalDeviceObject` 会破坏无 PDO 适配器；主显示对象与次显示对象的挂接条件也不同。给主栈返回真 PDO 会实际启用同步 BusRelations 失效及重枚举，仍缺这项行为变化的端到端验证。 |
| C2，FAT 脏页寿命 | [真实 pin、unpin、FatFlushFat](Checks/bcb.c)：保持最后一个 pin 时，模拟目标收到 1 次写入；先解除最后一个 pin 时，刷表返回 `0`，目标收到 **0 次**，页仍脏。 | 已确认“没有 BCB 就已写完”在本版不成立。尚未选择在 CC 保留脏 BCB，还是显式同步 FAT 范围。保留 BCB 会改变引用和缓存回收；直接按页补调 `CcFlushCache` 则可能反复刷 256 KiB 的整个视图，还需验证写回中的页、元数据锁顺序和大 FAT 的开销。没有改 `PIN_IF_BCB` 的通用语义。 |

## 本机 Windows 对照

用 Windows SDK **CDB 离线打开系统映像**，加载微软公开 PDB。没有接管宿主内核，也没有从 Windows 复制实现代码。主机版本与单个系统组件的版本号不是同一件事。

- `fastfat.sys` **10.0.26100.9974**，SHA-256 `C046046AE5D4A5C5DEFC63237B07F8027DD059D83660D90AC431348C90676D39`。`FatFlushFat` 的调用表同样含 `CcPinRead` → `CcSetDirtyPinnedData` → `CcRepinBcb` → `CcUnpinData` → `CcUnpinRepinnedBcb`。`ntoskrnl.exe` 同版本的 `CcUnpinDataForThread` 可走 `CcUnpinFileDataEx`，这不证明 Windows 会像 ReactOS 一样在最后一次 unpin 就丢掉脏 BCB。
- 此 Windows 版本的 `FatCommonShutdown` 在 `FatFlushVolume` 返回后重新读取卷标志，再决定是否 purge / `FatMarkVolume`，此处没有检查刚返回的 `NTSTATUS`。**F1 是本库针对已观察到的 ReactOS 返回错误路径的保守策略，不是声称 Windows 有相同的检查或损坏表现。** 核对命令：`cdb -z %SystemRoot%\System32\drivers\fastfat.sys -c ".reload /f; lmvm fastfat; uf /c fastfat!FatFlushFat; uf fastfat!FatCommonShutdown; q"`（设置微软公开符号路径）。
- `win32kbase.sys` **10.0.26100.9974** 的 `EngDeviceIoControl` 调用 `GreDeviceIoControlImpl`，其调用表包含 `IoBuildDeviceIoControlRequest`、`IofCallDriver` 和 `KeWaitForSingleObject`。仅此不足以推断 Windows 的调用上下文或完成时序。

### FINDSTR：边界反例

同一宿主上分别向系统 `findstr.exe` 和此前在来宾通过 7 个短行用例的候选 EXE 输入相同字节。下面写作 `退出码 / stdout 字节数`；[脚本](Checks/findstr-boundaries.py)生成输入，可复核。

| 参数和输入 | Windows | 候选 |
| --- | ---: | ---: |
| `/B red`：1023 个 `a` 后接 `red\r\n` | 1 / 0 | 0 / 5 |
| `/E red`：1020 个 `a` 后接 `redZ\r\n` | 1 / 0 | 0 / 1025 |
| `/R /X r.d`：`red\r\n` | 0 / 5 | 1 / 0 |
| `/X "red blue"`：`red\r\nblue\r\n` | 0 / 11 | 1 / 0 |

前两例直接说明候选把 `fgets` 分块边界当成行首、行尾。后两例是原有模式解析尚不支持的行为，不能归为该补丁新引入的回归。FINDSTR 只保留[原候选](../Candidates/findstr-anchors.patch)；SORT、WHERE 和记事本则已按[来宾结果](../Results.tsv)接入默认构建。

## 如何复核

在仓库根目录打开 x64 MSVC 开发者命令行，需 Python 3、Git。脚本只在临时目录编译控制用例，不安装驱动；`--stock` 跳过子模块的工作树改动，读取固定 Git 对象。原版的非零退出码表示观察到表中差异。

```bat
set QA=Docs\Notes\HyperV-Desktop-QA\Review\Checks
python %QA%\run.py Upstream\ReactOS\Source --stock
python %QA%\run.py Upstream\ReactOS\Source
python %QA%\patch-sets.py
```

可用 `--suite writeback` / `bcb` / `fat` / `user` / `relation` / `video` / `ioctl` 单独运行；只有已选 `--image-fixes` 的源树应通过 `--suite collision`。G2/C2/G4 的输出是行为记录，不计入已修复的 56 项。F1 使用原卷分支、完成回调和关机函数，测试提取时去掉不相干的文件/目录打开类型；真实设备持久性不在宿主桩的观测范围。四种正式补丁组合（17、18、19、20 个）用独立 Git 索引正向应用、逐个反向识别，并逆序回退至固定版本的同一 Git 树。

使用 x86 MSVC 构建同一套宿主检查也通过 56/56。Win32 Release、ARM64 Debug 均已生成 VS 方案；下面所述安装测试的架构是 x64。

## Hyper-V 安装与磁盘对照

首轮集成（含 M1/C1/F1/G3 及三项应用、尚不含 G1）用 x64 Debug、`DLL_EXPORT_VERSION=0x600`、默认补丁的 BootCD `07F3E5A97A2FE73DB978FF2A89854B9720F00FFB7767C4D997CF367E981B8C2C`。从空盘安装，在首次硬盘 FreeLdr 菜单保存 VM，复制停止写入的差分盘，以只读、无盘符方式挂载副本。两份 FAT SHA-256 都为 `83EEDE5987C5AFB777C4DF69F71A0C9EB4552C5A1250F9F3E23D2838178BDB88`，表项差异 0；1121 个文件未见短链或非法链尾，`PALAI.TTF` 为 68 簇，51 个字体与 CAB 内容相同。随后进入桌面，关机，移除介质冷启动，再从 Explorer 和记事本打开文件。

该镜像的安装版 SORT、WHERE、记事本通过 **14/14**；FINDSTR 仍为 1/7，安装版合计 15/21，同盘旁加载的四项旧候选仍为 21/21，测试工具错误 0。逐字节回执 SHA-256 `5EFC15DB1CB54944DF5F1062E5EB54B5B1D6CA833FA3E1DC7B161EC213D4EF49`。这复核了三个正式应用补丁已进入安装介质。

**最终默认镜像**（再加入 G1）SHA-256：`74A4C476476E5F43B82EAE6129BF25E3A3EE2F68442F62DCAD96E84A83FDAA1B`。`Configure.cmd`、`Build.cmd`、`MakeISO.cmd` 同树完成；最终 ISO 上的 `ntkrnlmp.exe`、`win32k.sys`、`fastfat.sys` 和四项应用均与链接产物逐字节一致。`kernel32`/`ntdll` 保持 1,227/1,812 个命名导出，`GetTickCount64` 存在。

从空盘重装最终镜像，文本阶段结束时两份 FAT 与上一轮的哈希相同：0 个不同表项、1121 个文件未见短链或非法链尾、51 个字体与 CAB 相同。默认 Debug 启动到安装桌面，[应用回执](../Images/integrated-apps.png)与上一轮逐字节相同，四个安装版 EXE 的哈希与最终 ISO 相符。正常关机、无介质冷启动、Explorer 和记事本重开均通过，最后还原空盘关机检查点。**两次新安装的结果不能说明历史上 29 个 FAT 表项为何没有一致写回。**

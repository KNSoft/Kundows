# C2：整卷刷写返回成功，两份 FAT 仍是旧扇区

固定 ReactOS `4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6`，Hyper-V Gen 1，x64 Debug，1 vCPU / 1 GiB / 8 GiB IDE。在独立诊断介质上，安装程序完成一次普通文件分配，再对目标卷调用 `NtFlushBuffersFile`。**修前，调用和 IOSB 均为 `STATUS_SUCCESS`，紧接着绕过文件系统对两份 FAT 的读取都返回了旧值。** 这说明问题能到达调用方；只在安装结束时比较两份 FAT，会漏掉这段时间窗口。

这个现象也已用[同一个独立 Win32 EXE](../ApiTests/README.md)在无探针的系统上复核：Windows 通过，ReactOS 修前失败，只换修后的 `fastfat.sys` 后通过。

## FastFAT 的来源与缓存层的关系

ReactOS 的[第三方清单](../../../../Upstream/ReactOS/Source/media/doc/3rd%20Party%20Files.txt#L117-L121)把本驱动列为 Microsoft FastFAT，来源为 `Windows-driver-samples` 的 `16022c5`。[导入提交 `0daa5547d98`](https://github.com/reactos/reactos/commit/0daa5547d98)记录了对 NT 5.2 的移植；[`23b7c7b8234`](https://github.com/reactos/reactos/commit/23b7c7b8234)将其选为正式 FAT 驱动。比对[微软对应版本的 `FatFlushFat`](https://github.com/microsoft/Windows-driver-samples/blob/16022c5/filesys/fastfat/flush.c#L794)和 ReactOS 固定版 `4fbbefd614b`：忽略空白和结构化异常处理宏的拼写后，FAT12/16/32 的 pin 扫描逻辑一致。

关键是扫描的前提。微软源码注释只针对这个特殊的卷元数据流：没有 BCB（固定缓冲区的控制块）就假定后台已把那段写完。[`PIN_IF_BCB` 的公开文档](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccpinread)只规定已有 BCB 才固定，未命中并不是通用的磁盘写入完成通知。ReactOS 的 [`CcpFindBcb` / `CcpDereferenceBcb`](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/pin.c) 对 pin 查找要求 `PinCount > 0`，最后一个引用释放时可移除 BCB；脏状态另记在缓存视图和 MM 页上。未打本库补丁的固定版也有这两条规则。来宾直接记录到了**同一扇区 pin 未命中、`DirtyFatMcb` 仍脏**。因此可以把本次漏刷定位在 FastFAT 的假设与 ReactOS 缓存实现的交界处；Windows 的盘面结果验证了行为，不能单凭它断言 Windows 内部一定以哪种方式保留或刷掉 BCB。

## 为什么需要两个扇区

- **第 311 扇区，在第一个 256 KiB 缓存视图中。** 复制 `PALAI.TTF` 后立即刷卷。`FatFlushFat` 的 `PIN_IF_BCB` 未命中，`FatFlushVolume` 返回成功时，目标扇区仍在 `DirtyFatMcb` 里。一次带阶段记录的运行中，`FatMarkVolume(VolumeClean)` 进入时写序号为 236，离开时为 248；涵盖扇区的两次类驱动完成都在这段区间内。随后 `FatUnpinRepinnedBcbs` 前后序号未变。调用方读到的两份内容都和之前的固定页快照相同。这是**内部漏刷、返回之前被其他写入补齐**的对照。
- **第 544 扇区，在第二个缓存视图中。** 测试安装程序创建 `KWALLOC.BIN`，设置 EOF 为 128 MiB。`FSCTL_GET_RETRIEVAL_POINTERS` 返回的区间包含 FAT 项 69632–69760；fastfat 在填好尚未解除固定的页上取出 512 字节，逐项核对 69632→69633，直到 69759→69760。刷卷前后分别对只读、无中间缓冲的 `\Device\Harddisk0\Partition0` 读取两个绝对位置 **1,343,488 / 9,714,688**。卷句柄是另开的 `\Device\Harddisk0\Partition1`；测量期间保持文件打开，报告在测量后才创建。

首次跨视图试验的安装程序读了第 512 扇区，内核样本却在相邻的第 513 扇区；没有把这两个样本按同一扇区比较。下面的对照固定使用第 544 扇区，且以分配区间和记录的卷内偏移 **294,912** 双重核对。

近端会被补齐也和代码路径吻合：`FatMarkVolume` 更新 FAT 清洁位，`FatSetFatEntry` 对清洁操作执行 `CcUnpinRepinnedBcb(..., TRUE, ...)`；命中脏 VACB 时，`CcRosFlushVacb` 刷整个视图。第 311 扇区在这个视图里，第 544 扇区在它之外。

修前，四次读取全为成功、各 512 字节，内容都是零扇区 `076a27c79e5ace2a3d47f9dd2e83e4ff6ea8872b3c2218f66c92b89b55f36560`。固定页的 SHA-256 为 `69bb82dfdd80b45416d3ef217c6afed027d7e5b9951985ceb7791b88dfea3cd7`。以用户刷卷入口为零点，调用于 **27,275,342** 个来宾 QPC ticks 返回；两份覆盖该扇区的 4096 字节写分别在 **1,085,538,002 / 1,086,094,298** ticks 完成。到首次硬盘菜单再只读取样，两份都已等于固定页哈希，32,768 簇的分配链完整。该轮 109 批、218 次镜像写最终全部成功足额。

## 同一测量的修前 / 修后

| 第 544 扇区 | 修前（Aligned） | 修后（Fixed） |
| --- | --- | --- |
| 刷卷前，从两份 FAT 读回 | 两份均为零扇区 | 两份均为零扇区 |
| `FatFlushFat` pin 未命中；离开该函数时目标仍脏 | 是 / 是 | 是 / 是 |
| 离开 `FatFlushVolume` 时，目标仍在脏区表 | 是 | **否** |
| FAT1 / FAT2 类驱动完成，相对入口 ticks | 1,085,538,002 / 1,086,094,298 | **56,962,314 / 58,038,279** |
| 系统调用返回，相对入口 ticks | 27,275,342，成功 | **63,387,761**，成功 |
| 返回后立刻读回，FAT1 / FAT2 | 旧 / 旧 | **新 / 新**，与同一固定页逐字节一致 |
| 整次镜像请求（错误 / 少传） | 218（0 / 0） | 230（0 / 0） |

修前、修后第 544 扇区的固定页哈希相同。近端字体的起始分配则后移了一簇，因此第 311 扇区只与**各自同一轮**的固定样本比较；修后它的两份写也在 `FatFlushVolume` 内完成。两次安装结束后，整份 FAT 的镜像差异均为零、51 个字体都与安装 CAB 一致。

[两次实测逐事件表](FlushBoundary.tsv)包含样本、四次读取、系统调用入口/返回、阶段和覆盖目标扇区的类驱动完成。`phase` 的 4 为 pin 未命中，6/7 为离开 `FatFlushFat` / `FatFlushVolume`，12/13 为 `FatMarkVolume` 前后，14/15 为解除 repin 前后；第 544 扇区的阶段号统一加 20。`read-start` 的 ticks 是提交读取前的时间，哈希和字节数来自这次同步读取完成后。

诊断 ISO SHA-256：修前 `78C767454B3900E5A7BFF653AEF26E1566D084B93174F92FC5FB65F6E0DDA681`，修后 `0B0AD6CDE3C5A2471018EFAB269F7A25412839855F68920BEF4D9AD7F9F77D6B`。从这两张 ISO 取出的测试安装程序、类驱动、NTDLL、Kernel32 分别逐字节相同，fastfat 不同。两次 ETW 分别配对 48,369 / 48,385 个宿主活动，无缺失开始/结束或描述不符；全部 218 / 230 个来宾写请求的偏移和长度均在对应宿主记录中找到（按重复次数核对）。成功状态、实际字节数和 QPC 顺序来自来宾。

## 修复范围

[补丁](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-fastfat-volume-dirty-runs.patch)保留原有 pin / repin 刷表。只在 `FatFlushVolume` 的刷表步骤成功后，从 `DirtyFatMcb` 取首个实际脏区间（第 0 项为保留区空洞）。把该区间交给 `CcFlushCache`；分页写仍负责在镜像完成后移除记录。每次重新取首项，因一次缓存写可能连带清掉同视图的其他区间。空表不发出 I/O；稀疏的大 FAT 不需要为了这个补刷再全表读入。

MCB 查询返回时已释放它的互斥锁，调用 CC 时没有新增元数据锁或持有 BCB。记录上次请求的 64 位结束位置：如果 CC 报成功却仍看见已请求的区间（例如写入尚未完成），返回 `STATUS_DEVICE_BUSY`，保留记录供下次刷卷；不在原卷锁下循环等待。实际的 CC 写错误直接保留。已有的 F1 补丁决定刷卷失败时不能标清洁。其他 `FatFlushFat` 调用方、缓存管理器通用 pin 行为及可选 MM 重叠写补丁不在本次改动内。

单文件刷写值得另外测：`UserFileOpen` 路径持有的是文件 FCB；文件或父目录带有 `FCB_STATE_FLUSH_FAT` 时才调 `FatFlushFat`。在这个锁范围内等待**整个卷**的脏区间，可能把其他文件的并发分配算到当前请求上，不能直接沿用整卷的判断。

宿主 16 项整卷回归，在 x64 和 x86 MSVC 均为 **5/16 → 16/16**。包括 FAT12/16/32、仍有 pin、完全空表、跨视图、一份 1 GiB FAT 中只有远端一个脏扇区、多区间被一次 I/O 清掉、写失败后的重试、CC 返回成功但区间仍待完成、已存在的目录错误、只读和无效卷。这里的 FAT12/16 和 1 GiB FAT 是受控内存用例，实际来宾为 FAT32。19 / 20 / 21 / 22 个正式补丁组合均在独立 Git 索引里完成应用、反向识别和回退。

Win32 Release 的 `fastfat.sys` 已链接。ARM64 Debug 的 Configure 和 fastfat C 源码编译通过；链接所需的 `libntoskrnl` 项目先因把 `libntoskrnl_impalias.asm` 交给不支持 ARM64 的 MASM 规则而停止（MSB3721）。ARM64 未生成可测试的驱动或启动介质。

**默认 x64 Debug BootCD。** 逆向撤去诊断改动、逐字节校验六份源码备份后，通过同树的 `Configure.cmd`、`Build.cmd`、`MakeISO.cmd` 构建。`OutDir/bootcd-x64-Debug.iso` 的 SHA-256 为 `FE13B2AC52AF5AE6BD121EF39032744F77FBB33894C3BFC423E4FAC51111BC09`；其中 `fastfat.sys` 为 `46452af4151e66cafabf5db9e652fd8fd5b60906e02a8ed3c0d8a76acbbea4e8`。安装程序、类驱动、fastfat、NTDLL、Kernel32 均与对应链接产物逐字节相同。正式介质没有诊断标记，两个系统 DLL 与上一张 `0x600` 介质相同。

用这张正式介质从 Hyper-V 空盘装到桌面。文本安装后，1121 个文件没有短链或非法链尾；两表均为 `83eede59…`，差异 0，51 个字体与 CAB 一致。正常关机、弹出两种介质后的硬盘冷启动、再次打开 Explorer 和记事本均通过。应用回执与上一正式版本逐字节相同：安装版 15/21（含已知 FINDSTR 项），旁加载候选 21/21，测试工具错误 0。14 步回执及截图在 `OutDir/C2-Release-4fbb`；最后已还原空盘关机检查点。

## 正数短写：这台来宾的存储路径

从同一次保存的内存以及匹配的 PDB 确认，FAT 目标为 `partmgr` 分区对象，磁盘类设备为 `\Driver\Disk`、下层为 `\Driver\atapi`；PnP 链是 `IDE\DiskVirtual_HD…` → `PCIIDE\IDEChannel` → Intel `8086:7111`。atapi 的 PDO 扩展报告 `Virtual HD`、512 字节扇区、`TransportFlags=0`、`DeviceFlags=0x40e`、选择模式 `0x410`（PIO4 / MWDMA2）。这是 ATA 磁盘，`DEVICE_IS_ATAPI`、`DEVICE_PIO_ONLY` 和 `DEVICE_NCQ` 均未置位；这个快照不能逐请求判定是否发生 PIO 重试。

- `atapi/satl.c:AtaReqScsiReadWrite` 正常选 DMA，PIO 重试是另一分支。`pciidex/chipset/pata_io.c:PataProcessRequest` 的总线主控 DMA 分支按 DMA 和设备错误位判成败，**不返回一个正数残余**。ATA PIO 写尚有待传数据时继续传；没有 DRQ 则报总线复位。`pata_io.c:559–561` 减去残余再返回 `SRB_STATUS_DATA_OVERRUN` 的代码属于 **ATAPI 分组设备 PIO**，不是这个 ATA 磁盘的普通写。
- `AtaReqCompleteRequest` 把请求长度放入下层 IRP 的 `Information`，把最终 SRB 状态交还给类驱动；`AtaSrbStatusToNtStatus` 将上述 overrun 映射为 `STATUS_BUFFER_OVERFLOW`。classpnp 的 overrun 例外针对 MODE SENSE、INQUIRY 等可变长回复，不含普通 WRITE。`TransferPktComplete` 的成功分支按 **SRB** 长度累计；原请求成功时 Debug 检查合计长度等于请求长度，实际类驱动中也有对应的比较和 `int 2Ch`。这两层的字节计数不能混作一个。
- 同扇区修前 / 修后的 476 / 488 个与 FAT 范围相交的类驱动写包中，SRB 状态都是 1、IRP 状态都是 0，请求长度、SRB 长度、IRP 完成字节数全部一致。因此[宿主正数短写反例](README.md#待核实一份镜像返回成功但字节数不足)仍是单独的契约问题，不是这次漏刷中观测到的返回值。

## 复核

在同树的默认修复集上，仅为测试 ISO 叠加 [`fat-mirror-probe.patch`](fat-mirror-probe.patch) 和 [`flush-boundary-probe.patch`](flush-boundary-probe.patch)。修前保留其余默认补丁，只撤去 `fastfat-volume-dirty-runs`。用同一个空盘检查点跑文本安装，回收关机留证窗口的 `closed.vmrs`，从只读盘副本取 `KWFLUSH.BIN` 和 `KWFAR.BIN`，分别命名为 `boundary.bin`、`far.bin`。在 x64 Windows SDK 环境运行：

```text
python read-boundary.py <取样目录> --provider <x64-vmsavedstatedumpprovider.dll>
```

读取器核对内存中活动表的自指针、镜像表封闭标志、实际驱动、线程、用户调用窗口、缓存样本与所读扇区。`ticks` 是相对当次系统调用入口的来宾时钟，不与宿主 ETW 时钟直接相减。用 [`fat-volume.c`](../Review/Checks/fat-volume.c) 在 MSVC 运行 `run.py <源码目录> --suite fat-volume`；用 `--stock` 读取固定 Git 对象。它执行真实 `FatFlushFat`、`FatFlushVolume`、MCB 取区间和稀疏枚举，缓存和镜像完成边界由宿主用例控制。

原始证据在 `OutDir/FatBoundary-Aligned-4fbb`、`OutDir/FatBoundary-Fixed-4fbb`，各有 `closed.vmrs`、ETL、两份安装程序报告和离线盘面摘要。两轮 `far.bin` 的 SHA-256 分别为 `64584F4508C4DDDC1DA22160F11E5AD994B8B5B6F6C3AEC1BDD915988123BEEC`、`847D61A37952EE7BEE61AA26FE5DFFDA7C6C9ECA107A63DE883B1195EBAB0212`。

历史故障盘缺少首次分歧时的记录。这次第 544 扇区的**两份都暂时过期**，不能解释封存盘第 311 扇区为什么恰好只有一份缺了 29 项。

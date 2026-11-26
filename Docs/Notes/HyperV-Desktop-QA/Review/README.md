# 安装故障代码走查：显示器枚举与 FAT32 写回

## 当前默认候选：`bd620b9b`

[Cache manager investigation: from a missed FAT32 flush to file truncation and close](../../FAT32-Volume-Flush/README.md) follows each proposed fix, the observation that changed it, the final Windows comparison, and the remaining differences.

本页保留初次走查及单页用例。**后续结论**见[正式补丁、Windows 对照与修后复测](Verified.md)：当时已接入 `monitor-relation-completion.patch`、`mm-flush-status.patch`、`cc-duplicate-bcb.patch`、`fastfat-clean-after-flush.patch` 和 `user-init-unlock.patch`。后续[显示器列表修复](MonitorList.md)为 `monitor-list-completion.patch`。视频端口目标对象选择仍待验证；[整卷漏刷的最终结论](../../FAT32-Volume-Flush/README.md)见独立笔记。

基线为 ReactOS `4fbbefd614b`，对应[上次 Hyper‑V 安装现场](../README.md#安装故障首次硬盘启动灰屏或蓝屏)。这次分别追踪了**设备关系的生产者到消费者**，以及 **FAT32 脏扇区到缓存、页写回和卷清洁标记**。该版的 [FAT 识别器](../../../../Upstream/ReactOS/Source/drivers/filesystems/fs_rec/fat.c#L167)加载 `fastfat`；实际 x64 内核工程编入的是 [`ntoskrnl/cc`](../../../../Upstream/ReactOS/Source/ntoskrnl/ntos.cmake#L32)。

## 显示和启动

| 补丁或待查问题 | 走查发现与证据 | 与现场的关系、修复建议 |
| --- | --- | --- |
| `monitor-relation-completion.patch` | **读取设备关系之前，完成结果是否已经交付？** [`EngpPnPTargetRelationRequest`](../../../../Upstream/ReactOS/Source/win32ss/gdi/eng/device.c#L527) 的局部 `Iosb` 未初始化；只在 `STATUS_PENDING` 时等待并采用最终状态，其他成功返回直接使用 `Iosb.Information`。[视频驱动的成功分支](../../../../Upstream/ReactOS/Source/win32ss/drivers/videoprt/dispatch.c#L1113)实际分配一个关系、写入 `Count=1`、引用对象并完成 IRP。另一方面，普通 IRP [排队完成 APC](../../../../Upstream/ReactOS/Source/ntoskrnl/io/iomgr/irp.c#L1596)，由[完成路径](../../../../Upstream/ReactOS/Source/ntoskrnl/io/iomgr/irp.c#L389)写调用方 IOSB，再通知事件。 | **最贴近蓝屏，仍是待验证假设。** [584 行](../../../../Upstream/ReactOS/Source/win32ss/gdi/eng/device.c#L584)读取 `Count` 时已经出错，不能反推驱动返回了错误数量。只在 pending 时等待本身是常见用法；关键是实际线程是否允许完成 APC 及时投递。内核[同类 PnP 查询](../../../../Upstream/ReactOS/Source/ntoskrnl/io/pnpmgr/pnpirp.c#L69)还明确检查了这个前提。优先对照驱动返回、IOSB 写入和调用方取结果三者顺序。初始化出参和验证结果有益，但不能代替完成同步；也不宜在不能交付 APC 的上下文中盲目加等待。 |
| 视频端口目标对象选择 | **查询目标 PDO，却可能拿到适配器 FDO。** [`videoprt` 明确区分物理对象与功能对象](../../../../Upstream/ReactOS/Source/win32ss/drivers/videoprt/videoprt.c#L238)，但其 [FDO 的 `TargetDeviceRelation` 分支](../../../../Upstream/ReactOS/Source/win32ss/drivers/videoprt/dispatch.c#L1125)把当前 FDO 自身作为结果。`win32k` 把它交给 [`IoOpenDeviceRegistryKey`](../../../../Upstream/ReactOS/Source/ntoskrnl/io/pnpmgr/pnpmgr.c#L1647)，此 API 要求有效的物理设备对象。 | 有真实 PDO 的适配器也可能被[当作旧式设备跳过重新枚举](../../../../Upstream/ReactOS/Source/win32ss/gdi/eng/device.c#L586)。这是结果**内容**不符，发生在已经能读 `Count` 之后。建议分别覆盖 PnP 适配器和无 PDO 的传统 VGA，再决定由哪一层返回或转发目标关系。难度：中。 |
| `user-init-unlock.patch` | **GDI 初始化失败会漏放全局 USER 独占锁。** [`NtUserInitialize`](../../../../Upstream/ReactOS/Source/win32ss/user/ntuser/ntuser.c#L192)先取独占锁；电源初始化失败会 `UserLeave()`，但 `InitializeGreCSRSS()` 返回假时直接 `return`。后者在 [DirectX 或字体子系统初始化失败](../../../../Upstream/ReactOS/Source/win32ss/gdi/ntgdi/init.c#L84)时确实能返回假。 | 明确的错误出口不对称，后续需要 USER 锁的调用可能无法继续。释放锁的改动小；要支持重试还应审查先前设置的 CSRSS 引用和初始化状态。单个字体文件异常并不等于 `InitFontSupport()` 已返回假，现有灰屏现场未证明走了这条分支。 |

`monitor-relation-completion.patch` 涉及两个不同的状态块：驱动写 `Irp->IoStatus`，而 `win32k` 读自己传入的 `Iosb`。在这个固定版本，构造函数[保存了后者的地址](../../../../Upstream/ReactOS/Source/ntoskrnl/io/iomgr/irp.c#L894)，同步包装再[设置事件并将请求入队](../../../../Upstream/ReactOS/Source/ntoskrnl/io/iomgr/irp.c#L1121)。这也是为什么检查驱动写了 `Count = 1` 还不足以结案。

## 缓存与 FAT32

| 补丁或待查问题 | 走查发现与证据 | 影响、修复建议 |
| --- | --- | --- |
| `cc-duplicate-bcb.patch` | **BCB（缓存缓冲区控制块）重复创建时，锁与交给调用者的对象不一致。** [`CcpGetAppropriateBcb`](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/pin.c#L139)找到已有的 `DupBcb` 后，却获取临时 `iBcb->Lock`，增加的固定计数属于 `DupBcb`；成功分支随后销毁临时对象并返回已有对象。 | 源码可确认的对象配对错误；该分支的固定/解除固定不再由同一把锁保护。建议使获得锁、增加计数、返回和释放针对同一个对象，并核对失败清理。难度：中。没有证据表明安装故障时恰好进入此分支。 |
| FAT32 整卷漏刷（历史兜底：`fastfat-volume-dirty-runs.patch`） | **“无 BCB”被当成“脏 FAT 页已写完”。** [`FatFlushFat`](../../../../Upstream/ReactOS/Source/drivers/filesystems/fastfat/flush.c#L899)逐页用 `PIN_IF_BCB` 查询，未命中就跳过；本函数可返回成功。ReactOS 的 [`CcpFindBcb`](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/pin.c#L59)在此查询中只匹配覆盖请求且仍被固定的 BCB；[最后一个引用释放](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/pin.c#L81)就把 BCB 摘链，脏状态仍可留在[缓存视图的脏列表](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/view.c#L748)。[未命中分支](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/pin.c#L265)不会再问脏页状态。 | 这是 fastfat 与缓存管理器之间的假设不一致。一个同步刷表调用可能跳过仍待回写的页；后台写回仍可能稍后成功，故不能据此断定单份 FAT 为何落后。建议对照脏区、刷表返回和实际写完成再选修复层，避免把整个 FAT 无条件标脏。难度：中高。 |
| `mm-flush-status.patch` | **页写失败，显式刷盘仍可报告成功。** [`MmCheckDirtySegment`](../../../../Upstream/ReactOS/Source/ntoskrnl/mm/section.c#L5331)在 `MiWritePage` 失败时重新标脏；它的布尔返回值表示是否释放了页，并不是 I/O 成败。[`MmFlushSegment`](../../../../Upstream/ReactOS/Source/ntoskrnl/mm/section.c#L5175)仍把一页计入完成字节，并在正常循环出口返回 `STATUS_SUCCESS`。 | 上层 [`CcRosFlushVacb`](../../../../Upstream/ReactOS/Source/ntoskrnl/cc/view.c#L193)依赖此状态决定是否重新排入脏列表。建议单独传递写回状态，只累计真正成功的字节，保留失败页以便重试。现有可选 MM 补丁处理写入重叠，并未处理错误上报。难度：中。 |
| `fastfat-clean-after-flush.patch` | **刷卷已返回错误，卷仍可能被标为清洁。** 普通[卷刷新路径](../../../../Upstream/ReactOS/Source/drivers/filesystems/fastfat/flush.c#L405)取得 `FatFlushVolume` 的状态，但标记 `VolumeClean` 和清内存脏标志时未判断该状态。[关机路径](../../../../Upstream/ReactOS/Source/drivers/filesystems/fastfat/shutdown.c#L218)则直接丢弃返回值。两者都还有卷状态等其他条件。 | 与 `mm-flush-status.patch` 不同：即便下层**正确返回失败**，这里仍可能错误地宣告卷清洁。建议仅在刷卷成功时转为 clean，失败时保留脏标记；关机仍应继续通知下层设备。修正判断较小，需回归正常、失败及原已脏卷。 |

**归因边界。** fastfat [为每一份 FAT 构建写请求](../../../../Upstream/ReactOS/Source/drivers/filesystems/fastfat/write.c#L781)，并[等待结果再清除相应脏区](../../../../Upstream/ReactOS/Source/drivers/filesystems/fastfat/write.c#L834)。故障盘的 29 个差异不能简单解释成“源码只写了一份”。区分写请求未发出、单份写失败和较旧写入晚到，需要每份 FAT 的完成记录。显示一侧，`UserEnterShared/Exclusive` 使用普通临界区，仅凭持有 USER 锁不能推断特殊 APC 被禁用。略去 Debug 断言也无济于事：紧接着的[注册表查询](../../../../Upstream/ReactOS/Source/win32ss/gdi/eng/device.c#L588)仍假定第一个关系对象可用。

**建议顺序：** 按可独立修复的缺陷，先处理 `mm-flush-status.patch` 的写回状态、`user-init-unlock.patch` 的失败解锁，再单测 `cc-duplicate-bcb.patch` 的锁配对。若目标是定位这张故障盘，则分别记录 `monitor-relation-completion.patch` 的请求完成顺序，以及同一次刷表操作中 FAT1、FAT2 各自的完成状态和字节数。[`PIN_IF_BCB` 的接口说明](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccpinread)只说没有 BCB 时不能固定数据；因此整卷漏刷的回归判据应是最后一次解除固定后，刷卷是否使修改真正落盘。

本库的[同步 I/O 状态补丁](../../../../Upstream/ReactOS/Patches/Runtime/io-sync-status.patch)改的是文件信息查询/设置，没有覆盖 `monitor-relation-completion.patch` 的设备关系请求。它说明之前另有完成时序问题，不是 `monitor-relation-completion.patch` 在这次启动中发生的证据。

走查时核对了官方 `master` 的精确提交 `a3772c9df2a1`。上述关键分支涉及的 12 个文件（显示引擎 `device.c`、`videoprt` 的 `dispatch.c`/`videoprt.c`、`ntuser.c`、GDI `init.c`、MM `section.c`、CC `pin.c`/`view.c`、fastfat `flush.c`/`shutdown.c`、I/O `irp.c`、PnP `pnpirp.c`）在两个**上游 Git 版本**间逐字节相同。本库已有的 CC 补丁在解除固定时重标脏，不改 `cc-duplicate-bcb.patch` 的锁选择、`fastfat-volume-dirty-runs.patch` 的查找条件或 `mm-flush-status.patch` 的错误上报。

供上游追溯：视频端口目标对象选择 的分支来自[多显示器适配改动 #8638](https://github.com/reactos/reactos/pull/8638)。[WDK 的 `TargetDeviceRelation` 约定](https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/irp-mn-query-device-relations)明确要求，在 PnP 栈上由功能驱动下传、由父总线驱动返回一个已引用的子设备 PDO。`cc-duplicate-bcb.patch` 的锁选择可追到[重写 BCB 创建流程的提交](https://github.com/reactos/reactos/commit/1afcbbd12539577fb7bb6df61378d8dd12a4f727)；`mm-flush-status.patch` 累计页字节数的逻辑可追到[引入 `MmFlushSegment` 的提交](https://github.com/reactos/reactos/commit/90c6a65efe7384269f27fe419e164868c8654fff)。

## `mm-flush-status.patch` 的独立检查方法

[单页状态检查](mm-flush-status.c)直接调用固定版本的 `MmFlushSegment`；历史夹具只替代内核环境和底层页写入。单线程操作内存中的一页，分别覆盖无脏页、正常写、写失败和同一页重试。检查外层返回值、IOSB 状态、完成字节数、脏位和实际写入结果。

**x64 MSVC 实测：4 个用例中 3 个通过，只有底层写失败时的状态上报不符合预期。** 夹具用数值 `100` 表示本应写入的数据，目标初值为 `0`；失败时模拟的页写入返回 `STATUS_UNSUCCESSFUL`。

| 情况 | `MmFlushSegment` / `Iosb.Status` | 报告完成字节 | 页仍脏 | 目标值 |
| --- | --- | ---: | --- | ---: |
| 无脏页 | 成功 / 成功 | 0 | 否 | 0 |
| 写入成功 | 成功 / 成功 | 4096 | 否 | 100 |
| 写入失败 | **成功 / 成功** | **4096** | **是** | **0** |
| 同一页重试成功 | 成功 / 成功 | 4096 | 否 | 100 |

这直接验证了 `mm-flush-status.patch` 的错误上报：第一次失败后页仍可重试，但调用方已收到刷盘成功。它没有证明故障安装盘曾出现同一个底层写错误。

复查时，在临时目录放入本库 `26617bf:Build/Tests/` 中的 `CacheFlushCollisionTests.c`、`ExtractCachePinnedData.ps1`，从 ReactOS 的 `4fbbefd614b:ntoskrnl/mm/section.c` 取出 `section.c`，并复制本页的 `mm-flush-status.c`。在配置好 x64 MSVC 的 PowerShell 中执行：

```powershell
.\ExtractCachePinnedData.ps1 -Source "$PWD\section.c" -Output "$PWD\cache-flush-functions.inc" -FunctionNames MmCheckDirtySegment,MmFlushSegment
cl /nologo /TC /MT /Od mm-flush-status.c
.\mm-flush-status.exe
```

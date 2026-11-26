# Hyper-V CC 生命周期实测：定点保留还缺回收

本页记录 **V1 诊断工具** 的第一轮结果。当前源码已增加失败 purge / 重叠 flush 条件，CC 回收候选及后续实测见 [CcOwnership.md](../Review/CcOwnership.md)；不把后来源码的 54 项与这里的 39 项混为同一二进制。

**已在真实 ReactOS 来宾中验证，不是宿主模型。** 同一个诊断驱动和控制器，在观察版内核、原 Keep+Find 实验内核各跑完整的 8 个条件 / 39 个检查；两次无加载、协议、线程等待或用例执行错误，均正常关机并还原夹具。

结果不是“CC-only 正式修复已经通过”：**Keep+Find 修好了 5 个脏 BCB 命中检查，但新增 7 个回收失败。** 它仍只能作为[漏刷的因果实验](../FatTrace/CcLifetime.md)，不能替换正式的 `fastfat-volume-dirty-runs.patch` 整卷兜底。

## 配对条件

- ReactOS 固定 `4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6`；Hyper-V Gen 1，1 vCPU，1 GiB，同一桌面安装基盘的独立副本，8 GiB IDE 系统盘，无网络/光盘。
- 两轮系统的 FastFAT 都是**未含 `fastfat-volume-dirty-runs.patch` 整卷兜底**的 `C31CCEC07F365CCE759B6DC3550C689A0BFA6D4B8FD2B3E99481D822450713F5`。已有 `cc-duplicate-bcb.patch`、`mm-flush-status.patch`、unpin 重新发布脏位等修复仍在；观察版不是完全未打补丁的上游系统。
- 对照内核为 `CD5E7315CF09F8E9E40C0861DB7CAEB5B8D3BDC2B090111D1730A560AA8A44F0`；Keep+Find 为 `7D6222618A4D395EAE0E4D4221089DC6A1B6247EA1D68F370340EBAF336FB99F`，即此前已测试的相同两个私有 SMP 内核。
- 两轮 `cccycle.sys` 均为 `ADB64DDC29177C9FD3AD4983930742D42B48229F5954FFCCDB3B21AC7C280338`；`cccycle.exe` 均为 `2308F4744763B111A50BC6AFD6156AD87B8233F6BFB7EA05E4CAC3C5DAA9B6F9`。
- 测试数据来自驱动自己的稀疏内存 stream，不通过 FastFAT；因此不能把测试错误解释成 FastFAT 镜像写错误或宿主 VHD 故障。系统盘只用于启动、加载工具和保存输出。

两轮关机证据盘随后分别只读挂载：内核、FastFAT、诊断驱动、控制器的哈希均与上述产物一致；盘上的 `KWRS.TXT` 与取出的原始日志一致。`Verify-result.txt` 另核对了当前 VM 已关机、1 vCPU / 1 GiB、单一基线盘、无光盘/网卡。

## 39 个检查分开看

| 检查类别 | 对照 CC | Keep+Find |
| --- | ---: | ---: |
| 最后普通 pin / repin、失败写回、写入中再修改之后的脏 BCB 查找 | **0/5** | **5/5** |
| 其余写回内容、具体错误、重试、事件顺序、lazy 回调、活动干净 pin 的 purge 对照 | 27/27 | 27/27 |
| 成功写回之后的 purge / 截断回收 | **7/7** | **0/7** |
| 合计 | 34/39 | 32/39 |

这不是用总分宣称一版“更好”；两版失败的维度不同。逐 case 的检查号及计数在 [`CcCycle.tsv`](CcCycle.tsv)，含义由 [`cccycle.c`](cccycle.c) 的对应 `Check` 调用定义。

### 写失败与重试确实发生

case 3 先让该 stream 的分页 WRITE 返回 **`STATUS_IO_DEVICE_ERROR (0xc0000185)`、Information=0**，不是在产生脏页之前禁止写入。两版 `CcUnpinRepinnedBcb(TRUE)` 的 IOSB 都得到原错误；内存后端的目标模式仍为零，`FailedWrites=1`。恢复后再刷写，两版后端都得到 `0x33`，本项各有 2 个真实写 IRP。

区别在于失败之后：对照 `PIN_IF_BCB` 未命中；Keep+Find 可以命中。随后成功重试虽保存了内容，Keep+Find 的 purge 仍返回 `FALSE`，而对照返回 `TRUE`。

### 不是靠延时猜测并发

case 5 的系统线程开始写模式 `0x35`。驱动在 WRITE 派发时先保存 A 快照、把 IRP 标为 pending，然后通知 `WriteEntered` 事件。主线程等到此事件才重新 pin、写模式 `0x45`、标脏并 unpin；之后由它完成旧写。等实际系统线程退出，后端仍为 `0x35`；再刷一次才变成 `0x45`。两版均按这个顺序观察到两次写，所有事件/线程等待返回 0。

Keep+Find 让第二阶段的脏记录查找通过，但写完后仍不能 purge。**这是 1 vCPU 上强制出的两线程交错，不是 2+ vCPU 的并行锁压力测试。**

### 真正的后台写也暴露相同回收问题

case 6 除最初 pin/标脏/unpin 外不主动刷写；启用 write-behind，等待真实 `ReleaseFromLazyWrite` 回调。两版各观察到 `LazyWrites=1`、一次成功写入、后端模式 `0x36`。

回调之后 CC 仍会短暂持有临时引用，所以测试在最多 100 次 / 每次 10 ms 内尝试 purge。对照成功，Keep+Find 的 100 次均失败。不能用“lazy writer 本身已写完”代替“BCB 和 VACB 已可回收”。

### 为什么 purge 不能完成

定点干预只在最后引用减到零时保留 BCB，并让 pin 查找接受它；`CcRosFlushVacb` 成功后没有逐个清理这些 idle BCB。被保留的对象仍拥有其 VACB 引用。

`CcPurgeCacheSection` 的干净 VACB 分支要求只剩基准引用；多出的 BCB 引用使它返回 `FALSE`。活动干净 pin 的 case 7 是对照：两版持有 pin 时均返回 `FALSE`，解除后均为 `TRUE`，说明测试并非无条件要求 purge 成功。case 8 成功写回后截断到零，对照可回收，Keep+Find 仍不可回收。

这为此前的“不能只把 Dirty 条件加到最后释放出口”提供了实测反例。后续 CC-only 修复需要明确的脏记录所有权、写回代次与成功退休路径，而不是以 VACB 的瞬时 Dirty 位代替它们。

## 范围与尚未验证的部分

- 实际运行的是生产 CC API；错误注入位于诊断驱动自己的分页写完成边界，尚未测试实际 ATA/VHD 存储链的可控失败。
- 后端成功写入会复制完整的 4096 字节，但本版本数据检查记录的是模式页的**首字节**；不是整页逐字节哈希。外部 FAT32 的 128 项返回后核对仍由独立 `fatflush.exe` 完成，不能混为这套内存 stream 的结果。
- 各 case 最后调用真实 `CcUninitializeCacheMap` 并等待完成事件，未超时；这不能证明所有 pool 对象均已释放，也不等于真实卷的卸载/重新挂载验证。控制器没有把 SCM 的驱动卸载返回另列成检查。
- 未在 Windows 来宾加载此驱动；元数据式 stream 的脏 BCB 查找期望有静态 Windows 依据，不是 Windows 同驱动运行记录。
- 未覆盖两个写回同时进入 MM、强制低内存、2+ vCPU 锁竞争和长期重复挂载。新 CC 候选还应在这些条件、正常 FAT32 API 回归与安装/冷启动上继续验证。

## 复核

```text
python Docs/Notes/HyperV-Desktop-QA/KernelTests/summarize.py ^
    OutDir/CcCycle-4fbb/BeforeCycleControl01.log ^
    OutDir/CcCycle-4fbb/BeforeCycleBoth01.log
```

读取器必须看到全部 8 个 case、39 个不重复检查、正确的实际值/期望值/判词、零执行状态，以及实际发生的失败写和 lazy 回调，才接受计数。另有宿主解析器自检，拒绝不完整、伪 PASS、未触发错误/后台分支和退出码不符的日志。

`OutDir/CcCycle-4fbb/` 留存两份 `*-result.txt`、原始控制器输出、桌面与关机回执、`Control-observed.vmrs` / `Both-observed.vmrs` 和解出的 73 / 85 条 CC 事件。观察表未溢出；其中 map / BCB 地址可能在不同测试 stream 之间被 lookaside 复用，不能按同一标签把不同 case 的对象合并成一段寿命。

这轮未修改正式源码、补丁队列或默认 ISO。Hyper-V 夹具最后已回到关机基线，两个诊断产物只保留为证据，不安装到宿主。

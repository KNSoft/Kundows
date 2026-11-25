# 在 Hyper-V 来宾里控制 CC 的写回与回收边界

`cccycle.sys` 直接调用运行中内核的 CC API；`cccycle.exe` 只负责在**一次性测试来宾**里加载这个诊断驱动、逐项运行、输出检查值。不是在宿主执行生产函数的模型，也不是给真实磁盘插入过滤驱动。

**[第一轮实测](Results.md)** 保留 V1 工具的 8 条件 / 39 项结果：对照 34/39、Keep+Find 32/39。[后续 CC 回收候选](../Review/CcOwnership.md)已补齐成功写回退休，V3 实测 **54/54**，包括失败 purge 和重叠 flush；不是只沿用第一轮的 39 项。

## 隔离范围

- 驱动创建自己的设备、stream file object、FCB 和 section。稀疏内存后端只保存目标 4096 字节，其余读取为零。
- 无磁盘枚举、无文件路径写入、无附加设备栈、无内核 hook。报错和暂停仅发生在发给**这个私有 stream** 的分页写 IRP 上；不让系统盘变只读，也不向 Hyper-V 宿主注入存储错误。
- stream 的大小和目标页与 [CC 定点实验](../FatTrace/CcLifetime.md)一致，因此可用原观察内核及原 Keep+Find 内核直接对照，不需要再改 FastFAT。
- 设备控制请求在 PASSIVE_LEVEL 执行。线程和卸载等待有上限；如果出现异常、未交还的 pin 或线程超时，保留上下文并禁用驱动卸载，控制器停止测试。该次 VM 必须从宿主关机、还原，不把超时当作通过。
- 控制器会创建 `KwCcCycle` 内核服务，路径固定 `C:\CCCYCLE.SYS`。**不要在宿主运行它**，也不要把驱动装到生产 Windows；只把两个产物复制进关机的来宾盘副本。

## 当前工具：十个条件

当前协议 V3，共 54 个检查值。V1 为前八项（39 检查），V2 再加 case 9（44 检查）。失败仍输出实际值、期望值和该项 I/O 计数；执行异常另计，不混入兼容性失败。

| case | 条件 | 直接观察 |
| ---: | --- | --- |
| 1 | 最后一次普通 unpin | 脏 BCB 是否仍能被 `PIN_IF_BCB` 找到；显式写回内容；成功后 purge 是否完成。 |
| 2 | 普通 pin 已放，只留 repin；再放最后 repin | 两个阶段分别查找；不靠第二次阶段的结果猜第一次。 |
| 3 | write-through 的分页 WRITE 真正返回 `STATUS_IO_DEVICE_ERROR` | IOSB 必须保留该错误，内存后端仍为旧值；脏 BCB 可再次命中；恢复后重试保存新值、可回收。 |
| 4 | premark → 持有 pin 时刷写 → 再修改 → unpin | 第二次修改不额外调用 mark；确认已有 unpin 重新发布脏位的行为在真实内核里保存了新数据。 |
| 5 | 写 A 的 IRP pending 时，另一线程写 B | 驱动先保存 A 的快照，再通过事件通知测试线程；B 写完才完成 A 的 IRP。先确认后端是 A，再重试确认是 B。不是按延时猜测重叠。 |
| 6 | 实际 lazy writer | 除此项外禁用 write-behind；此项等 `ReleaseFromLazyWrite`，核对真实回调和后端内容。允许其短暂尾部引用释放后再判断 purge。 |
| 7 | 干净页上仍有活动 pin | 活动时 purge 不可成功；最后 pin 释放后应可成功，作为回收检查的对照。 |
| 8 | 成功写回后把 allocation / file size 截断至零 | `CcSetFileSizes` 及后续 purge 的可回收性。 |
| 9 | 同一视图的页 B 仍被 pin，页 A 是 idle dirty BCB | 全视图 purge 必须失败，并保留 A 的可查找性；不能先丢掉 A 的记录再因 B 拒绝。 |
| 10 | lazy 写 IRP 仍 pending 时另一个线程显式刷写 | 允许拒绝碰撞或等旧写完成，不允许旧 IRP 完成前返回成功。随后重试、内容和回收均检查。 |

`PIN_IF_BCB` 的脏记录期望针对这个空 CCB、公共头未设 `DO_MODIFIED_WRITE` 的元数据式 stream，依据是前述 Windows 静态分支和目标修复的不变量。**尚未把同一诊断驱动在 Windows 来宾上运行**，不能把所有期望都写成 Windows 动态实测结果。

case 5 在 1 vCPU 上已经能强制产生两线程交错，验证“旧写尚未完成时再次修改”的顺序；它不等于多处理器上的全套锁竞争验证。case 6 的 purge 检查最多等待 100 次、每次 10 ms，让 lazy-writer 回调后的临时引用正常释放，不能仅因刚收到回调时的一次 `FALSE` 就宣称泄漏。

## 构建与运行

先完成默认 x64 Debug 构建，再在 x64 MSVC 开发者环境执行：

```text
python Docs/Notes/HyperV-Desktop-QA/KernelTests/build.py
```

构建器复用现有 `null.sys` 的 ReactOS 头文件、导入库和本机编译选项，但替换编译单元、入口和所有输出路径；不覆盖默认驱动或内核。当前产物在 `OutDir/CcCycle-4fbb-v3/`，可用 `--out` 指定独立产物目录。V1/V2 证据产物分别保留在旧目录及 `CcCycle-4fbb-v2/`，不与当前源码混用。用户态控制器静态链接 CRT；两个 C 单元均以 `/WX` 编译。

把 `cccycle.exe`、`cccycle.sys` 分别放在一次性来宾的 `C:\CCCYCLE.EXE`、`C:\CCCYCLE.SYS`，运行：

```text
C:\CCCYCLE.EXE >C:\KWRS.TXT
echo APP_EXIT=%ERRORLEVEL% >>C:\KWRS.TXT
```

退出 `0` 为所有检查符合期望，`1` 为至少一个检查不符合，`2` 为加载、协议、超时或测试执行错误。输出按 case 列出 `reads / writes / errors / lazy`，控制器每项后立即刷新日志，发生来宾故障时能定位最后开始的条件。

保存 RAM 和日志以后，正常关机、从只读磁盘副本取日志并核对测试工具和内核哈希，最后还原关机基线。第一轮原始运行回执、保存内存和日志放在 `OutDir/CcCycle-4fbb/`；后续版本各有独立证据目录，见候选记录，不能把测试工具构建成功算成来宾验证通过。

用 `python summarize.py <控制器日志> ...` 验证日志完整性、错误/lazy 分支确已执行及退出码，再与第一轮 [`CcCycle.tsv`](CcCycle.tsv) 或后续 [`CcOwnership.tsv`](CcOwnership.tsv) 对照。诊断 stream 的内容只核对模式页首字节；真实卷的逐字节读回、卸载和代码层面的 SMP 审查另见候选记录。

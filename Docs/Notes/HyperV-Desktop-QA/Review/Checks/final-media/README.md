# x64 Debug 候选：从构建输入到 FAT 验收

当前默认候选的构建及来宾对照见[文件尺寸与关闭事件](../../CcSizeCompatibility.md)。本页保留早期预检和可重复的验收步骤。

## 早期预检（仅旧文件和夹具）

`OutDir/CcRuntime-4fbb/{configure,build,makeiso}.txt`：Solution 已生成；Build 1296 成功、0 失败、123 最新、14 跳过，`bootcd` 明列 skipped；MakeISO 日志停在项目输出，没有发布回执。当前发布位置 `OutDir/bootcd-x64-Debug.iso` 与 `OutDir/CcRuntime-4fbb/before/bootcd-x64-Debug.iso` 都是 263192576 字节的旧盘，SHA-256 `fe13b2ac52af5ae6bd121ef39032744f77fbb33894c3bfc423e4fac51111bc09`。同树的 `liveimg.iso`、旧诊断目录和 `FatBoundary-4fbb/*.iso` 均不是本轮 BootCD；后者甚至有多张大小完全相同。临时 `OutDir/amd64/Debug/bootcd.iso` 当前不存在。

`%SystemRoot%\System32\tar.exe -tvf` 在旧盘上列出以下三个 `amd64/` 常规文件，`reactos/` 对应项均是指向它们的硬链接（单独 `-xOf` 别名返回零字节）。从常规项以二进制 stdout 提取，实际结果如下；当前构建树内的三个同名链接输出与旧盘均不同。

| `amd64/` 内路径 | 字节 | SHA-256 |
|---|---:|---|
| `system32/ntoskrnl.exe` | 3096576 | `5108435b636a4bee2b10cad7467b992e660e42f2a14daea75de4cc9707c68864` |
| `system32/ntkrnlmp.exe` | 3103744 | `ccab9c71469ddd631baa456e0df1d7c686bba157a38b6735afb12ece4886cf9f` |
| `system32/drivers/fastfat.sys` | 528384 | `46452af4151e66cafabf5db9e652fd8fd5b60906e02a8ed3c0d8a76acbbea4e8` |

`amd64/reactos.cab` 实测 57554038 字节，tar 解出后可由 tar 或 `C:\Program Files\7-Zip\7z.exe l` 列目录，三项均无同名 CAB 成员。PATH 首选的 RosBE `7z.exe` 缺 DLL，故入口只用系统 tar。比较完整 PE 字节（含签名区）；未观察到需要去签名、解包 PE 或忽略字段的理由。介质布局或转换不同则停下查明，不作模糊匹配。

[只读入口](check.py) 仅输出 stdout，失败退出非零。实测目录 `OutDir/FinalQA-ijx39ktw`：`baseline` 使用 `good.json` 退出 0，同 ISO、同名、只改 MP 预期哈希的 `bad.json` 退出 1。`fixture/` 用另一个历史 ISO 和隔离的假输出树演练 `identity → links → media` 全链成功；去掉模拟 MakeISO 发布尾行、改变模拟构建输入分别失败。这里的链接文件来自夹具，回执是模拟的；这些结果只验证核对器。

## 最终构建者（候选冻结后）

1. 在**独占、无空格路径**的候选 checkout，以已审定的上游 pin、默认补丁及源码准备干净输入；不要复制活动树的忽略文件、生成头或对象。无旧 `OutDir/amd64/Debug`、发布 ISO 和同名临时 ISO；优先新 checkout，勿清其他工作者的树。冻结期间禁止并行修改。保存独立交付者认可的已应用队列身份；不能将刚读到的未知树自行认作获批版本。
2. 依赖：Windows x64，VS2026 Desktop C++、SDK、CMake 组件，RosBE 的 bison/flex/m4、骨架文件；不进 MinGW shell。本机缓存指向 VS 18 Community、`C:\RosBE`。根目录三个 cmd 实际分派给 [`Build/ReactOS.cmd`](../../../../../../Build/ReactOS.cmd)。Configure 运行 `vcvarsall x64`、检查 gitlink、识别/应用默认队列、调用 VS 自带 CMake + 上游 `toolchain-msvc.cmake`；不用 `--image-fixes`。只接受 `Visual Studio 18 2026` / `x64` / `amd64` / `Debug` / `DLL_EXPORT_VERSION:STRING=0x600`。Build 是 `devenv.com REACTOS.slnx /Build "Debug|x64"`；MakeISO 才会调用同树 `cmake --build ... --config Debug --target bootcd` 并将临时 `bootcd.iso` 移至发布位置。
3. **在 cmd.exe 批处理内**从该 checkout 根执行下列步骤。`Q` 必须全新；`approved.json` 是交付者在同布局、已应用默认队列的冻结输入上用 `python -I -B %C% identity` 生成的 JSON（原样 cmd 重定向）。核对实际输入而非仅比较 HEAD：包含 submodule pin、对 HEAD 的 binary diff 摘要、非忽略未跟踪文件、补丁及分派脚本哈希。忽略文件、工具链不是这一摘要的组成部分，故要求干净构建树及保留配置日志。

```bat
set "C=Docs\Notes\HyperV-Desktop-QA\Review\Checks\final-media\check.py"
set "Q=OutDir\Final-accepted-run01"
if exist "%Q%" exit /b 1
mkdir "%Q%" || exit /b 1
call Configure.cmd x64 Debug >"%Q%\configure.log" 2>&1 || exit /b 1
python -I -B "%C%" identity >"%Q%\input.json" || exit /b 1
fc /b approved.json "%Q%\input.json" >nul || exit /b 1
call Build.cmd x64 Debug >"%Q%\build.log" 2>&1 || exit /b 1
python -I -B "%C%" links "%Q%\input.json" >"%Q%\links.json" || exit /b 1
call MakeISO.cmd x64 Debug >"%Q%\make.log" 2>&1 || exit /b 1
copy /b OutDir\bootcd-x64-Debug.iso "%Q%\test.iso" >nul || exit /b 1
python -I -B "%C%" media "%Q%\input.json" "%Q%\links.json" "%Q%\make.log" "%Q%\test.iso" >"%Q%\media.json" || exit /b 1
```

保留 `CMakeCache.txt`、configure/build/make 日志及三个项目的 CL/link tlog。核对 build.log 中实际构建的 `ntoskrnl`、`ntkrnlmp`、`fastfat` 项目和输出；本工具内的三个 `PE` 路径相对于 `OutDir/amd64/Debug`。`links` 在 Build 后固定哈希；`media` 在 MakeISO 后重算输入、链接产物、发布盘与指定测试盘并提取比较。中途取消、非零退出、日志无本次发布尾行、快照漂移、测试副本不符均不得启动验收。相符证明的是内容身份；仍需新树及这次真实构建日志证明编译来源。

## 隔离来宾：普通用户态 FAT

实算已存在 `OutDir/ApiTests/fatflush.exe` SHA-256 `09C2AB157C56173FE0AD36DBA023A44D051DAC25A7A932FF7F29C459F407E986`，`fatcycle.exe` 为 `A23813874929A716C49B6D43F18CB8F5E38342335469FE1ECE3EABBB270A2C27`。使用这两份原件的核对过的副本，不重新编译来替代原 EXE。模板 `OutDir/ApiFlush-4fbb/Fixture.vhdx` 实算 `6A8E892125B3BFB4AC26041FF063E0B131A702E2B9760429DED8C3787EFDF6E7`。模板**仅复制**；绝不挂载模板。容量 512 MiB、当前宿主文件 36 MiB，MBR 单分区偏移 1 MiB、双 FAT32、512 字节逻辑扇区、4 KiB 簇、卷标 `KWFATQA`、根标记 `KWC2.ID`。契约及新建模板入口见 [ApiTests](../../../ApiTests/README.md)。

- 先提供本次专用且关机的隔离 VM 身份、全新空系统盘、足够空间及来宾内 EXE/日志传递途径。仅将 `media.json` 所列 `test.iso` 用于安装，记录 VM ID、DVD 介质实际绝对路径与 SHA-256；安装后正常关机、移除安装介质、冷启动。核对安装的 `system32/drivers/fastfat.sys` 和启动配置实际选用的内核文件（若 MP 安装时改名，按内容与 `links.json` 对应），保留选择依据和哈希；不能从文件名推断运行的是哪个内核。身份不符或无法确认时停止评分。不调用旧 VM 自动化或诊断探针。
- 在空的本轮目录做字节副本，例如 PowerShell `[IO.File]::Copy((Resolve-Path OutDir/ApiFlush-4fbb/Fixture.vhdx).Path, "$((Resolve-Path $Q).Path)\fat-test.vhdx", $false)`（先将 `$Q` 设为本轮目录），`Get-FileHash` 核对。仅在上述专用 VM 关机时将副本接到空闲的第二 IDE 磁盘位置；记录控制器、磁盘路径、来宾实际盘符。不给现有 VM、封存盘或宿主盘分配此角色。需要重测则重新从模板复制。
- 在**该来宾**中用有权刷卷的账户，从工具和日志目录（测试卷之外）执行，以下以已核对的 `X:` 为例。先 `vol X:`、`type X:\KWC2.ID`，确认不是系统卷且与第二盘身份一致。保留两份输出、各自退出码；任一阶段失败单报。

```bat
fatflush.exe X: > fatflush.log 2>&1
echo %errorlevel% > fatflush.rc
fatcycle.exe X: > fatcycle.log 2>&1
echo %errorlevel% > fatcycle.rc
```

`fatflush` 新分配 288 MiB 并核实连续区间；卷刷写**返回时**要求 FAT1/FAT2 各 128/128（各读足 512 字节），退出 0。记录 before，两份原为零才有上述前后对照；退出 1 是内容失败，2 是执行错误，77 是夹具不适用。`fatcycle` 独立判定：开文件锁卷被拒、关文件后锁定/卸载成功、旧句柄未挂载、按路径重挂，核对身份、EOF、3 × 8 MiB 逐字节均相符、退出 0。两项不互相替代。回收本轮输出，正常关机后仅分离本轮副本。

**资源与缺口：** 当前 Debug 树文件总长约 6.65 GiB；另需发布盘与测试副本各约 251 MiB、CAB 读取内存（基线约 55 MiB 加解析开销），每个 FAT 副本预留完整 512 MiB 加 VHDX 元数据。构建至少预留 12 GiB 工作余量（估算，非峰值保证），隔离系统 VHDX 的最大增长及可能的检查点另计。本次既未取得冻结候选与获批 `approved.json`，也未取得最终编译记录、新 ISO 或指定的新 VM/安装后身份及 FAT 输出。只读预检不能作为最终来宾成绩。

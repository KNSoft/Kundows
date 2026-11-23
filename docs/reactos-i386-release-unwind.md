# ReactOS i386 Release: frame-pointer omission during unwind

## What upstream configures

At ReactOS commit `1103f43a0c0eae4906828b708a8726587888ef5f`,
[`sdk/cmake/msvc.cmake`](https://github.com/reactos/reactos/blob/1103f43a0c0eae4906828b708a8726587888ef5f/sdk/cmake/msvc.cmake#L1-L8)
explicitly adds `/Ox /Ob2 /Ot /Oy` when `CMAKE_BUILD_TYPE` is `Release`.
`/Oy` allows the x86 compiler to omit the frame pointer. With a single-config
generator, an unspecified build type defaults to `Debug`, so a default build
is not equivalent to this Release configuration.

For `ARCH=i386`, the upstream [`rtl` target](https://github.com/reactos/reactos/blob/1103f43a0c0eae4906828b708a8726587888ef5f/sdk/lib/rtl/CMakeLists.txt#L104-L142)
includes both `i386/except.c` and `i386/except_asm.s`. No `/Oy-` override was
found for `except.c` in the upstream CMake rules. This is a source-level check
of the pinned configuration; the binary inspected below was built by native
MSBuild/MSVC, not by an upstream CMake build.

### When did this combination appear?

The EBP-based capture and its use by `RtlUnwind` date back to September
2005 (`b9020b11dde3`; `43fc14904289` sets ESP to `ebp+8`). Upstream
explicitly added `/Oy` to MSVC Release in May 2015 (`233d650aa0b`).
The 2021 edit to the current flag line (`5176f580e575`) separated `/GT`
for clang-cl; it did not introduce `/Oy`.

The pinned `.github/workflows/build.yml` includes i386 MSVC Release in
`build-msvc`; that job builds and uploads the ISO. It does not exercise
exception unwinding on an installed guest. `/Oy` permits, but does not
require, frame omission in any particular function. These dates establish
the age of the source and flag, not the first failing upstream binary.
No known-good and first-bad upstream ISO pair has been identified.

## What the native test establishes

`RtlUnwind` calls `RtlpCaptureContext`. The i386 assembly reads the return
address from `[ebp+4]` and derives the stack pointer from `ebp+8`; that
requires its caller to have established an EBP frame. Disassembly of the
native Release kernel shows `RtlUnwind` allocating locals relative to ESP
without establishing that frame before the call.

An isolated Win32 MSVC test compiled the upstream `RtlUnwind` body with its
context-capture assembly. A test double observed the context passed to
`ZwContinue` and compared it with the return address and stack pointer
recorded by the caller. Both variants used the same input and compiler:

| Variant | Return address (EIP) | Stack pointer (ESP) | Return value (EAX) |
| --- | --- | --- | --- |
| Release with `/Oy` | Wrong | Wrong | Correct |
| Same build, preserve the frame pointer only in `RtlUnwind` | Correct | Correct | Correct |

The same fixture gave these results with both MSVC 14.51 (v145) and
Visual Studio 2022 17.14 (v143, toolset 14.44.35207). For the VS 2022
run, the compiler logs confirm `/Ox /Ob2 /Oy` in both variants. The
baseline returned exit code 1 (both EIP and ESP mismatched); the
function-local frame-pointer control returned 0. This rules out a
14.51-only regression in the fixture; it does not date the first affected
upstream ISO.

This demonstrates a frame-pointer dependency in the upstream implementation.
It is consistent with the observed Win32 Release exception-unwind failure.
It does not establish why the installed SYSTEM hive's FAT chain was short.

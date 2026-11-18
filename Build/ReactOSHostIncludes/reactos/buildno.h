/* Host-tool fallback. The target build supplies the generated ReactOS header. */
#ifndef _INC_REACTOS_BUILDNO
#define _INC_REACTOS_BUILDNO

#define KERNEL_VERSION_BUILD        0
#define KERNEL_VERSION_BUILD_STR    "0-hosttools"
#define KERNEL_VERSION_BUILD_RC     "0-hosttools\0"
#define KERNEL_VERSION_RC           "0.0\0"
#define KERNEL_VERSION_STR          "0.0"
#define KERNEL_VERSION_REVISION_RC  "hosttools\0"
#define KERNEL_VERSION_REVISION_STR "hosttools"
#define KERNEL_VERSION_COMMIT_HASH  "hosttools"
#define REACTOS_DLL_VERSION_MAJOR   0
#define REACTOS_DLL_VERSION_RC      "0.0.0\0"
#define REACTOS_DLL_VERSION_STR     "0.0.0"
#define REACTOS_COMPILER_NAME       "MSVC"
#define REACTOS_COMPILER_VERSION    "v145"

#endif

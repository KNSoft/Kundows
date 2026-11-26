/* Diagnostic protocol, shared by a disposable-guest driver and its controller. */
#define KW_CC_IOCTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x831, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define KW_CC_VERSION 3
#define KW_CC_CASES 10
#define KW_CC_MAX_CHECKS 24
typedef struct {
    unsigned long Id, Actual, Expected;
} KW_CC_CHECK;
typedef struct {
    unsigned long Version, CaseId, Status, Count;
    unsigned long Reads, Writes, FailedWrites, LazyWrites;
    KW_CC_CHECK Checks[KW_CC_MAX_CHECKS];
} KW_CC_REPORT;

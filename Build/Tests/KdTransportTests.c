/* Deterministic host tests for the production KD serial packet implementation. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Substitute only the kernel/UART boundary, not the packet implementation. */
#define _KDDLL_H_
#define IN
#define OUT
#define OPTIONAL
#define NTAPI
#define KDDBGPRINT(...)
#define TRUE 1
#define FALSE 0
typedef void VOID, *PVOID;
typedef uint8_t UCHAR, *PUCHAR, BOOLEAN;
typedef uint16_t USHORT;
typedef uint32_t ULONG, *PULONG;
typedef struct { USHORT Length, MaximumLength; char *Buffer; } STRING, *PSTRING;
typedef struct { ULONG KdpDefaultRetries; BOOLEAN KdpControlCPending; } KD_CONTEXT, *PKD_CONTEXT;
typedef struct { ULONG PacketLeader; USHORT PacketType, ByteCount; ULONG PacketId, Checksum; } KD_PACKET;
typedef enum { KDP_PACKET_RECEIVED, KDP_PACKET_TIMEOUT, KDP_PACKET_RESEND } KDP_STATUS;
#define INITIAL_PACKET_ID 0x80800000
#define SYNC_PACKET_ID 0x800
#define PACKET_MAX_SIZE 4000
#define PACKET_LEADER 0x30303030
#define CONTROL_PACKET_LEADER 0x69696969
#define PACKET_LEADER_BYTE 0x30
#define CONTROL_PACKET_LEADER_BYTE 0x69
#define BREAKIN_PACKET_BYTE 0x62
#define PACKET_TRAILING_BYTE 0xaa
#define PACKET_TYPE_KD_STATE_CHANGE32 1
#define PACKET_TYPE_KD_DEBUG_IO 3
#define PACKET_TYPE_KD_ACKNOWLEDGE 4
#define PACKET_TYPE_KD_RESEND 5
#define PACKET_TYPE_KD_RESET 6
#define PACKET_TYPE_KD_STATE_CHANGE64 7
#define PACKET_TYPE_KD_POLL_BREAKIN 8
#define PACKET_TYPE_KD_FILE_IO 11
#define DbgKdExceptionStateChange 0x3030
#define DbgKdLoadSymbolsStateChange 0x3031
#define DbgKdPrintStringApi 0x3230
#define DbgKdGetStringApi 0x3231
#define DbgKdCreateFileApi 0x3430
static BOOLEAN NotPresent;
#define KD_DEBUGGER_NOT_PRESENT NotPresent
static struct { UCHAR KdDebuggerEnabled; } Shared;
#define SharedUserData (&Shared)
static UCHAR Tx[65536], Rx[65536];
static size_t TxSize, RxSize, RxPosition;
static unsigned Timeouts, AckAfterTimeouts, Failures, Tests;
static unsigned ReceiveErrors, ReceiveCalls;
static BOOLEAN LastControlCPending;
VOID NTAPI KdpSendByte(UCHAR byte);
KDP_STATUS NTAPI KdpReceiveByte(PUCHAR byte);
KDP_STATUS NTAPI KdpPollByte(PUCHAR byte);
KDP_STATUS NTAPI KdpPollBreakIn(VOID);

#include <drivers/base/kdcom/kdserial.c>
#include <drivers/base/kdcom/kddll.c>

#define CHECK(e) do { if (!(e)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #e); ++Failures; } } while (0)

VOID NTAPI KdpSendByte(UCHAR byte)
{
    if (TxSize == sizeof(Tx)) { fputs("Unbounded retry\n", stderr); exit(2); }
    Tx[TxSize++] = byte;
}

static void EnqueueAck(void)
{
    KD_PACKET ack = { CONTROL_PACKET_LEADER, PACKET_TYPE_KD_ACKNOWLEDGE, 0, CurrentPacketId & ~SYNC_PACKET_ID, 0 };
    memcpy(Rx + RxSize, &ack, sizeof(ack));
    RxSize += sizeof(ack);
}

KDP_STATUS NTAPI KdpReceiveByte(PUCHAR byte)
{
    if (++ReceiveCalls > 4096)
    {
        fputs("Unbounded UART error polling\n", stderr);
        exit(2);
    }
    if (ReceiveErrors)
    {
        --ReceiveErrors;
        return KDP_PACKET_RESEND;
    }
    if (RxPosition == RxSize && AckAfterTimeouts && Timeouts == AckAfterTimeouts)
    {
        EnqueueAck();
        AckAfterTimeouts = 0;
    }
    if (RxPosition < RxSize) { *byte = Rx[RxPosition++]; return KDP_PACKET_RECEIVED; }
    ++Timeouts;
    return KDP_PACKET_TIMEOUT;
}
KDP_STATUS NTAPI KdpPollByte(PUCHAR byte) { return KdpReceiveByte(byte); }
KDP_STATUS NTAPI KdpPollBreakIn(VOID) { return KDP_PACKET_TIMEOUT; }

static void Reset(void)
{
    TxSize = RxSize = RxPosition = Timeouts = AckAfterTimeouts = 0;
    ReceiveErrors = ReceiveCalls = LastControlCPending = 0;
    CurrentPacketId = INITIAL_PACKET_ID | SYNC_PACKET_ID;
    RemotePacketId = INITIAL_PACKET_ID;
    KdpConnectionEstablished = NotPresent = FALSE;
    Shared.KdDebuggerEnabled = 3;
}

static unsigned Send(ULONG type, ULONG message, unsigned retries)
{
    STRING header = { sizeof(message), sizeof(message), (char*)&message };
    KD_CONTEXT context = { retries, FALSE };
    size_t before = TxSize;
    KdSendPacket(type, &header, NULL, &context);
    LastControlCPending = context.KdpControlCPending;
    CHECK(context.KdpDefaultRetries == retries);
    return (unsigned)((TxSize - before) / (sizeof(KD_PACKET) + sizeof(message) + 1));
}

int main(void)
{
    Reset();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 2);
    CHECK(Timeouts == 2 && NotPresent && !KdpConnectionEstablished);
    CHECK(Shared.KdDebuggerEnabled == 1); /* KD stays enabled. */
    CHECK(CurrentPacketId == (INITIAL_PACKET_ID | SYNC_PACKET_ID));
    ++Tests;
    Reset();
    EnqueueAck();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 1);
    CHECK(!NotPresent && KdpConnectionEstablished && Shared.KdDebuggerEnabled == 3);
    CHECK(CurrentPacketId == (INITIAL_PACKET_ID ^ 1));
    ++Tests;
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 20);
    CHECK(Timeouts == 20 && NotPresent && !KdpConnectionEstablished);
    ++Tests;
    EnqueueAck();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 1);
    CHECK(!NotPresent && KdpConnectionEstablished && Shared.KdDebuggerEnabled == 3);
    ++Tests;
    Reset();
    CHECK(Send(PACKET_TYPE_KD_STATE_CHANGE64, DbgKdLoadSymbolsStateChange, 20) == 2 && NotPresent);
    ++Tests;
    Reset();
    CHECK(Send(PACKET_TYPE_KD_FILE_IO, DbgKdCreateFileApi, 20) == 2 && NotPresent);
    ++Tests;
    Reset();
    AckAfterTimeouts = 3; /* Genuine exceptions must not silently time out. */
    CHECK(Send(PACKET_TYPE_KD_STATE_CHANGE64, DbgKdExceptionStateChange, 20) == 4);
    CHECK(Timeouts == 3 && !NotPresent && KdpConnectionEstablished);
    ++Tests;
    Reset();
    AckAfterTimeouts = 3;
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdGetStringApi, 20) == 4);
    CHECK(!NotPresent && KdpConnectionEstablished);
    ++Tests;
    Reset();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 1) == 1 && NotPresent);
    ++Tests;
    Reset();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 0) == 1 && NotPresent);
    ++Tests;
    Reset();
    ReceiveErrors = 10000; /* An emulated UART can report persistent line errors. */
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 2);
    CHECK(ReceiveCalls == 2 && NotPresent && !LastControlCPending);
    ++Tests;
    Reset();
    ReceiveErrors = 1;
    EnqueueAck();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 2);
    CHECK(KdpConnectionEstablished && !NotPresent && !LastControlCPending);
    ++Tests;
    Reset();
    EnqueueAck();
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 1);
    ReceiveErrors = 10000;
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 20);
    CHECK(NotPresent && !KdpConnectionEstablished && !LastControlCPending);
    ++Tests;
    Reset();
    Rx[0] = BREAKIN_PACKET_BYTE;
    RxSize = 1;
    AckAfterTimeouts = 1;
    CHECK(Send(PACKET_TYPE_KD_DEBUG_IO, DbgKdPrintStringApi, 20) == 2);
    CHECK(KdpConnectionEstablished && LastControlCPending);
    ++Tests;
    printf("KD transport: %u cases, %u failures\n", Tests, Failures);
    return Failures ? 1 : 0;
}

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stddef.h>

// TH10 retains ".\src\core\midilib.cpp" diagnostics. MidiOutputView is a
// maintained descriptive type; the target proves the offsets and MIDI behavior
// used here, but does not retain the original class identifier.
class MidiOutputView {
public:
    unsigned int timerVtable;             // +00, opaque timer base vptr
    unsigned int timerId;                 // +04
    unsigned int timerPeriod;             // +08
    unsigned int timerOpaque0C;           // +0C
    int activeFileIndex;                  // +10
    MIDIHDR *pendingLongHeaders[32];       // +14
    unsigned char opaque94[0x138 - 0x94];
    void *tracks;                          // +138
    HMIDIOUT outputHandle;                 // +13C

    int UnprepareHeader(MIDIHDR *header);
    int StopPlayback();
};

typedef char MidiPendingLongHeadersAt14[
    (offsetof(MidiOutputView, pendingLongHeaders) == 0x14) ? 1 : -1];
typedef char MidiTracksAt138[
    (offsetof(MidiOutputView, tracks) == 0x138) ? 1 : -1];
typedef char MidiOutputHandleAt13C[
    (offsetof(MidiOutputView, outputHandle) == 0x13c) ? 1 : -1];

int MidiOutputView::UnprepareHeader(MIDIHDR *header)
{
    int i;
    for (i = 0; i < 32; ++i) {
        if (pendingLongHeaders[i] == header) {
            pendingLongHeaders[i] = NULL;
            midiOutUnprepareHeader(outputHandle, header, sizeof(*header));
            if (header->lpData != NULL) {
                free(header->lpData);
                header->lpData = NULL;
            }
            free(header);
            return 0;
        }
    }
    return -1;
}

int MidiOutputView::StopPlayback()
{
    if (tracks == NULL)
        return -1;

    for (int i = 0; i < 32; ++i) {
        if (pendingLongHeaders[i] != NULL)
            UnprepareHeader(pendingLongHeaders[i]);
    }

    if (timerId != 0)
        timeKillEvent(timerId);
    timeEndPeriod(timerPeriod);
    timerId = 0;

    if (outputHandle != NULL) {
        midiOutReset(outputHandle);
        midiOutClose(outputHandle);
        outputHandle = NULL;
    }

    activeFileIndex = -1;
    return 0;
}

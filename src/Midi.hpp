#ifndef TH10_RECONSTRUCTION_MIDI_HPP
#define TH10_RECONSTRUCTION_MIDI_HPP

#include <windows.h>
#include <mmsystem.h>
#include <stddef.h>

struct MidiTrackView
{
    int active;
    int nextEventTick;
    unsigned int dataSize;
    unsigned char runningStatus;
    unsigned char unknown00D[3];
    unsigned char *data;
    unsigned char *cursor;
    unsigned char *loopCursor;
    int loopEventTick;
};

struct MidiChannelView
{
    unsigned char heldNotes[16];
    unsigned char program;
    unsigned char bank;
    unsigned char pan;
    unsigned char reverb;
    unsigned char chorus;
    unsigned char volume;
    unsigned char scaledVolume;
};

// The timer trampoline calls the first vtable slot. Its 0x10-byte base
// keeps the derived 64-bit playback fields from changing the timer offsets.
struct MidiTimerView
{
    void StopTimer()
    {
        if (timerId != 0)
            timeKillEvent(timerId);
        timeEndPeriod(timerPeriod);
        timerId = 0;
    }
    ~MidiTimerView();
    virtual void Tick();
    unsigned int timerId;
    unsigned int timerPeriod;
    unsigned int timerOpaque0C;
};

// Target-derived partial owner. Unreviewed fields and the original
// class/allocation owner remain unknown.
class MidiOutputView : public MidiTimerView
{
public:
    int activeFileIndex;
    MIDIHDR *pendingLongHeaders[32];
    int pendingLongHeaderIndex;
    unsigned char *fileData[32];
    int trackCount;
    int unknown11C;
    int ticksPerQuarterNote;
    int tempoMicroseconds;
    unsigned __int64 elapsedTimerTicks;
    unsigned __int64 eventTickBase;
    MidiTrackView *tracks;
    HMIDIOUT outputHandle;
    unsigned char unknown140[0x14];
    MidiChannelView channels[16];
    signed char transpose;
    unsigned char unknown2C5[3];
    float fadeVolume;
    int fadeStep;
    int unknown2D0;
    int suppressVolumeUpdates;
    int unknown2D8;
    int resetState2DC;
    int fadeActive;
    int fadeDuration;
    int fadePosition;
    int loopTempoMicroseconds;
    unsigned __int64 loopElapsedTimerTicks;
    unsigned __int64 loopEventTickBase;

    ~MidiOutputView();
    void ReleaseTracks();
    int UnprepareHeader(MIDIHDR *header);
    int StopPlayback();
    void ResetTracks();
    virtual void Tick();
    void ApplyChannelVolumes(int adjustment);
    static void __stdcall ProcessEvent(MidiOutputView *output, MidiTrackView *track);
};

unsigned int MidiReadVariableLength(unsigned char **cursor);

typedef char MidiTrackSizeIs20[(sizeof(MidiTrackView) == 0x20) ? 1 : -1];
typedef char MidiTrackCursorAt14[
    (offsetof(MidiTrackView, cursor) == 0x14 &&
     offsetof(MidiTrackView, loopEventTick) == 0x1c) ? 1 : -1];
typedef char MidiChannelSizeIs17[(sizeof(MidiChannelView) == 0x17) ? 1 : -1];
typedef char MidiPendingLongHeadersAt14[
    (offsetof(MidiOutputView, pendingLongHeaders) == 0x14 &&
     offsetof(MidiOutputView, pendingLongHeaderIndex) == 0x94) ? 1 : -1];
typedef char MidiTimerIdAt04[
    (offsetof(MidiOutputView, timerId) == 4) ? 1 : -1];
typedef char MidiClockOffsets[
    (offsetof(MidiOutputView, trackCount) == 0x118 &&
     offsetof(MidiOutputView, ticksPerQuarterNote) == 0x120 &&
     offsetof(MidiOutputView, tempoMicroseconds) == 0x124 &&
     offsetof(MidiOutputView, elapsedTimerTicks) == 0x128 &&
     offsetof(MidiOutputView, eventTickBase) == 0x130) ? 1 : -1];
typedef char MidiTracksAt138[
    (offsetof(MidiOutputView, tracks) == 0x138) ? 1 : -1];
typedef char MidiOutputHandleAt13C[
    (offsetof(MidiOutputView, outputHandle) == 0x13c) ? 1 : -1];
typedef char MidiChannelAndFadeOffsets[
    (offsetof(MidiOutputView, channels) == 0x154 &&
     offsetof(MidiOutputView, transpose) == 0x2c4 &&
     offsetof(MidiOutputView, fadeVolume) == 0x2c8 &&
     offsetof(MidiOutputView, fadeActive) == 0x2e0 &&
     offsetof(MidiOutputView, fadeDuration) == 0x2e4 &&
     offsetof(MidiOutputView, fadePosition) == 0x2e8) ? 1 : -1];
typedef char MidiLoopClockOffsets[
    (offsetof(MidiOutputView, loopTempoMicroseconds) == 0x2ec &&
     offsetof(MidiOutputView, loopElapsedTimerTicks) == 0x2f0 &&
     offsetof(MidiOutputView, loopEventTickBase) == 0x2f8) ? 1 : -1];
typedef char MidiNativeHeaderSizeIs40[(sizeof(MIDIHDR) == 0x40) ? 1 : -1];

#endif

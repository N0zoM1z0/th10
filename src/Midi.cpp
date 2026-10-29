#include "Midi.hpp"
#include <stdlib.h>
#include <string.h>

// TH10 retains ".\src\core\midilib.cpp" diagnostics. MidiOutputView is a
// maintained descriptive type; the target proves the offsets and MIDI behavior
// used here, but does not retain the original class identifier.
// TH10 0x0043B8B0 is the one-byte default timer vtable callback.
// Its source-written versus generated origin remains unresolved.
void MidiTimerView::Tick()
{
}

// Target4203C0..4203EF retains both period-end calls.
void MidiTimerView::StopTimer()
{
    if (timerId != 0)
        timeKillEvent(timerId);
    timeEndPeriod(timerPeriod);
    timerId = 0;
}

MidiTimerView::~MidiTimerView()
{
    StopTimer();
    timeEndPeriod(timerPeriod);
}

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

// TH10 0x0043A730: unsigned accumulation wraps at 32 bits. Input bounds and
// a terminating byte are caller preconditions in the target.
unsigned int MidiReadVariableLength(unsigned char **cursor)
{
    unsigned int value = 0;
    unsigned char byte;
    do {
        byte = **cursor;
        ++*cursor;
        value = (value << 7) + (byte & 0x7f);
    } while ((byte & 0x80) != 0);
    return value;
}

// TH10 0x0043ACE0.
void MidiOutputView::ResetTracks()
{
    MidiTrackView *track = tracks;
    fadeVolume = 1.0f;
    resetState2DC = 0;
    fadeActive = 0;
    elapsedTimerTicks = 0;
    eventTickBase = 0;
    for (int i = 0; i < trackCount; ++i, ++track) {
        track->cursor = track->data;
        track->loopCursor = track->data;
        track->active = 1;
        track->nextEventTick = static_cast<int>(MidiReadVariableLength(&track->cursor));
    }
}

// TH10 0x0043B7A0. Conversion uses the historical x87 compiler surface;
// the following integer addition wraps before the signed clamp.
void MidiOutputView::ApplyChannelVolumes(int adjustment)
{
    if (suppressVolumeUpdates != 0)
        return;
    for (int channel = 0; channel < 16; ++channel) {
        unsigned int status = static_cast<unsigned char>(channel + 0xb0);
        int value = static_cast<int>(channels[channel].volume * fadeVolume);
        value = static_cast<int>(static_cast<unsigned int>(value) +
                                 static_cast<unsigned int>(adjustment));
        if (value < 0)
            value = 0;
        else if (value > 127)
            value = 127;
        if (outputHandle != NULL) {
            unsigned int message;
            unsigned char *bytes = reinterpret_cast<unsigned char *>(&message);
            bytes[0] = static_cast<unsigned char>(status);
            bytes[2] = static_cast<unsigned char>(value);
            bytes[1] = 7;
            // The target initializes only the three transmitted bytes.
            midiOutShortMsg(outputHandle, message);
        }
    }
}

// TH10 0x0043AF30. The vtable holds this ECX-receiver timer callback.
void MidiOutputView::Tick()
{
    int anyActive = 0;
    unsigned __int64 eventTime = eventTickBase + ((static_cast<unsigned __int64>(
                static_cast<__int64>(this->ticksPerQuarterNote)) *
            this->elapsedTimerTicks * 1000) /
        static_cast<unsigned __int64>(
            static_cast<__int64>(this->tempoMicroseconds)));
    if (fadeActive != 0) {
        if (fadePosition >= fadeDuration) {
            fadeVolume = 0.0f;
            return;
        }
        float level = 1.0f - static_cast<float>(fadePosition) / fadeDuration;
        fadeVolume = level;
        int step = static_cast<int>(level * 128.0f);
        if (step != fadeStep)
            ApplyChannelVolumes(0);
        fadeStep = static_cast<int>(fadeVolume * 128.0f);
        ++fadePosition;
    }
    for (int i = 0; i < trackCount; ++i) {
        if (tracks[i].active == 0)
            continue;
        anyActive = 1;
        do {
            MidiTrackView *track = &tracks[i];
            if (static_cast<unsigned __int64>(
                    static_cast<__int64>(track->nextEventTick)) > eventTime)
                break;
            ProcessEvent(this, track);
            eventTime = eventTickBase + ((static_cast<unsigned __int64>(
                static_cast<__int64>(this->ticksPerQuarterNote)) *
            this->elapsedTimerTicks * 1000) /
        static_cast<unsigned __int64>(
            static_cast<__int64>(this->tempoMicroseconds)));
        } while (tracks[i].active != 0);
    }
    ++elapsedTimerTicks;
    if (anyActive == 0)
        ResetTracks();
}

// TH10 0x0043B110: complete owned extent includes the three compressed
// selector tables through 0x0043B795, separate from the executable body.
void __stdcall MidiOutputView::ProcessEvent(
    MidiOutputView *output, MidiTrackView *track)
{
    unsigned char data1 = 0;
    unsigned char data2 = 0;
    unsigned char status = *track->cursor;
    if (status < 0x80)
        status = track->runningStatus;
    else
        ++track->cursor;
    unsigned char channel = status & 0x0f;
    unsigned char type = status & 0xf0;

    switch (type) {
    case 0xf0:
        if (status == 0xf0) {
            if (output->pendingLongHeaders[output->pendingLongHeaderIndex] != NULL)
                output->UnprepareHeader(
                    output->pendingLongHeaders[output->pendingLongHeaderIndex]);
            output->pendingLongHeaders[output->pendingLongHeaderIndex] =
                static_cast<MIDIHDR *>(malloc(sizeof(MIDIHDR)));
            MIDIHDR *header = output->pendingLongHeaders[output->pendingLongHeaderIndex];
            int length = static_cast<int>(MidiReadVariableLength(&track->cursor));
            memset(header, 0, sizeof(*header));
            unsigned int bufferLength = static_cast<unsigned int>(length) + 1;
            header->lpData = static_cast<char *>(malloc(bufferLength));
            header->lpData[0] = static_cast<char>(0xf0);
            header->dwFlags = 0;
            header->dwBufferLength = bufferLength;
            for (int i = 0; i < length; ++i) {
                header->lpData[i + 1] = *track->cursor;
                ++track->cursor;
            }
            int failed = 0;
            if (output->outputHandle != NULL) {
                if (midiOutPrepareHeader(output->outputHandle, header, sizeof(*header)) != 0)
                    failed = 1;
                else
                    failed = midiOutLongMsg(output->outputHandle, header, sizeof(*header)) != 0;
            }
            if (failed != 0) {
                if (header->lpData != NULL) {
                    free(header->lpData);
                    header->lpData = NULL;
                }
                free(header);
                output->pendingLongHeaders[output->pendingLongHeaderIndex] = NULL;
            }
            output->pendingLongHeaderIndex = (output->pendingLongHeaderIndex + 1) % 32;
        } else if (status == 0xff) {
            unsigned char meta = *track->cursor++;
            int length = static_cast<int>(MidiReadVariableLength(&track->cursor));
            if (meta == 0x2f) {
                track->active = 0;
                return;
            }
            if (meta == 0x51) {
                output->eventTickBase += ((static_cast<unsigned __int64>(
                static_cast<__int64>(output->ticksPerQuarterNote)) *
            output->elapsedTimerTicks * 1000) /
        static_cast<unsigned __int64>(
            static_cast<__int64>(output->tempoMicroseconds)));
                output->elapsedTimerTicks = 0;
                output->tempoMicroseconds = 0;
                for (int i = 0; i < length; ++i) {
                    // Target IMUL is 0x101, including for ordinary three-byte
                    // tempo payloads. Preserve its 32-bit wrapping recurrence.
                    output->tempoMicroseconds = static_cast<int>(
                        static_cast<unsigned int>(output->tempoMicroseconds) * 0x101u +
                        *track->cursor);
                    ++track->cursor;
                }
            } else {
                track->cursor += length;
            }
        }
        break;
    case 0x80:
    case 0x90:
    case 0xa0:
    case 0xb0:
    case 0xe0:
        data1 = *track->cursor++;
        data2 = *track->cursor++;
        break;
    case 0xc0:
    case 0xd0:
        data1 = *track->cursor++;
        data2 = 0;
        break;
    }

    switch (type) {
    case 0x90:
        if (data2 != 0) {
            data1 += output->transpose;
            output->channels[channel].heldNotes[data1 >> 3] |= 1 << (data1 & 7);
            break;
        }
    case 0x80:
        data1 += output->transpose;
        output->channels[channel].heldNotes[data1 >> 3] &= ~(1 << (data1 & 7));
        break;
    case 0xc0:
        output->channels[channel].program = data1;
        break;
    case 0xb0:
        switch (data1) {
        case 0:
            output->channels[channel].bank = data2;
            break;
        case 7:
        {
            output->channels[channel].volume = data2;
            int volume = static_cast<int>(data2 * output->fadeVolume);
            if (volume < 0)
                volume = 0;
            else if (volume > 127)
                volume = 127;
            output->channels[channel].scaledVolume = static_cast<unsigned char>(volume);
            data2 = static_cast<unsigned char>(volume);
            break;
        }
        case 91:
            output->channels[channel].reverb = data2;
            break;
        case 93:
            output->channels[channel].chorus = data2;
            break;
        case 10:
            output->channels[channel].pan = data2;
            break;
        case 2:
        {
            MidiTrackView *saved = output->tracks;
            for (int i = 0; i < output->trackCount; ++i, ++saved) {
                saved->loopCursor = saved->cursor;
                saved->loopEventTick = saved->nextEventTick;
            }
            output->loopTempoMicroseconds = output->tempoMicroseconds;
            output->loopElapsedTimerTicks = output->elapsedTimerTicks;
            output->loopEventTickBase = output->eventTickBase;
            break;
        }
        case 4:
        {
            MidiTrackView *saved = output->tracks;
            for (int i = 0; i < output->trackCount; ++i, ++saved) {
                saved->cursor = saved->loopCursor;
                saved->nextEventTick = saved->loopEventTick;
            }
            output->tempoMicroseconds = output->loopTempoMicroseconds;
            output->elapsedTimerTicks = output->loopElapsedTimerTicks;
            output->eventTickBase = output->loopEventTickBase;
            break;
        }
        }
        break;
    }
    if (status < 0xf0 && output->outputHandle != NULL) {
        unsigned int message;
        unsigned char *bytes = reinterpret_cast<unsigned char *>(&message);
        bytes[1] = data1;
        bytes[0] = status;
        bytes[2] = data2;
        midiOutShortMsg(output->outputHandle, message);
    }
    track->runningStatus = status;
    track->nextEventTick = static_cast<int>(
        static_cast<unsigned int>(track->nextEventTick) + MidiReadVariableLength(&track->cursor));
}

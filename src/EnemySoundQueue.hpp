#ifndef TH10_RECONSTRUCTION_ENEMY_SOUND_QUEUE_HPP
#define TH10_RECONSTRUCTION_ENEMY_SOUND_QUEUE_HPP
#include <stddef.h>

// Existing partial queue view of sound owner 0x00492590; no storage definition.
struct EnemySoundQueueView
{
    unsigned char unknown000[0x408];
    int cueValues[134];
    int activeSoundIds[12];
    int sampleCounts[12];
    int samples[12][128];

    __declspec(noinline) void QueueSoundCue(int soundId, float positionX);
    __declspec(noinline) void QueueSoundSample(int soundId, int sample);
};

typedef char EnemySoundCueValuesAt408[(offsetof(EnemySoundQueueView, cueValues) == 0x408) ? 1 : -1];
#endif

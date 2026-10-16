#include "Hint.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>

namespace
{
struct HintNameValue
{
    const char *name;
    int value;
};
// Target474494 and4744B0. Logical tables only: physical storage still open.
const HintNameValue alignmentNames[] = {
    {"center", 0}, {"left", 1}, {"right", 2}
};
const HintNameValue baseNames[] = {
    {"start",0},
    {"converse_1",1}, {"converse_2",2}, {"converse_3",3},
    {"converse_4",4}, {"converse_5",5},
    {"mboss_1",6}, {"mboss_2",7}, {"mboss_3",8}, {"mboss_4",9},
    {"mboss_5",10}, {"mboss_6",11}, {"mboss_7",12}, {"mboss_8",13}, {"mboss_9",14},
    {"mboss_card_1",15}, {"mboss_card_2",16}, {"mboss_card_3",17},
    {"mboss_card_4",18}, {"mboss_card_5",19}, {"mboss_card_6",20},
    {"mboss_card_7",21}, {"mboss_card_8",22}, {"mboss_card_9",23},
    {"boss_1",24}, {"boss_2",25}, {"boss_3",26}, {"boss_4",27}, {"boss_5",28},
    {"boss_6",29}, {"boss_7",30}, {"boss_8",31}, {"boss_9",32}, {"boss_10",33},
    {"boss_11",34}, {"boss_12",35}, {"boss_13",36}, {"boss_14",37},
    {"boss_15",38}, {"boss_16",39}, {"boss_17",40}, {"boss_18",41}, {"boss_19",42},
    {"boss_card_1",43}, {"boss_card_2",44}, {"boss_card_3",45},
    {"boss_card_4",46}, {"boss_card_5",47}, {"boss_card_6",48},
    {"boss_card_7",49}, {"boss_card_8",50}, {"boss_card_9",51},
    {"boss_card_10",52}, {"boss_card_12",53}, {"boss_card_13",54},
    {"boss_card_14",55}, {"boss_card_15",56}, {"boss_card_16",57},
    {"boss_card_17",58}, {"boss_card_18",59}, {"boss_card_19",60}
};
}

int __stdcall ExportAutomaticHints(HintOwnerView *owner, const char *path)
{
    char *buffer = static_cast<char *>(malloc(4096));
    _mkdir("hint");
    if (FileSystem::OpenWriteFile(path) == 0)
    {
        sprintf(buffer, "# ========================================================= \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        // CP932 literals46D510,46D4D8,46D490, not UTF-8 replacements.
        sprintf(buffer, "# \x93\x8c\x95\xfb\x95\x97\x90\x5f\x98\x5e\x81\x40\x8d\x55\x97\xaa\x83\x71\x83\x93\x83\x67\x83\x74\x83\x40\x83\x43\x83\x8b \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "#\r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "# \x82\xb1\x82\xcc\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xcd\x81\x41\x8e\xa9\x93\xae\x93\x49\x82\xc9\x90\xb6\x90\xac\x81\x41\x8f\xe3\x8f\x91\x82\xab\x82\xb3\x82\xea\x82\xdc\x82\xb7 \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "# \x8e\xa9\x95\xaa\x82\xc5\x8d\x55\x97\xaa\x83\x71\x83\x93\x83\x67\x82\xf0\x8d\x5c\x90\xac\x82\xb5\x82\xbd\x82\xa2\x8f\xea\x8d\x87\x82\xcd\x81\x41\x95\xca\x82\xcc\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xf0\x8e\x67\x97\x70\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2 \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "#\r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        time_t now;
        time(&now);
        tm *stamp = localtime(&now);
        sprintf(buffer, "#                                Time-stamp: <%.4d/%.2d/%.2d %.2d:%.2d>\r\n",
                stamp->tm_year + 1900, stamp->tm_mon + 1, stamp->tm_mday,
                stamp->tm_hour, stamp->tm_min);
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "\r\n\r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "Version = %s\r\n\r\n", "0.0");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));

        for (int stage = 0; stage < 8; ++stage)
        {
            int emitted = 0;
            for (HintLinkView *node = owner->exportStageLists[stage].next; node; )
            {
                HintLinkView *next = node->next;
                HintEntryView *entry = node->entry;
                if (entry->remaining != 0)
                {
                    if (emitted == 0)
                    {
                        sprintf(buffer, "# ================================== \r\n");
                        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                        sprintf(buffer, "Stage : %d\r\n\r\n", stage);
                        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    }
                    sprintf(buffer, "Tips\r\n", stage);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    if (entry->remaining > 0)
                        sprintf(buffer, "\tRemain\t: %d\r\n", entry->remaining);
                    // Native negative-remaining path writes the prior Tips again.
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tText\t: \"%s\"\r\n", entry->text);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tPos\t\t: %d, %d\r\n",
                            static_cast<int>(entry->position[0]), static_cast<int>(entry->position[1]));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tCount\t: %d\r\n", entry->count);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    const char *baseName = 0;
                    for (int baseIndex = 0; baseIndex < 61; ++baseIndex)
                        if (baseNames[baseIndex].value == entry->base)
                        {
                            baseName = baseNames[baseIndex].name;
                            break;
                        }
                    sprintf(buffer, "\tBase\t: %s\r\n", baseName);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    const char *alignmentName = 0;
                    for (int alignmentIndex = 0; alignmentIndex < 3; ++alignmentIndex)
                        if (alignmentNames[alignmentIndex].value == entry->alignment)
                        {
                            alignmentName = alignmentNames[alignmentIndex].name;
                            break;
                        }
                    sprintf(buffer, "\tAlign\t: %s\r\n", alignmentName);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tTime\t: %d\r\n", entry->time);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tAlpha\t: %d\r\n", static_cast<int>(entry->alpha));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tColor\t: %d, %d, %d\r\n", static_cast<int>(entry->red),
                            static_cast<int>(entry->green), static_cast<int>(entry->blue));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tScale\t: %.1f\r\n", static_cast<double>(entry->scale));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "End\r\n\r\n", stage);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    ++emitted;
                    if (emitted >= 255)
                        break;
                }
                node = next;
            }
            if (emitted != 0)
            {
                sprintf(buffer, "StageEnd\r\n");
                FileSystem::WriteToOpenFile(buffer, strlen(buffer));
            }
        }
    }
    FileSystem::CloseWriteFile();
    free(buffer);
    return 0;
}

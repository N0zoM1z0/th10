#include "Main.hpp"
#include "Hint.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Target41A0A0. Shift the terminating NUL along with each leading byte.
static char *HintTrimWhitespace(char *text)
{
    while (*text == ' ' || *text == '\t' || *text == '\n' || *text == '\r')
    {
        int length = static_cast<int>(strlen(text));
        for (int i = 0; i < length; ++i)
            text[i] = text[i + 1];
    }
    int last = static_cast<int>(strlen(text)) - 1;
    while (last >= 0 && (text[last] == ' ' || text[last] == '\t' ||
                         text[last] == '\n' || text[last] == '\r'))
        text[last--] = 0;
    return text;
}

// Target419F40. The unbounded input scans and uncapped final copy are native
// quirks. No sentinel, delimiter normalization, or final-tail trim is invented.
static char *__stdcall HintReadLine(char *cursor, char *line, int *remaining,
                          unsigned int capacity)
{
    memset(line, 0, capacity);
    while (*line == 0)
    {
        char *end = strchr(cursor, '\n');
        if (end == 0)
            end = strchr(cursor, '\r');
        if (end == 0)
        {
            memcpy(line, cursor, *remaining);
            *remaining = 0;
            break;
        }
        int copySize = static_cast<int>(end - cursor);
        if (copySize >= static_cast<int>(capacity - 1))
            copySize = capacity - 1;
        memcpy(line, cursor, copySize);
        *remaining -= static_cast<int>(end - cursor);
        cursor = end;
        while (*cursor == '\n' || *cursor == '\r')
        {
            ++cursor;
            --*remaining;
        }
        char *comment = strchr(line, '#');
        if (comment != 0)
        {
            memset(comment, 0, capacity - (comment - line));
            *comment = 0;
        }
        HintTrimWhitespace(line);
    }
    return cursor;
}

// Target41A050. A colonless directive does not reset the value buffer.
static void HintSplitLine(const char *line, char *key, char *value)
{
    strcpy(key, line);
    char *colon = strchr(key, ':');
    if (colon == 0)
    {
        HintTrimWhitespace(key);
        return;
    }
    strcpy(value, colon + 1);
    *colon = 0;
    HintTrimWhitespace(key);
    HintTrimWhitespace(value);
}

// Target4198C0. An unmatched token selects value zero.
static int __stdcall HintLookupValue(const char *name, const HintNameValue *table, int count)
{
    for (int i = 0; i < count; ++i)
        if (strcmp(name, table[i].name) == 0)
            return table[i].value;
    return 0;
}


int __stdcall LoadHintFile(HintOwnerView *owner, const char *path, int mode)
{
    int stage = -1;
    int entries = 0;
    int remaining;
    char *file = reinterpret_cast<char *>(FileSystem::OpenFile(path, &remaining, 1));
    if (file == 0)
        return -1;

    char *line = static_cast<char *>(malloc(4096));
    char *key = static_cast<char *>(malloc(4096));
    char *value = static_cast<char *>(malloc(4096));
    char *cursor = file;
    while (remaining > 0)
    {
        cursor = HintReadLine(cursor, line, &remaining, 4096);
        // Both passes are present in the native outer loop.
        HintSplitLine(line, key, value);
        HintSplitLine(line, key, value);
        if (strcmp(key, "Version") == 0)
        {
            if (strcmp(value, "0.0") != 0)
            {
                ClearHintLists(owner);
                break;
            }
        }
        else if (strcmp(key, "Stage") == 0)
        {
            stage = atol(value);
            entries = 0;
            if (stage < 1 || stage > 7)
                stage = -1;
        }
        else if (strcmp(key, "StageEnd") == 0)
            stage = -1;
        else if (strcmp(key, "Tips") == 0 && stage > 0)
        {
            if (entries >= 255)
            {
                stage = -1;
                continue;
            }
            HintEntryView *entry = new HintEntryView;
            ++entries;
            memset(entry->unknown18, 0, sizeof(entry->unknown18));
            while (remaining > 0)
            {
                cursor = HintReadLine(cursor, line, &remaining, 4096);
                HintSplitLine(line, key, value);
                if (strcmp(key, "Pos") == 0)
                {
                    char *comma = strchr(value, ',');
                    if (comma != 0)
                    {
                        *comma = 0;
                        ++comma;
                        HintTrimWhitespace(value);
                        HintTrimWhitespace(comma);
                        entry->position.x = static_cast<float>(atol(value));
                        entry->position.y = static_cast<float>(atol(comma));
                    }
                }
                else if (strcmp(key, "Text") == 0)
                {
                    char *first = strchr(value, '"');
                    if (first != 0)
                    {
                        ++first;
                        char *last = strrchr(first, '"');
                        if (last == 0)
                            break;
                        *last = 0;
                        strncpy(entry->text, first, 65);
                    }
                }
                else if (strcmp(key, "Count") == 0)
                    entry->count = atol(value);
                else if (strcmp(key, "Time") == 0)
                    entry->time = atol(value);
                else if (strcmp(key, "Base") == 0)
                    entry->base = HintLookupValue(value, baseNames, 61);
                else if (strcmp(key, "Align") == 0)
                    entry->alignment = HintLookupValue(value, alignmentNames, 3);
                else if (strcmp(key, "Remain") == 0)
                    entry->remaining = atol(value);
                else if (strcmp(key, "Scale") == 0)
                {
                    double parsedScale = atof(value);
                    entry->scale = static_cast<float>(parsedScale);
                    if (parsedScale > 4.0)
                        entry->scale = 4.0f;
                    else if (parsedScale < -4.0)
                        entry->scale = -4.0f;
                }
                else if (strcmp(key, "Color") == 0)
                {
                    char *first = strchr(value, ',');
                    if (first != 0)
                    {
                        *first = 0;
                        ++first;
                        HintTrimWhitespace(value);
                        entry->red = static_cast<unsigned char>(atol(value));
                        char *second = strchr(first, ',');
                        if (second != 0)
                        {
                            *second = 0;
                            HintTrimWhitespace(first);
                            entry->green = static_cast<unsigned char>(atol(first));
                            entry->blue = static_cast<unsigned char>(atol(HintTrimWhitespace(second + 1)));
                        }
                    }
                }
                else if (strcmp(key, "Alpha") == 0)
                    entry->alpha = static_cast<unsigned char>(atol(value));
                else if (strcmp(key, "End") == 0)
                    break;
            }
            // VC7.1 keeps this x87 expression live until the final float store.
            entry->textWidth = static_cast<float>(strlen(entry->text)) * entry->scale * 7.5f;
            owner->loadedStageLists[stage].Append(&entry->link);
            if (mode == 0)
            {
                HintEntryView *copy = new HintEntryView;
                memcpy(copy, entry, sizeof(*copy));
                copy->link.payload = copy;
                copy->link.next = 0;
                copy->link.previous = 0;
                owner->exportStageLists[stage].Append(&copy->link);
            }
        }
    }
    free(line);
    free(key);
    free(value);
    free(file);
    return 0;
}

#include "Main.hpp"
#include "Hint.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Target41A0A0. Shift the terminating NUL along with each leading byte.
static char *__stdcall HintTrimWhitespace(char *text)
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
static void __stdcall HintSplitLine(char *key, char *value, const char *line)
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

static inline char *ReadHintScriptLine(
    char *cursor, char *line, int *remaining, unsigned int capacity)
{
    return HintReadLine(cursor, line, remaining, capacity);
}

int __stdcall LoadHintFile(HintOwnerView *owner, const char *path, int mode)
{
    int stage = -1, entries = 0, remaining;
    char *file = reinterpret_cast<char *>(FileSystem::OpenFile(path, &remaining, 1));
    if (file == 0)
        return -1;

    char *line = static_cast<char *>(malloc(4096));
    char *key = static_cast<char *>(malloc(4096));
    char *value = static_cast<char *>(malloc(4096));
    char *cursor = file;
    while (remaining > 0)
    {
        cursor = ReadHintScriptLine(cursor, line, &remaining, 4096);
        // Both passes are present in the native outer loop.
        HintSplitLine(key, value, line);
        HintSplitLine(key, value, line);
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
            if (stage <= 0 || stage >= 8)
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
            entry->reserved18 = 0;
            if (remaining > 0)
            do
            {
                cursor = ReadHintScriptLine(cursor, line, &remaining, 4096);
                HintSplitLine(key, value, line);
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
                    entry->base = HintLookupValue(value, g_HintBaseNames, 61);
                else if (strcmp(key, "Align") == 0)
                    entry->alignment = HintLookupValue(value, g_HintAlignmentNames, 3);
                else if (strcmp(key, "Remain") == 0)
                    entry->remaining = atol(value);
                else if (strcmp(key, "Scale") == 0)
                {
                    float parsedScale = static_cast<float>(atof(value));
                    entry->scale = parsedScale;
                    if (parsedScale > 4.0f)
                        entry->scale = 4.0f;
                    else if (parsedScale < -4.0f)
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
                        char *green = first;
                        first = strchr(first, ',');
                        if (first != 0)
                        {
                            *first = 0;
                            HintTrimWhitespace(green);
                            entry->green = static_cast<unsigned char>(atol(green));
                            entry->blue = static_cast<unsigned char>(atol(HintTrimWhitespace(first + 1)));
                        }
                    }
                }
                else if (strcmp(key, "Alpha") == 0)
                    entry->alpha = static_cast<unsigned char>(atol(value));
                else if (strcmp(key, "End") == 0)
                    break;
            } while (remaining > 0);
            // VC7.1 keeps this x87 expression live until the final float store.
            entry->textWidth = static_cast<float>(strlen(entry->text)) * entry->scale * 7.5f;
            owner->loadedStageLists[stage].Append(&entry->link);
            if (mode == 0)
            {
                HintEntryView *copy = new HintEntryView;
                *copy = *entry;
                copy->link.payload = copy;
                copy->link.ClearLinks();
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

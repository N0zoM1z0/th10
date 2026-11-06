#include "PbgArchive.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"

#include <stdio.h>

// Physical data ownership for these globals is still unknown; addresses are
// recorded in the target ledger and must be target-bound for exactness.
extern PbgArchive g_MainPbgArchive;                   // original 0x00497990
extern const char g_GameStartupArchiveFilename[];   // original 0x0046E000
extern const char g_GameStartupVersionFilenameFormat[]; // 0x0046DFF0
extern const char g_GameStartupArchiveError[];      // original 0x0046DFA0
extern const char g_GameStartupVersionError[];      // original 0x0046DFC8
extern int g_GameStartupVersionSize;                // original 0x00492388
extern unsigned char *g_GameStartupVersionData;     // original 0x0049238C

// Target original TH10 JP 1.00a 0x00420100..0x004201AD (174 bytes).
// The original loader does not try a disk fallback or keep the failed data.
int GameLoadStartupVersion()
{
    char versionFile[128];
    int fileSize;
    if (g_MainPbgArchive.Load(g_GameStartupArchiveFilename))
    {
        sprintf(versionFile, g_GameStartupVersionFilenameFormat, 0x100, 'a');
        g_GameStartupVersionData = FileSystem::OpenFile(versionFile, &fileSize, 0);
        g_GameStartupVersionSize = fileSize;
        if (g_GameStartupVersionData == NULL)
        {
            th10::g_GameErrorContext.Fatal(
                const_cast<char *>(g_GameStartupVersionError));
            return -1;
        }
        return 0;
    }

    th10::g_GameErrorContext.Fatal(const_cast<char *>(g_GameStartupArchiveError));
    return -1;
}

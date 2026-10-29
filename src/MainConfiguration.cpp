#include "Main.hpp"
#include "GameConfiguration.hpp"
#include "GameErrorContext.hpp"
#include "FileSystem.hpp"
#include <stdlib.h>
#include <string.h>

extern short g_DefaultGameKeyBindings[9];
extern char g_MainConfigMissingLog[];
extern char g_MainConfigInvalidLog[];
extern char g_MainConfigOption004Log[];
extern char g_MainConfigOption001Log[];
extern char g_MainConfigWindowedLog[];
extern char g_MainConfigOption002Log[];
extern char g_MainConfigOption008Log[];
extern char g_MainConfigOption010Log[];
extern char g_MainConfigOption020Log[];
extern char g_MainConfigOption040Log[];
extern char g_MainConfigWriteError[];
extern char g_MainConfigWriteAdvice[];

// Target420870..420A85 reads the shared settings, validates and writes them back.
int __stdcall MainLoadConfiguration(const char *path, MainSupervisorView *supervisor)
{
    GameConfigurationView &configuration =
        *reinterpret_cast<GameConfigurationView *>(
            g_MainSupervisorView.serializedConfiguration);
    configuration.ResetDefaults();
    int size;
    unsigned char *data = FileSystem::OpenFile(path, &size, 1);
    if (data == NULL)
    {
        th10::g_GameErrorContext.Log(g_MainConfigMissingLog);
    }
    else
    {
        memcpy(&configuration, data, sizeof(configuration));
        free(data);
        if (static_cast<unsigned char>(configuration.colorMode) < 2 &&
            configuration.unknown01B < 3 && configuration.unknown01C < 2 &&
            configuration.windowed < 2 && configuration.frameskip < 3 &&
            configuration.unknown01F < 3 && configuration.format == 0x100003 &&
            size == sizeof(configuration))
        {
            memcpy(g_DefaultGameKeyBindings, configuration.keyBindings,
                   sizeof(configuration.keyBindings));
            goto configurationLoaded;
        }
        th10::g_GameErrorContext.Log(g_MainConfigInvalidLog);
    }
    configuration.ResetDefaults();

configurationLoaded:
    supervisor->disableVsync = 0;
    if ((supervisor->options & 4) != 0)
        th10::g_GameErrorContext.Log(g_MainConfigOption004Log);
    if ((supervisor->options & 1) != 0)
        th10::g_GameErrorContext.Log(g_MainConfigOption001Log);
    if (supervisor->windowed != 0)
        th10::g_GameErrorContext.Log(g_MainConfigWindowedLog);
    if ((supervisor->options & 2) != 0)
        th10::g_GameErrorContext.Log(g_MainConfigOption002Log);
    if ((supervisor->options & 8) != 0)
        th10::g_GameErrorContext.Log(g_MainConfigOption008Log);
    if ((supervisor->options & 0x10) != 0)
        th10::g_GameErrorContext.Log(g_MainConfigOption010Log);
    if ((supervisor->options & 0x20) != 0)
    {
        th10::g_GameErrorContext.Log(g_MainConfigOption020Log);
        g_MainSupervisorView.disableVsync = 1;
    }
    if ((supervisor->options & 0x40) != 0)
        th10::g_GameErrorContext.Log(g_MainConfigOption040Log);
    if (FileSystem::WriteFile(path, &configuration, sizeof(configuration)) != 0)
    {
        th10::g_GameErrorContext.Fatal(g_MainConfigWriteError, path);
        th10::g_GameErrorContext.Fatal(g_MainConfigWriteAdvice);
        return -1;
    }
    return 0;
}

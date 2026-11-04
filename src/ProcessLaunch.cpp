#include <windows.h>

// Complete native 0x00436570..0x00436638 (201 bytes). The original caller
// supplies its mutable command line in EDX, an unused value in ECX, then the
// application name and wait flag on the stack (RET 8). Original authorship
// and the physical entry's reachability are not yet established.
int __fastcall LaunchProcessWithOptionalWait(
    void *unused, char *commandLine, const char *applicationName, int waitForExit)
{
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    startup.cb = sizeof(startup);
    startup.lpReserved = NULL;
    startup.lpDesktop = NULL;
    startup.lpTitle = NULL;
    startup.dwX = CW_USEDEFAULT;
    startup.dwY = CW_USEDEFAULT;
    startup.dwXSize = CW_USEDEFAULT;
    startup.dwYSize = CW_USEDEFAULT;
    startup.dwXCountChars = 80;
    startup.dwYCountChars = 25;
    startup.dwFillAttribute = 0;
    startup.dwFlags = 0;
    startup.wShowWindow = SW_SHOWDEFAULT;
    startup.cbReserved2 = 0;
    startup.lpReserved2 = NULL;
    startup.hStdInput = NULL;
    startup.hStdOutput = NULL;
    startup.hStdError = NULL;

    if (!CreateProcessA(applicationName, commandLine,
                        NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
                        NULL, NULL, &startup, &process))
        return -1;
    if (waitForExit != 0)
    {
        DWORD exitCode = STILL_ACTIVE;
        do {
            GetExitCodeProcess(process.hProcess, &exitCode);
        } while (exitCode == STILL_ACTIVE);
        return static_cast<int>(exitCode);
    }
    return 0;
}

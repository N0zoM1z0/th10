#include <windows.h>
#include <string.h>

// Target 0x0044BD10..0x0044BE13 (260 bytes). This source only describes
// native behavior; the physical routine is CC-bounded without any reviewed
// inbound direct call, so historical original source ownership is unknown.
int DetectLegacyWindowsVersion()
{
    OSVERSIONINFOA version;
    memset(&version, 0, sizeof(version));
    version.dwOSVersionInfoSize = sizeof(version);
    GetVersionExA(&version);

    switch (version.dwPlatformId)
    {
    case VER_PLATFORM_WIN32s:
        return 31;
    case VER_PLATFORM_WIN32_WINDOWS:
        if (version.dwMajorVersion == 4)
            return 95 + (version.dwMinorVersion != 0 ? 3 : 0);
        return version.dwMajorVersion * 100 + version.dwMinorVersion;
    case VER_PLATFORM_WIN32_NT:
        if (version.dwMajorVersion == 5)
            return 2000;
        return (version.dwMajorVersion * 100 + version.dwMinorVersion) * 10;
    default:
        return -1;
    }
}

#include "ResFile.hpp"
#include "ArchiveOrDiskPath.hpp"

#include <stdlib.h>
#include <string.h>

CMemoryPbgFile::CMemoryPbgFile()
{
    m_Size = 0;
    m_Current = NULL;
    m_Data = NULL;
}

CMemoryPbgFile::~CMemoryPbgFile()
{
    Close();
}

bool CMemoryPbgFile::Open(const char *filename, char *mode)
{
    (void)mode;

    ArchiveOrDiskPathView *path =
        reinterpret_cast<ArchiveOrDiskPathView *>(const_cast<char *>(filename));
    m_Data = path->Read(NULL);
    m_Size = path->GetSize();
    // Intentional assignment expression: the target stores the cursor and
    // then converts the same EAX pointer value directly to bool.
    return m_Current = m_Data;
}

void CMemoryPbgFile::Close()
{
    if (m_Data != NULL)
    {
        free(m_Data);
        m_Data = NULL;
    }

    m_Data = NULL;
    m_Size = 0;
    m_Current = NULL;
}

DWORD CMemoryPbgFile::Read(void *data, DWORD dataLen)
{
    // Native Win32 cursors are low 32-bit address words: Seek can leave
    // m_Current before m_Data, so C++ array pointer subtraction would
    // be undefined even though the original uses ADD/SUB dwords.
    DWORD bytesRemaining =
        reinterpret_cast<DWORD>(m_Data) + m_Size -
        reinterpret_cast<DWORD>(m_Current);
    if (bytesRemaining >= dataLen)
    {
        memcpy(data, m_Current, dataLen);
        m_Current = reinterpret_cast<BYTE *>(reinterpret_cast<DWORD>(m_Current) + dataLen);
        return dataLen;
    }

    if (bytesRemaining != 0)
    {
        memcpy(data, m_Current, bytesRemaining);
        m_Current = reinterpret_cast<BYTE *>(reinterpret_cast<DWORD>(m_Current) + bytesRemaining);
        return bytesRemaining;
    }

    return 0;
}

bool CMemoryPbgFile::Write(void *data, DWORD dataLen)
{
    (void)data;
    (void)dataLen;
    return false;
}

DWORD CMemoryPbgFile::Tell()
{
    // Signed negative seeks still report their native wrapping offset.
    return reinterpret_cast<DWORD>(m_Current) - reinterpret_cast<DWORD>(m_Data);
}

DWORD CMemoryPbgFile::GetSize()
{
    return m_Size;
}

bool CMemoryPbgFile::Seek(DWORD offset, DWORD seekFrom)
{
    // Interpret the seek DWORD as one signed native i386 word. Copy the
    // representation instead of converting an out-of-range unsigned value
    // to LONG (which is implementation-defined by ISO C++).
    LONG signedOffset;
    memcpy(&signedOffset, &offset, sizeof signedOffset);
    unsigned char *position;
    switch (seekFrom)
    {
    case FILE_BEGIN:
        if (signedOffset < 0 || offset >= m_Size)
            return false;
        // Retail forms the FILE_BEGIN position as one low-32-bit Win32
        // address word, even if the backing allocation metadata is invalid.
        position = reinterpret_cast<BYTE *>(
            reinterpret_cast<DWORD>(m_Data) + offset);
        break;
    case FILE_CURRENT:
    {
        // The original ADD/SUB uses Win32 address words, not C++ array
        // subtraction: an earlier negative seek may leave this cursor
        // before the allocated buffer, and the signed check still applies.
        // Remaining bytes are compared as one signed native 32-bit word.
        // Preserve the original low 32-bit address arithmetic without
        // converting an out-of-range DWORD to signed LONG.
        const DWORD remainingBits = reinterpret_cast<DWORD>(m_Data) + m_Size -
            reinterpret_cast<DWORD>(m_Current);
        const LONG remaining = *reinterpret_cast<const LONG *>(&remainingBits);
        if (remaining <= signedOffset)
            return false;
        position = reinterpret_cast<BYTE *>(reinterpret_cast<DWORD>(m_Current) + offset);
        break;
    }
    case FILE_END:
    {
        if (signedOffset > 0)
            return false;
        DWORD distance = signedOffset < 0 ? 0U - offset : offset;
        if (distance >= m_Size)
            return false;
        position = reinterpret_cast<BYTE *>(reinterpret_cast<DWORD>(m_Data) + m_Size + offset);
        break;
    }
    default:
        return false;
    }
    m_Current = position;
    return true;
}

// The native derived destruction at 0x00436680 is physically separate but
// has the same complete cleanup body as the base memory-file destructor.
CWin32ResourcePbgFile::~CWin32ResourcePbgFile() {}

bool CWin32ResourcePbgFile::Open(const char *resourceName, char *mode)
{
    (void)mode;

    HRSRC resourceInfo;
    HGLOBAL resourceData;
    BYTE *lockedData;

    Close();

    resourceInfo = FindResourceA(NULL, resourceName, RT_RCDATA);
    if (resourceInfo == NULL)
        goto failure;

    resourceData = LoadResource(NULL, resourceInfo);
    if (resourceData == NULL)
        goto failure;

    lockedData = (BYTE *)LockResource(resourceData);
    if (lockedData == NULL)
    {
        FreeResource((HGLOBAL)lockedData);
        goto failure;
    }

    m_Size = SizeofResource(NULL, resourceInfo);
    m_Data = (BYTE *)malloc(m_Size);
    if (m_Data == NULL)
    {
    failure:
        Close();
        return true;
    }

    memcpy(m_Data, lockedData, m_Size);
    m_Current = m_Data;
    FreeResource(resourceData);
    return true;
}

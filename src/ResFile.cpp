#include "ResFile.hpp"

#include <stdlib.h>
#include <string.h>

// Physical target-call view for the reviewed archive-or-disk helpers at
// 0x00435800 and 0x004358E0. TH10 passes the path pointer in ECX; the data
// helper additionally consumes one stack buffer argument. The descriptive
// type does not claim an original source class or identifier.
struct ArchiveOrDiskPathView
{
    BYTE *Read(BYTE *optionalBuffer);
    DWORD GetSize();
};

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
    DWORD bytesRemaining = (DWORD)(m_Data + m_Size - m_Current);
    if (bytesRemaining >= dataLen)
    {
        memcpy(data, m_Current, dataLen);
        m_Current += dataLen;
        return dataLen;
    }

    if (bytesRemaining != 0)
    {
        memcpy(data, m_Current, bytesRemaining);
        m_Current += bytesRemaining;
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
    return (DWORD)(m_Current - m_Data);
}

DWORD CMemoryPbgFile::GetSize()
{
    return m_Size;
}

bool CMemoryPbgFile::Seek(DWORD offset, DWORD seekFrom)
{
    LONG signedOffset = (LONG)offset;

    switch (seekFrom)
    {
    case FILE_BEGIN:
        if (signedOffset < 0 || offset >= m_Size)
            return false;
        m_Current = m_Data + offset;
        return true;

    case FILE_CURRENT:
    {
        LONG bytesRemaining = (LONG)(m_Data + m_Size - m_Current);
        if (bytesRemaining <= signedOffset)
            return false;
        m_Current += signedOffset;
        return true;
    }

    case FILE_END:
    {
        if (signedOffset > 0)
            return false;

        DWORD distance = signedOffset < 0 ? 0U - offset : offset;
        if (distance >= m_Size)
            return false;

        m_Current = m_Data + m_Size + signedOffset;
        return true;
    }

    default:
        return false;
    }
}

bool CWin32ResourcePbgFile::Open(const char *resourceName, char *mode)
{
    (void)mode;

    Close();

    HRSRC resourceInfo = FindResourceA(NULL, resourceName, RT_RCDATA);
    if (resourceInfo == NULL)
    {
        Close();
        return true;
    }

    HGLOBAL resourceData = LoadResource(NULL, resourceInfo);
    if (resourceData == NULL)
    {
        Close();
        return true;
    }

    BYTE *lockedData = (BYTE *)LockResource(resourceData);
    if (lockedData == NULL)
    {
        FreeResource((HGLOBAL)lockedData);
        Close();
        return true;
    }

    m_Size = SizeofResource(NULL, resourceInfo);
    m_Data = (BYTE *)malloc(m_Size);
    if (m_Data == NULL)
    {
        Close();
        return true;
    }

    memcpy(m_Data, lockedData, m_Size);
    m_Current = m_Data;
    FreeResource(resourceData);
    return true;
}

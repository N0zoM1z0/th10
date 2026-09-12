#pragma once

#include "PbgFile.hpp"

// Descriptive reconstruction names are used here because the original TH10
// class identifiers are not established. The target source string proves that
// src/pack/ResFile.cpp existed, but it does not bind these methods to original
// identifiers or a physical translation-unit owner.
class CMemoryPbgFile : public IPbgFile
{
  public:
    CMemoryPbgFile();
    virtual ~CMemoryPbgFile();

    virtual bool Open(const char *filename, char *mode);
    virtual void Close();
    virtual DWORD Read(void *data, DWORD dataLen);
    virtual bool Write(void *data, DWORD dataLen);
    virtual DWORD Tell();
    virtual DWORD GetSize();
    virtual bool Seek(DWORD offset, DWORD seekFrom);

  protected:
    DWORD m_Size;
    BYTE *m_Current;
    BYTE *m_Data;
};

class CWin32ResourcePbgFile : public CMemoryPbgFile
{
  public:
    virtual bool Open(const char *resourceName, char *mode);
};

typedef char CMemoryPbgFileSizeIs10[(sizeof(CMemoryPbgFile) == 0x10) ? 1 : -1];
typedef char CWin32ResourcePbgFileSizeIs10[(sizeof(CWin32ResourcePbgFile) == 0x10) ? 1 : -1];

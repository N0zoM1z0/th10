#pragma once

#include <windows.h>

// Maintained target-facing declaration used by PbgArchive and PbgFile. The
// original TH10 translation-unit partition and higher pack/File utilities are
// not established yet.
class IPbgFile
{
  public:
    virtual bool Open(const char *filename, char *mode) = 0;
    virtual void Close() = 0;
    virtual DWORD Read(void *data, DWORD dataLen) = 0;
    virtual bool Write(void *data, DWORD dataLen) = 0;
    virtual DWORD Tell() = 0;
    virtual DWORD GetSize() = 0;
    virtual bool Seek(DWORD offset, DWORD seekFrom) = 0;
    virtual ~IPbgFile() {}
};

class CPbgFile : public IPbgFile
{
  public:
    CPbgFile();
    virtual ~CPbgFile();

    virtual bool Open(const char *filename, char *mode);
    virtual void Close();
    virtual DWORD Read(void *data, DWORD dataLen);
    virtual bool Write(void *data, DWORD dataLen);
    virtual DWORD Tell();
    virtual DWORD GetSize();
    virtual bool Seek(DWORD offset, DWORD seekFrom);
    virtual HGLOBAL ReadWholeFile(DWORD maxSize);

    static void GetFullFilePath(char *buffer, const char *filename);

  protected:
    HANDLE m_hFile;

  private:
    DWORD m_DesiredAccess;
};

typedef char CPbgFileSizeIs0C[(sizeof(CPbgFile) == 0x0c) ? 1 : -1];

extern char *g_PbgFileOpenModes[3];
extern int g_PbgFileSeekModes[3];

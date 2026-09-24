#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>


struct ThBgmFormat;
class CSoundManager;

// TH10's wave-file fields are independently visible in the streaming read,
// reopen, reset and CSound destruction paths. The Win32 layout below places
// flags at +0x78, the memory/file mode at +0x7C, memory cursors at +0x80/+0x84,
// remaining size at +0x88, file handle at +0x8C and format pointer at +0x90.
class CWaveFile
{
public:
    HMMIO m_hmmio;
    MMCKINFO m_ck;
    MMCKINFO m_ckRiff;
    DWORD m_dwSize;
    MMIOINFO m_mmioinfoOut;
    DWORD m_dwFlags;
    BOOL m_bIsReadingFromMemory;
    BYTE *m_pbData;
    BYTE *m_pbDataCur;
    ULONG m_ulDataSize;
    HANDLE m_hWaveFile;
    ThBgmFormat *m_pzwf;

    HRESULT Close()
    {
        if (m_dwFlags == 1)
        {
            CloseHandle(m_hWaveFile);
            m_hWaveFile = INVALID_HANDLE_VALUE;
        }
        return S_OK;
    }

    ~CWaveFile()
    {
        Close();
    }
};

// TH10 retains original source-path strings for src\core\zwave.cpp. The
// neighboring target methods recover the complete CSound prefix: virtual class
// pointer +0, buffer array +0x04, buffer size +0x08, wave file +0x0C, buffer
// count +0x10, fade state +0x14..+0x1C, Play priority/flags +0x20/+0x24,
// playing state +0x30, DSBUFFERDESC +0x34 and sound manager +0x58.
class CSound
{
protected:
    LPDIRECTSOUNDBUFFER *m_apDSBuffer;
    DWORD m_dwDSBufferSize;
    CWaveFile *m_pWaveFile;
    DWORD m_dwNumBuffers;

public:
    INT m_iCurFadeProgress;
    INT m_iTotalFade;
    INT m_iFadeType;
    DWORD m_dwPriority;
    DWORD m_dwFlags;
    DWORD unconsumedDword28;
    DWORD unconsumedDword2C;
    BOOL m_bIsPlaying;
    DSBUFFERDESC m_dsbd;
    CSoundManager *m_pSoundManager;

    virtual ~CSound();

    HRESULT Stop();
    HRESULT Pause();
    HRESULT Unpause();
    HRESULT Reset();
};

class CStreamingSound : public CSound
{
protected:
    DWORD m_dwLastPlayPos;
    DWORD m_dwPlayProgress;
    DWORD m_dwNextWriteOffset;
    BOOL m_bFillNextNotificationWithSilence;

public:
    DWORD m_dwNotifySize;
    HANDLE m_hNotifyEvent;
    BOOL m_bIsLocked;

    virtual ~CStreamingSound();
};

typedef char CWaveFileSizeIs94[(sizeof(CWaveFile) == 0x94) ? 1 : -1];
typedef char CSoundSizeIs5C[(sizeof(CSound) == 0x5c) ? 1 : -1];
typedef char CStreamingSoundSizeIs78[
    (sizeof(CStreamingSound) == 0x78) ? 1 : -1];


// TH10 0x0044D080. The target releases each DirectSound buffer, deletes the
// buffer-pointer array, inlines the owned CWaveFile close/destruction path and
// clears both owned pointers.
CSound::~CSound()
{
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
    {
        if (m_apDSBuffer[i] != NULL)
        {
            m_apDSBuffer[i]->Release();
            m_apDSBuffer[i] = NULL;
        }
    }

    if (m_apDSBuffer != NULL)
    {
        delete[] m_apDSBuffer;
        m_apDSBuffer = NULL;
    }

    if (m_pWaveFile != NULL)
    {
        delete m_pWaveFile;
        m_pWaveFile = NULL;
    }
}


// TH10 0x0044D5B0. This is the same DirectSound utility source family whose
// original path survives in the executable as ".\src\core\zwave.cpp".
HRESULT CSound::Pause()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
    m_bIsPlaying = FALSE;
    hr |= m_apDSBuffer[0]->Stop();
    return hr;
}


// TH10 0x0044D730. The retained destructor installs the derived vtable and
// tail-calls CSound::~CSound. Whether the original destructor was explicitly
// written or implicitly emitted remains origin-indeterminate.
CStreamingSound::~CStreamingSound()
{
}

#include <windows.h>
#include <dsound.h>


class CWaveFile;
class CSoundManager;

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

typedef char CSoundSizeIs5C[(sizeof(CSound) == 0x5c) ? 1 : -1];
typedef char CStreamingSoundSizeIs78[
    (sizeof(CStreamingSound) == 0x78) ? 1 : -1];


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

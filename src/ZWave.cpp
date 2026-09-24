#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdlib.h>


struct ThBgmFormat;
class CSoundManager;
extern int g_FrontEndSoundBgmVolume;

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

    HRESULT RestoreBuffer(LPDIRECTSOUNDBUFFER buffer, BOOL *wasRestored);
    LPDIRECTSOUNDBUFFER GetFreeBuffer();
    HRESULT FillBufferWithSound(
        LPDIRECTSOUNDBUFFER buffer, BOOL repeatIfBufferLarger);
    HRESULT Play(DWORD priority, DWORD flags);
    HRESULT SetVolume(int volume);
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




// TH10 0x0044D300. Restores a lost DirectSound buffer and reports whether a
// restore occurred. The retained double-Restore loop is visible in the target.
HRESULT CSound::RestoreBuffer(
    LPDIRECTSOUNDBUFFER buffer, BOOL *wasRestored)
{
    HRESULT hr;

    if (buffer == NULL)
        return CO_E_NOTINITIALIZED;
    if (wasRestored != NULL)
        *wasRestored = FALSE;

    DWORD status;
    if (FAILED(hr = buffer->GetStatus(&status)))
        return hr;

    if (status & DSBSTATUS_BUFFERLOST)
    {
        do
        {
            hr = buffer->Restore();
            if (hr == DSERR_BUFFERLOST)
                Sleep(10);
        } while (hr = buffer->Restore());

        if (wasRestored != NULL)
            *wasRestored = TRUE;
        return S_OK;
    }
    return S_FALSE;
}


// TH10 0x0044D370. Returns the first non-playing DirectSound buffer, falling
// back to a random buffer when every allocated buffer is currently playing.
LPDIRECTSOUNDBUFFER CSound::GetFreeBuffer()
{
    if (m_apDSBuffer == NULL)
        return NULL;

    DWORD i;
    for (i = 0; i < m_dwNumBuffers; ++i)
    {
        if (m_apDSBuffer[i] != NULL)
        {
            DWORD status = 0;
            m_apDSBuffer[i]->GetStatus(&status);
            if ((status & DSBSTATUS_PLAYING) == 0)
                break;
        }
    }

    if (i != m_dwNumBuffers)
        return m_apDSBuffer[i];
    return m_apDSBuffer[rand() % m_dwNumBuffers];
}


// TH10 0x0044D440. Natural DirectSound play path; target-local control flow
// establishes the helper calls, fade reset, stored priority/flags and final Play.
HRESULT CSound::Play(DWORD priority, DWORD flags)
{
    HRESULT hr;
    BOOL restored;

    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER buffer = GetFreeBuffer();
    if (buffer == NULL)
        return E_FAIL;

    if (FAILED(hr = RestoreBuffer(buffer, &restored)))
        return hr;

    if (restored)
    {
        if (FAILED(hr = FillBufferWithSound(buffer, FALSE)))
            return hr;
        Reset();
    }

    m_iFadeType = 0;
    m_iCurFadeProgress = 0;
    m_iTotalFade = 0;
    SetVolume(0);
    m_bIsPlaying = TRUE;
    m_dwPriority = priority;
    m_dwFlags = flags;
    unconsumedDword2C = 0;
    return buffer->Play(0, priority, flags);
}


// TH10 0x0044D4E0. Applies the shared BGM-volume attenuation curve before
// forwarding the adjusted millibel value to the primary DirectSound buffer.
HRESULT CSound::SetVolume(int volume)
{
    float volumeScale = (float)g_FrontEndSoundBgmVolume / 100.0f;

    if (g_FrontEndSoundBgmVolume != 0)
    {
        volumeScale = 1.0f - volumeScale;
        volumeScale *= volumeScale;
        volumeScale = 1.0f - volumeScale;
        return m_apDSBuffer[0]->SetVolume(
            (int)((volume + 5000) * volumeScale) - 5000);
    }
    return m_apDSBuffer[0]->SetVolume(DSBVOLUME_MIN);
}


// TH10 0x0044D600. Resets every DirectSound buffer to position zero.
HRESULT CSound::Reset()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
        hr |= m_apDSBuffer[i]->SetCurrentPosition(0);
    return hr;
}


// TH10 0x0044D550. Stops every allocated DirectSound buffer, rewinds each
// buffer to position zero, clears playing state and resets the fade type.
HRESULT CSound::Stop()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
    m_bIsPlaying = FALSE;
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
    {
        hr |= m_apDSBuffer[i]->Stop();
        hr |= m_apDSBuffer[i]->SetCurrentPosition(0);
    }
    m_iFadeType = 0;
    return hr;
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

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdlib.h>

#include "SoundFormat.hpp"


class CSound;
class CSoundManager
{
public:
    LPDIRECTSOUND directSound;

    HRESULT CreateStreamingFromMemory(
        CSound **sound, BYTE *data, ULONG dataSize, ThBgmFormat *format,
        DWORD creationFlags, GUID algorithm, DWORD bufferCount,
        DWORD notifySize, HANDLE notifyEvent);
};
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

    CWaveFile()
    {
        m_pzwf = NULL;
        m_hmmio = NULL;
        m_dwSize = 0;
        m_bIsReadingFromMemory = FALSE;
    }

    HRESULT OpenFromMemory(
        BYTE *data, ULONG dataSize, ThBgmFormat *format, DWORD flags)
    {
        m_pzwf = format;
        m_ulDataSize = dataSize;
        m_pbData = data;
        m_pbDataCur = data;
        m_bIsReadingFromMemory = TRUE;
        if (flags != 0)
            return E_NOTIMPL;
        return S_OK;
    }

    HRESULT Close()
    {
        if (m_dwFlags == 1)
        {
            CloseHandle(m_hWaveFile);
            m_hWaveFile = INVALID_HANDLE_VALUE;
        }
        return S_OK;
    }

    __declspec(noinline) HRESULT Reopen(ThBgmFormat *format);
    HRESULT ResetFile(bool loop);
    HRESULT Read(BYTE *buffer, DWORD bytesToRead, DWORD *bytesRead);

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

// Diagnostic-only natural entry context for the target's private Play ABI.
// The target body is reached through a whole-program call graph where the
// receiver arrives in EAX; this ordinary forwarding root lets VC7.1 expose
// that optimizer context without changing CSound's public declaration.
HRESULT ProbePlayRoot(CSound *sound, DWORD priority, DWORD flags)
{
    return sound->Play(priority, flags);
}

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

    CStreamingSound(
        LPDIRECTSOUNDBUFFER buffer, DWORD bufferSize,
        CWaveFile *waveFile, DWORD notifySize);
    virtual ~CStreamingSound();
    HRESULT InitSoundBuffers();
};

typedef char CWaveFileSizeIs94[(sizeof(CWaveFile) == 0x94) ? 1 : -1];
typedef char CSoundSizeIs5C[(sizeof(CSound) == 0x5c) ? 1 : -1];
typedef char CStreamingSoundSizeIs78[
    (sizeof(CStreamingSound) == 0x78) ? 1 : -1];

HRESULT CSoundManager::CreateStreamingFromMemory(
    CSound **sound, BYTE *data, ULONG dataSize, ThBgmFormat *format,
    DWORD creationFlags, GUID algorithm, DWORD bufferCount,
    DWORD notifySize, HANDLE notifyEvent)
{
    if (directSound == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER soundBuffer = NULL;
    LPDIRECTSOUNDNOTIFY notify = NULL;
    DSBPOSITIONNOTIFY *notifications = NULL;
    CWaveFile *waveFile = new CWaveFile();
    waveFile->OpenFromMemory(data, dataSize, format, 0);

    DWORD bufferSize = notifySize * bufferCount;
    DSBUFFERDESC description;
    ZeroMemory(&description, sizeof(description));
    description.dwSize = sizeof(description);
    description.dwFlags = creationFlags | DSBCAPS_CTRLPOSITIONNOTIFY |
        DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 |
        DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
    description.dwBufferBytes = bufferSize;
    description.guid3DAlgorithm = algorithm;
    description.lpwfxFormat = &waveFile->m_pzwf->format;

    if (FAILED(directSound->CreateSoundBuffer(
            &description, &soundBuffer, NULL)) ||
        FAILED(soundBuffer->QueryInterface(
            IID_IDirectSoundNotify, reinterpret_cast<void **>(&notify))))
        return E_FAIL;

    notifications = new DSBPOSITIONNOTIFY[bufferCount];
    if (notifications == NULL)
        return E_OUTOFMEMORY;

    for (DWORD i = 0; i < bufferCount; ++i)
    {
        notifications[i].dwOffset = notifySize * i + notifySize - 1;
        notifications[i].hEventNotify = notifyEvent;
    }

    HRESULT result = notify->SetNotificationPositions(bufferCount, notifications);
    if (FAILED(result))
    {
        if (notify != NULL)
        {
            notify->Release();
            notify = NULL;
        }
        delete[] notifications;
        return E_FAIL;
    }

    if (notify != NULL)
    {
        notify->Release();
        notify = NULL;
    }
    delete[] notifications;

    *sound = new CStreamingSound(soundBuffer, bufferSize, waveFile, notifySize);
    CopyMemory(&(*sound)->m_dsbd, &description, sizeof(description));
    (*sound)->m_pSoundManager = this;
    static_cast<CStreamingSound *>(*sound)->m_hNotifyEvent = notifyEvent;
    static_cast<CStreamingSound *>(*sound)->m_bIsLocked = FALSE;
    return S_OK;
}




// TH10 0x0044DCA0. Rebinds a disk-backed wave object to one BGM-format row.
// The target inlines the non-memory ResetFile(false) path: seek to the row's
// start offset plus the shared BGM-file base and refresh both size fields.
extern int g_SoundBgmFileBaseOffset;

HRESULT CWaveFile::Reopen(ThBgmFormat *format)
{
    if (m_bIsReadingFromMemory)
        return E_FAIL;
    if (m_hWaveFile == INVALID_HANDLE_VALUE)
        return E_FAIL;

    m_pzwf = format;
    ResetFile(false);
    m_dwSize = m_ck.cksize;
    return S_OK;
}

HRESULT CWaveFile::ResetFile(bool loop)
{
    if (m_bIsReadingFromMemory)
    {
        m_pbDataCur = m_pbData;
        if (m_pzwf->totalLength > 0)
            m_ulDataSize = m_pzwf->totalLength;
        if (loop && m_pzwf->introLength > 0)
            m_pbDataCur += m_pzwf->introLength;
    }
    else
    {
        if (m_hWaveFile == NULL)
            return CO_E_NOTINITIALIZED;

        if (loop && m_pzwf->introLength > 0)
        {
            SetFilePointer(
                m_hWaveFile,
                g_SoundBgmFileBaseOffset + m_pzwf->startOffset +
                    m_pzwf->introLength,
                NULL,
                FILE_BEGIN);
            m_ck.cksize = m_pzwf->totalLength - m_pzwf->introLength;
        }
        else
        {
            SetFilePointer(
                m_hWaveFile,
                g_SoundBgmFileBaseOffset + m_pzwf->startOffset,
                NULL,
                FILE_BEGIN);
            m_ck.cksize = m_pzwf->totalLength;
        }
    }
    return S_OK;
}

// TH10 0x0044DE10. Reads from the in-memory wave view or the backing file;
// the target's private LTCG receiver is recovered by the real FillBuffer graph.
HRESULT CWaveFile::Read(
    BYTE *buffer,
    DWORD bytesToRead,
    DWORD *bytesRead)
{
    if (m_bIsReadingFromMemory)
    {
        if (m_pbDataCur == NULL)
            return CO_E_NOTINITIALIZED;
        if (bytesRead != NULL)
            *bytesRead = 0;
        if (m_pbDataCur + bytesToRead > m_pbData + m_ulDataSize)
            bytesToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
        CopyMemory(buffer, m_pbDataCur, bytesToRead);
        m_pbDataCur += bytesToRead;
        if (bytesRead != NULL)
            *bytesRead = bytesToRead;
        return S_OK;
    }

    else
    {
        if (m_hWaveFile == NULL)
            return CO_E_NOTINITIALIZED;
        if (buffer == NULL || bytesRead == NULL)
            return E_INVALIDARG;

        UINT bytesIn = bytesToRead;
        if (bytesIn > m_ck.cksize)
            bytesIn = m_ck.cksize;
        m_ck.cksize -= bytesIn;

        DWORD size;
        ReadFile(m_hWaveFile, buffer, bytesIn, &size, NULL);
        *bytesRead = size;
        return S_OK;
    }
}

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

// TH10 0x0044D110. Fills a DirectSound buffer from the current wave stream,
// repeating or silencing a short read according to the caller's flag.
HRESULT CSound::FillBufferWithSound(
    LPDIRECTSOUNDBUFFER soundBuffer,
    BOOL repeatIfLarger)
{
    HRESULT hr;
    VOID *lockedBuffer = NULL;
    DWORD lockedSize = 0;
    DWORD waveBytesRead = 0;

    if (soundBuffer == NULL)
        return CO_E_NOTINITIALIZED;
    if (FAILED(hr = RestoreBuffer(soundBuffer, NULL)))
        return hr;
    if (FAILED(hr = soundBuffer->Lock(
            0,
            m_dwDSBufferSize,
            &lockedBuffer,
            &lockedSize,
            NULL,
            NULL,
            0)))
        return hr;

    m_pWaveFile->ResetFile(false);
    if (FAILED(hr = m_pWaveFile->Read(
            (BYTE *)lockedBuffer, lockedSize, &waveBytesRead)))
        return hr;

    if (waveBytesRead == 0)
    {
        FillMemory(
            (BYTE *)lockedBuffer,
            lockedSize,
            (BYTE)(m_pWaveFile->m_pzwf->format.wBitsPerSample == 8 ? 128 : 0));
    }
    else if (waveBytesRead < lockedSize)
    {
        if (repeatIfLarger)
        {
            DWORD readSoFar = waveBytesRead;
            while (readSoFar < lockedSize)
            {
#pragma inline_depth(0)
                if (FAILED(hr = m_pWaveFile->ResetFile(false)))
                    return hr;
#pragma inline_depth(16)
                hr = m_pWaveFile->Read(
                    (BYTE *)lockedBuffer + readSoFar,
                    lockedSize - readSoFar,
                    &waveBytesRead);
                if (FAILED(hr))
                    return hr;
                readSoFar += waveBytesRead;
            }
        }
        else
        {
            FillMemory(
                (BYTE *)lockedBuffer + waveBytesRead,
                lockedSize - waveBytesRead,
                (BYTE)(
                    m_pWaveFile->m_pzwf->format.wBitsPerSample == 8 ? 128 : 0));
        }
    }

    soundBuffer->Unlock(lockedBuffer, lockedSize, NULL, 0);
    return S_OK;
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


// TH10 0x0044D5D0. Resumes the primary buffer with the priority/flags retained
// by Play and restores the public playing-state flag.
HRESULT CSound::Unpause()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER buffer = m_apDSBuffer[0];
    m_bIsPlaying = TRUE;
    return buffer->Play(0, m_dwPriority, m_dwFlags);
}


// TH10 0x0044D730. The retained destructor installs the derived vtable and
// tail-calls CSound::~CSound. Whether the original destructor was explicitly
// written or implicitly emitted remains origin-indeterminate.
CStreamingSound::~CStreamingSound()
{
}


// Target 0x0044CF20. Recreate every buffer and its sixteen notifications.
// Early failures intentionally retain the target's partial state and leaks;
// only SetNotificationPositions failure performs the notify/array cleanup.
HRESULT CStreamingSound::InitSoundBuffers()
{
    m_bIsPlaying = FALSE;
    for (DWORD releaseIndex = 0; releaseIndex < m_dwNumBuffers; ++releaseIndex)
    {
        if (m_apDSBuffer[releaseIndex] != NULL)
        {
            m_apDSBuffer[releaseIndex]->Release();
            m_apDSBuffer[releaseIndex] = NULL;
        }
    }
    if (m_apDSBuffer != NULL)
    {
        delete[] m_apDSBuffer;
        m_apDSBuffer = NULL;
    }

    LPDIRECTSOUNDNOTIFY notify = NULL;
    m_apDSBuffer = new LPDIRECTSOUNDBUFFER[m_dwNumBuffers];
    for (DWORD bufferIndex = 0; bufferIndex < m_dwNumBuffers; ++bufferIndex)
    {
        if (FAILED(m_pSoundManager->directSound->CreateSoundBuffer(
                &m_dsbd, &m_apDSBuffer[bufferIndex], NULL)))
            return E_FAIL;
        if (FAILED(m_apDSBuffer[bufferIndex]->QueryInterface(
                IID_IDirectSoundNotify, reinterpret_cast<void **>(&notify))))
            return E_FAIL;

        DSBPOSITIONNOTIFY *notifications = new DSBPOSITIONNOTIFY[16];
        if (notifications == NULL)
            return E_OUTOFMEMORY;
        for (DWORD notifyIndex = 0; notifyIndex < 16; ++notifyIndex)
        {
            notifications[notifyIndex].dwOffset =
                m_dwNotifySize * (notifyIndex + 1) - 1;
            notifications[notifyIndex].hEventNotify = m_hNotifyEvent;
        }
        if (FAILED(notify->SetNotificationPositions(16, notifications)))
        {
            if (notify != NULL)
            {
                notify->Release();
                notify = NULL;
            }
            delete[] notifications;
            return E_FAIL;
        }
        if (notify != NULL)
        {
            notify->Release();
            notify = NULL;
        }
        delete[] notifications;
    }
    return S_OK;
}

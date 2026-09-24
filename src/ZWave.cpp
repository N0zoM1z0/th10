#include <windows.h>
#include <dsound.h>


class CWaveFile;

// TH10 retains original source-path strings for src\core\zwave.cpp.  The
// layout below is also visible directly in the neighboring Stop/Pause/Unpause/
// Reset machine bodies: the virtual class pointer occupies +0, the DirectSound
// buffer array begins at +4, the buffer count is +0x10, fade state occupies
// +0x14..+0x1c, Play priority/flags are +0x20/+0x24, and playing state is +0x30.
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

    virtual ~CSound();

    HRESULT Stop();
    HRESULT Pause();
    HRESULT Unpause();
    HRESULT Reset();
};

typedef char CSoundSizeIs34[(sizeof(CSound) == 0x34) ? 1 : -1];


// TH10 0x0044D5B0.  This is the same DirectSound utility source family whose
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

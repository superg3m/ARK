/*===========================================================
 * File: Win32_Audio.cpp
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_Audio.h"

Win32_soundOutput soundOutput;
LPDIRECTSOUNDBUFFER secondaryBuffer;

void Win32_InitDirectSound(HWND windowHandle, int32 samplesPerSecond, int32 bufferSize)
{
    HMODULE DSoundLib                      = LoadLibraryA("dsound.dll");
    direct_sound_create* directSoundCreate = (direct_sound_create*)GetProcAddress(DSoundLib, "DirectSoundCreate");
    LPDIRECTSOUND directSound              = {};
    if (directSoundCreate && SUCCEEDED(directSoundCreate(0, &directSound, 0))) {
        WAVEFORMATEX waveFormat    = {};
        waveFormat.wFormatTag      = WAVE_FORMAT_PCM;
        waveFormat.nChannels       = 2;
        waveFormat.nSamplesPerSec  = samplesPerSecond;
        waveFormat.wBitsPerSample  = 16;
        waveFormat.nBlockAlign     = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
        waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
        waveFormat.cbSize          = 0;
        if (SUCCEEDED(directSound->SetCooperativeLevel(windowHandle, DSSCL_PRIORITY))) {
            LPDIRECTSOUNDBUFFER primaryBuffer;
            DSBUFFERDESC bufferDescription = {};
            bufferDescription.dwSize       = sizeof(bufferDescription);
            bufferDescription.dwFlags      = DSBCAPS_PRIMARYBUFFER;

            if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &primaryBuffer, nullptr))) {

                if (SUCCEEDED(primaryBuffer->SetFormat(&waveFormat))) {
                    OutputDebugStringA("Primary Buffer was set!\n");
                } else {
                    OutputDebugStringA("Primary Buffer failed to set!\n");
                    // TODO(Jovanni): FAIL
                }
            } else {
                // TODO(Jovanni): FAIL
            }
        } else {
            // TODO(Jovanni): Some logs here about failing to create directSound
        }

        DSBUFFERDESC bufferDescription  = {};
        bufferDescription.dwSize        = sizeof(bufferDescription);
        bufferDescription.dwFlags       = 0;
        bufferDescription.dwBufferBytes = bufferSize;
        bufferDescription.lpwfxFormat   = &waveFormat;

        if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &secondaryBuffer, 0))) {
            OutputDebugStringA("Secondary Buffer was set!\n");
        } else {
            OutputDebugStringA("Secondary Buffer failed to set!\n");
            // TODO(Jovanni): FAIL
        }

    } else {
        // TODO(Jovanni): FAIL
    }
}

void Win32_FillSoundBuffer(Win32_soundOutput* soundOutput, DWORD bytesToLock, DWORD bytesToWrite)
{

    void* regionOne;
    DWORD regionOneSize;

    void* regionTwo;
    DWORD regionTwoSize;

    HRESULT errorCode =
        secondaryBuffer->Lock(bytesToLock, bytesToWrite, &regionOne, &regionOneSize, &regionTwo, &regionTwoSize, 0);

    if (SUCCEEDED(errorCode)) {

        // TODO(Jovanni): assert that regionOneSize/regionTwoSize is valid

        DWORD regionOneSampleCount = regionOneSize / soundOutput->bytesPerSample;
        int16* sampleOut           = (int16*)regionOne;
        for (DWORD sampleIndex = 0; sampleIndex < regionOneSampleCount; sampleIndex++) {

            float sineValue   = sinf(soundOutput->tsine);
            int16 sampleValue = (int16)(soundOutput->volume * sineValue);
            *sampleOut++      = sampleValue; // Left positive values
            *sampleOut++      = sampleValue; // Right negative values
            soundOutput->tsine += (2.0f * PI) / (float)soundOutput->WavePeriod;
            soundOutput->runningSampleIndex++;
        }

        DWORD regionTwoSampleCount = regionTwoSize / soundOutput->bytesPerSample;
        sampleOut                  = (int16*)regionTwo;
        for (DWORD sampleIndex = 0; sampleIndex < regionTwoSampleCount; sampleIndex++) {
            float sineValue   = sinf(soundOutput->tsine);
            int16 sampleValue = (int16)(soundOutput->volume * sineValue);
            *sampleOut++      = sampleValue; // Left positive values
            *sampleOut++      = sampleValue; // Right negative values
            soundOutput->tsine += (2.0f * PI) / (float)soundOutput->WavePeriod;
            soundOutput->runningSampleIndex++;
        }
        secondaryBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
    }
}

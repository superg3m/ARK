/*===========================================================
 * File: Win32_Audio.h
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma once

#include "Types.h"
#include <dsound.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

struct Win32_soundOutput {
    int samplesPerSecond;
    int hz;
    int volume;
    int runningSampleIndex;
    int WavePeriod;
    int bytesPerSample;
    int secondaryBufferSize;
    float tsine;
};

typedef HRESULT direct_sound_create(LPCGUID lpGUID, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter);

const float PI = 3.14159265359f;

/**
 * @brief Initalize the DirectSound Library
 *
 * @param windowHandle
 * @param bufferSize
 * @param samplesPerSecond
 */
void Win32_InitDirectSound(HWND windowHandle, int32 samplesPerSecond, int32 bufferSize);

/**
 * @brief fill the sound buffer
 *
 * @param soundOutput
 * @param bytesToLock
 * @param bytesToWrite
 */
void Win32_FillSoundBuffer(Win32_soundOutput* soundOutput, DWORD bytesToLock, DWORD bytesToWrite);

// NOTE(Jovanni): Implement XAudio2 and DirectSound allow user to swap between the two

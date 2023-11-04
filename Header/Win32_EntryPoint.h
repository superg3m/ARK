/*
===========================================================
 * File: Win32_EntryPoint.h
 * Date: October 29, 2023
 * Creator: Jovanni Djonaj
===========================================================
*/

#pragma once

#include <dsound.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <xinput.h>

#define internal static
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

// ===========================================================
// Win32_EntryPoint Structs
// ===========================================================
struct Win32_BitmapBuffer {
    BITMAPINFO info;
    void* memory;
    int width;
    int height;
};

struct Win32_WindowDimensions {
    int width;
    int height;
};

struct Win32_soundOutput {
    int samplesPerSecond;
    int hz;
    int volume;
    int runningSampleIndex;
    int WavePeriod;
    int bytesPerSample;
    int secondaryBufferSize;
};

// ===========================================================
// Win32_EntryPoint Constants
// ===========================================================

const float PI = 3.14159265359f;

// NOTE(Jovanni): XInput
#define X_INPUT_GET_STATE(name) DWORD name(DWORD, XINPUT_STATE*)
#define X_INPUT_SET_STATE(name) DWORD name(DWORD, XINPUT_VIBRATION*)
typedef X_INPUT_GET_STATE(x_input_get_state);
typedef X_INPUT_SET_STATE(x_input_set_state);

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

DWORD Win32_XInputGetStateStub(DWORD, XINPUT_STATE*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD Win32_XInputSetStateStub(DWORD, XINPUT_VIBRATION*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

// NOTE(Jovanni): DirectSound
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID lpGUID, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

// ===========================================================
// Win32_EntryPoint Globals
// ===========================================================

global_variable x_input_get_state* XInputGetState = Win32_XInputGetStateStub;
global_variable x_input_set_state* XInputSetState = Win32_XInputSetStateStub;

global_variable bool windowIsRunning = false;
global_variable Win32_BitmapBuffer bitBuffer;
global_variable LPDIRECTSOUNDBUFFER secondaryBuffer;

// ===========================================================
// Win32_EntryPoint Functions
// ===========================================================

/**
 * @brief get the currentWindowDimensions
 *
 * @param handle
 * @return Win32_WindowDimensions
 */
Win32_WindowDimensions Win32_GetDimensions(HWND handle)
{
    Win32_WindowDimensions ret;
    RECT clientRect = {};
    GetClientRect(handle, &clientRect);

    ret.width  = clientRect.right - clientRect.left;
    ret.height = clientRect.bottom - clientRect.top;

    return ret;
}

/**
 * @brief Load the xinput dll at runtime
 *
 * @return void
 */
internal void Win32_LoadXInput()
{
    HMODULE XInputLibrary;
    XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (XInputLibrary) {
        XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");

        XInputSetState = (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}

/**
 * @brief Initalize the DirectSound Library
 *
 * @param windowHandle
 * @param bufferSize
 * @param samplesPerSecond
 * @return internal
 */
internal void Win32_InitDirectSound(HWND windowHandle, int32 samplesPerSecond, int32 bufferSize)
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

            if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDescription, &primaryBuffer, 0))) {

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

/**
 * @brief Render the bitmap pixel by pixel.
 *
 * @param bitmapBuffer
 * @param xOffset
 * @param yOffset
 * @return void
 */
internal void Win32_RenderBitmap(const Win32_BitmapBuffer* bitmapBuffer, const int xOffset, const int yOffset)
{
    uint32* pixel = (uint32*)bitmapBuffer->memory;

    for (int y = 0; y < bitmapBuffer->height; y++) {
        for (int x = 0; x < bitmapBuffer->width; x++) {
            uint8 blue    = x + xOffset;
            uint8 green   = 0;
            uint8 red     = y + yOffset;
            uint8 padding = 0;

            *pixel++ = (blue << 0) | (green << 8) | (red << 16) | (padding << 24);
        }
    }
}

/**
 * @brief Resizes the Device-Independent Bitmap.
 * This function frees the bitmap if one already exists
 * then allocates a new one of the proper size.
 *
 * @param bitmapBuffer
 * @param width
 * @param height
 * @return internal
 */
internal void Win32_ResizeDIBSection(Win32_BitmapBuffer* bitmapBuffer, const int width, const int height)
{
    // Device-Independent Bitmap

    if (bitmapBuffer->memory) {
        VirtualFree(bitmapBuffer->memory, 0, MEM_RELEASE);
    }

    bitmapBuffer->width  = width;
    bitmapBuffer->height = height;

    bitmapBuffer->info.bmiHeader.biSize        = sizeof(bitmapBuffer->info.bmiHeader);
    bitmapBuffer->info.bmiHeader.biWidth       = bitmapBuffer->width;
    bitmapBuffer->info.bmiHeader.biHeight      = -bitmapBuffer->height;
    bitmapBuffer->info.bmiHeader.biPlanes      = 1;
    bitmapBuffer->info.bmiHeader.biBitCount    = 32;
    bitmapBuffer->info.bmiHeader.biCompression = BI_RGB;

    const uint8 bytesPerPixel = 4;

    int bitmapMemorySize = ((bitmapBuffer->width * bitmapBuffer->height) * bytesPerPixel);

    bitmapBuffer->memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

internal void Win32_DisplayBufferToWindow(const Win32_BitmapBuffer* bitmapBuffer, HDC deviceContext, int windowWidth,
                                          int windowHeight)
{

    StretchDIBits(deviceContext, 0, 0, windowWidth, windowHeight, 0, 0, bitmapBuffer->width, bitmapBuffer->height,
                  bitmapBuffer->memory, &bitmapBuffer->info, DIB_RGB_COLORS, SRCCOPY);
}

/**
 * @brief Custom Window Procedure to handle specific behavior.
 *
 * @param handle
 * @param message
 * @param wParam
 * @param lParam
 * @return LRESULT
 */
internal LRESULT Win32_WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (message) {
        case WM_SIZE: {

        } break;

        case WM_CLOSE: {
            windowIsRunning = false;
        } break;

        case WM_DESTROY:

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYUP:
        case WM_KEYDOWN: {
            uint32 VKCode = wParam;
            bool wasDown  = ((lParam & (1 << 30)) != 0);
            bool isDown   = ((lParam & (1 << 31)) == 0);

            if (VKCode == 'W') {
                // yOffset += 25;
                OutputDebugStringA("W\n");
            }
            if (VKCode == 'A') {
                // xOffset -= 25;
                OutputDebugStringA("A\n");
            } else if (VKCode == 'S') {
                // yOffset -= 25;
                OutputDebugStringA("S\n");
            } else if (VKCode == 'D') {
                // xOffset += 25;
                OutputDebugStringA("D\n");
            } else if (VKCode == 'Q') {
                OutputDebugStringA("Q\n");
            } else if (VKCode == 'E') {
                OutputDebugStringA("E\n");
            } else if (VKCode == VK_UP) {
                OutputDebugStringA("E\n");
            } else if (VKCode == VK_DOWN) {
                OutputDebugStringA("DOWN\n");
            } else if (VKCode == VK_LEFT) {
                OutputDebugStringA("LEFT\n");
            } else if (VKCode == VK_RIGHT) {
                OutputDebugStringA("RIGHT\n");
            } else if (VKCode == VK_SHIFT) {
                OutputDebugStringA("SHIFT\n");
            } else if (VKCode == VK_SPACE) {
                OutputDebugStringA("SPACE\n");
            } else if (VKCode == VK_ESCAPE) {
                OutputDebugStringA("ESCAPE\n");
                windowIsRunning = false;
            }

            bool L_AltIsPressed = (lParam & (1 << 29)) != 0;
            if (L_AltIsPressed && VKCode == VK_F4) {
                windowIsRunning = false;
            }

        } break;

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(handle, &paint);

            Win32_WindowDimensions dimension = Win32_GetDimensions(handle);

            Win32_DisplayBufferToWindow(&bitBuffer, deviceContext, dimension.width, dimension.height);

            EndPaint(handle, &paint);
        } break;

        default: {
            result = DefWindowProcA(handle, message, wParam, lParam);
        } break;
    }

    return result;
}

/**
 * @brief Registers the window class name in the WinAPI.
 *
 * @param instance
 * @param windowClassName
 * @return ATOM
 */
internal ATOM registerWindowClass(HINSTANCE instance, const char* windowClassName)
{
    WNDCLASSA windowClass     = {};
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc   = Win32_WindowProc;
    windowClass.hInstance     = instance;
    windowClass.lpszClassName = windowClassName;

    return RegisterClassA(&windowClass);
}

internal void Win32_FillSoundBuffer(Win32_soundOutput* soundOutput, DWORD bytesToLock, DWORD bytesToWrite)
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
            float time        = 2.0f * PI * (float)soundOutput->runningSampleIndex / (float)soundOutput->WavePeriod;
            float sineValue   = sinf(time);
            int16 sampleValue = (int16)(soundOutput->volume * sineValue);
            *sampleOut++      = sampleValue; // Left positive values
            *sampleOut++      = sampleValue; // Right negative values
            soundOutput->runningSampleIndex++;
        }

        DWORD regionTwoSampleCount = regionTwoSize / soundOutput->bytesPerSample;
        sampleOut                  = (int16*)regionTwo;
        for (DWORD sampleIndex = 0; sampleIndex < regionTwoSampleCount; sampleIndex++) {
            float time        = 2.0f * PI * (float)soundOutput->runningSampleIndex / (float)soundOutput->WavePeriod;
            float sineValue   = sinf(time);
            int16 sampleValue = (int16)(soundOutput->volume * sineValue);
            *sampleOut++      = sampleValue; // Left positive values
            *sampleOut++      = sampleValue; // Right negative values
            soundOutput->runningSampleIndex++;
        }
        secondaryBuffer->Unlock(regionOne, regionOneSize, regionTwo, regionTwoSize);
    } else {
        OutputDebugStringA("FAILED TO FILL SECONDARY BUFFER\n");
    }
}
/*
 * File: Win32_EntryPoint.h
 * Date: October 29, 2023
 * Creator: Jovanni Djonaj
 */

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

Win32_WindowDimensions Win32_GetDimensions(HWND handle)
{
    Win32_WindowDimensions ret;
    RECT clientRect = {};
    GetClientRect(handle, &clientRect);

    ret.width  = clientRect.right - clientRect.left;
    ret.height = clientRect.bottom - clientRect.top;

    return ret;
}

DWORD Win32_XInputGetStateStub(DWORD, XINPUT_STATE*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD Win32_XInputSetStateStub(DWORD, XINPUT_VIBRATION*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

typedef DWORD(x_input_get_state)(DWORD, XINPUT_STATE*);
typedef DWORD(x_input_set_state)(DWORD, XINPUT_VIBRATION*);

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

global_variable x_input_get_state* XInputGetState = Win32_XInputGetStateStub;
global_variable x_input_set_state* XInputSetState = Win32_XInputSetStateStub;

global_variable bool windowIsRunning;
global_variable Win32_BitmapBuffer bitBuffer;

global_variable uint8 xOffset;
global_variable uint8 yOffset;

internal void Win32_LoadXInput(void)
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
                yOffset += 25;
                OutputDebugStringA("W\n");
            } else if (VKCode == 'A') {
                xOffset -= 25;
                OutputDebugStringA("A\n");
            } else if (VKCode == 'S') {
                yOffset -= 25;
                OutputDebugStringA("S\n");
            } else if (VKCode == 'D') {
                xOffset += 25;
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

internal ATOM registerWindowClass(HINSTANCE instance, const char* windowClassName)
{
    WNDCLASSA windowClass     = {};
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc   = Win32_WindowProc;
    windowClass.hInstance     = instance;
    windowClass.lpszClassName = windowClassName;

    return RegisterClassA(&windowClass);
}
/*===========================================================
 * File: Win32_WindowClass.cpp
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_window.h"

Win32_Window* window;
Win32_BitmapBuffer bitBuffer;

extern Win32_soundOutput soundOutput;
extern LPDIRECTSOUNDBUFFER secondaryBuffer;

void Win32_RenderBitmap(const Win32_BitmapBuffer* bitmapBuffer, const int xOffset, const int yOffset)
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

void Win32_ResizeDIBSection(Win32_BitmapBuffer* bitmapBuffer, const int width, const int height)
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

void Win32_DisplayBufferToWindow(const Win32_BitmapBuffer* bitmapBuffer, HDC deviceContext, int windowWidth,
                                 int windowHeight)
{

    StretchDIBits(deviceContext, 0, 0, windowWidth, windowHeight, 0, 0, bitmapBuffer->width, bitmapBuffer->height,
                  bitmapBuffer->memory, &bitmapBuffer->info, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT Win32_WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    uint32 VKCode  = wParam;

    switch (message) {
        case WM_SIZE: {

        } break;

        case WM_CLOSE: {
            window->isRunning = false;
        } break;

        case WM_DESTROY:

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYUP: {
            if (VKCode == 'W') {
                // yOffset += 25;
                // OutputDebugStringA("W\n");
                soundOutput.hz         = 256;
                soundOutput.WavePeriod = soundOutput.samplesPerSecond / soundOutput.hz;
            }
        } break;

        case WM_KEYDOWN: {
            bool wasDown = ((lParam & (1 << 30)) != 0);
            bool isDown  = ((lParam & (1 << 31)) == 0);

            if (VKCode == 'W') {
                // yOffset += 25;
                soundOutput.hz         = 512;
                soundOutput.WavePeriod = soundOutput.samplesPerSecond / soundOutput.hz;
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
                window->isRunning = false;
            }

            bool L_AltIsPressed = (lParam & (1 << 29)) != 0;
            if (L_AltIsPressed && VKCode == VK_F4) {
                window->isRunning = false;
            }

        } break;

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(handle, &paint);

            Win32_WindowDimensions dimension = ark_window_get_dimensions(handle);

            Win32_DisplayBufferToWindow(&bitBuffer, deviceContext, dimension.width, dimension.height);

            EndPaint(handle, &paint);
        } break;

        default: {
            result = DefWindowProcA(handle, message, wParam, lParam);
        } break;
    }

    return result;
}

ATOM ark_window_register_class(HINSTANCE instance, const char* windowClassName)
{
    WNDCLASSA windowClass     = {};
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc   = Win32_WindowProc;
    windowClass.hInstance     = instance;
    windowClass.lpszClassName = windowClassName;

    return RegisterClassA(&windowClass);
}

Win32_WindowDimensions ark_window_get_dimensions(HWND handle)
{
    Win32_WindowDimensions ret;
    RECT clientRect = {};
    GetClientRect(handle, &clientRect);

    ret.width  = clientRect.right - clientRect.left;
    ret.height = clientRect.bottom - clientRect.top;

    return ret;
}

Win32_Window* ark_window_create(HINSTANCE instance, WNDPROC Win32_WindowProc, const char* windowClassName)
{

    Win32_Window* window    = (Win32_Window*)VirtualAlloc(0, sizeof(window), MEM_COMMIT, PAGE_READWRITE);
    window->isRunning       = false;
    window->windowClassName = windowClassName;

    ark_window_register_class(instance, windowClassName);

    return window;
}
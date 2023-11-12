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

extern uint8 yOffset;

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

ATOM ark_window_register_class(HINSTANCE instance,
                               LRESULT Win32_WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam),
                               const char* windowClassName)
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

    ark_window_register_class(instance, Win32_WindowProc, windowClassName);

    return window;
}
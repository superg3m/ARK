/*===========================================================
 * File: Win32_WindowClass.h
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma once

#include "Base.h"
#include "Win32_Audio.h"
#include <windows.h>

struct Win32_WindowDimensions {
    int width;
    int height;
};

struct Win32_Window {
    bool isRunning = false;
    const char* windowClassName;
};

struct Win32_BitmapBuffer {
    BITMAPINFO info;
    void* memory;
    int width;
    int height;
};

/**
 * @brief Creates a window
 * @param instance
 * @param Win32_WindowProc
 * @param windowClassName
 * @return Win32_Window
 */
Win32_Window ark_window_create(HINSTANCE instance, WNDPROC Win32_WindowProc, const char* windowClassName);

/**
 * @brief Registers the window class name in the WinAPI.
 *
 * @param instance
 * @param windowClassName
 * @return ATOM
 */
ATOM ark_window_register_class(HINSTANCE instance,
                               LRESULT Win32_WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam),
                               const char* windowClassName);

/**
 * @brief Get the Dimensions of the window
 *
 * @param handle
 * @return Win32_WindowDimensions
 */
Win32_WindowDimensions ark_window_get_dimensions(HWND handle);

/**
 * @brief Render the bitmap pixel by pixel.
 *
 * @param bitmapBuffer
 * @param xOffset
 * @param yOffset
 * @return void
 */
void Win32_RenderBitmap(const Win32_BitmapBuffer* bitmapBuffer, const int xOffset, const int yOffset);

/**
 * @brief Resizes the Device-Independent Bitmap.
 * This function frees the bitmap if one already exists
 * then allocates a new one of the proper size.
 *
 * @param bitmapBuffer
 * @param width
 * @param height
 * @return void
 */
void Win32_ResizeDIBSection(Win32_BitmapBuffer* bitmapBuffer, const int width, const int height);

/**
 * @brief Display the pixel buffer the the window
 *
 * @param bitmapBuffer
 * @param deviceContext
 * @param windowWidth
 * @param windowHeight
 */
void Win32_DisplayBufferToWindow(const Win32_BitmapBuffer* bitmapBuffer, HDC deviceContext, int windowWidth,
                                 int windowHeight);
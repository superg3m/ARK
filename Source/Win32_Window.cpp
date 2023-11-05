/*===========================================================
 * File: Win32_WindowClass.cpp
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_Window.h"

Win32_Window::Win32_Window(HINSTANCE instance, WNDPROC Win32_WindowProc, const char* windowClassName)
{
    this->Win32_WindowProc = &Win32_WindowProc;
    this->registerWindowClass(instance, windowClassName);
}

/**
 * @brief Registers the window class name in the WinAPI.
 *
 * @param instance
 * @param windowClassName
 * @return ATOM
 */
ATOM Win32_Window::registerWindowClass(HINSTANCE instance, const char* windowClassName)
{
    WNDCLASSA windowClass     = {};
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc   = *this->Win32_WindowProc;
    windowClass.hInstance     = instance;
    windowClass.lpszClassName = windowClassName;

    return RegisterClassA(&windowClass);
}

/**
 * @brief get the currentWindowDimensions
 *
 * @param handle
 * @return Win32_WindowDimensions
 */
Win32_WindowDimensions Win32_Window::getDimensions(HWND handle)
{
    Win32_WindowDimensions ret;
    RECT clientRect = {};
    GetClientRect(handle, &clientRect);

    ret.width  = clientRect.right - clientRect.left;
    ret.height = clientRect.bottom - clientRect.top;

    return ret;
}
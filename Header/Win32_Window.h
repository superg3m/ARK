/*===========================================================
 * File: Win32_WindowClass.h
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include <windows.h>

struct Win32_WindowDimensions {
    int width;
    int height;
};

class Win32_Window
{
  public:
    bool isRunning            = false;
    WNDPROC* Win32_WindowProc = nullptr;

    Win32_Window() = delete;
    Win32_Window(HINSTANCE instance, WNDPROC Win32_WindowProc, const char* windowClassName);

    ATOM registerWindowClass(HINSTANCE instance, const char* windowClassName);
    Win32_WindowDimensions getDimensions(HWND handle);

  private:
};
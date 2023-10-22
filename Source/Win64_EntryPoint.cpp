/*
 * File: Win64_EntryPoint.cpp
 * Date: October 20, 2023
 * Creator: Jovanni Djonaj
 */

#include <stdint.h>
#include <windows.h>
#include <xinput.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

struct Win64_BitmapBuffer {
    BITMAPINFO info;
    void* memory;
    int width;
    int height;
};

struct Win64_WindowDimensions {
    int width;
    int height;
};

Win64_WindowDimensions Win64_GetDimensions(HWND handle)
{
    Win64_WindowDimensions ret;
    RECT clientRect = {};
    GetClientRect(handle, &clientRect);

    ret.width  = clientRect.right - clientRect.left;
    ret.height = clientRect.bottom - clientRect.top;

    return ret;
}

#define internal static
#define local_persist static
#define global_variable static

global_variable bool windowIsRunning;
global_variable Win64_BitmapBuffer bitBuffer;

typedef DWORD(x_input_get_state)(DWORD, XINPUT_STATE*);
typedef DWORD(x_input_set_state)(DWORD, XINPUT_VIBRATION*);

DWORD Win64_XInputGetStateStub(DWORD, XINPUT_STATE*)
{
    return 0;
}
DWORD Win64_XInputSetStateStub(DWORD, XINPUT_VIBRATION*)
{
    return 0;
}

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

global_variable x_input_get_state* XInputGetState = Win64_XInputGetStateStub;
global_variable x_input_set_state* XInputSetState = Win64_XInputSetStateStub;

internal void Win64_LoadXInput(void)
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_3.dll");
    if (XInputLibrary) {
        XInputGetState =
            (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");

        XInputSetState =
            (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}

internal void Win64_RenderBitmap(const Win64_BitmapBuffer* bitmapBuffer,
                                 const int xOffset, const int yOffset)
{
    uint32* pixel = (uint32*)bitmapBuffer->memory;

    for (int y = 0; y < bitmapBuffer->height; y++) {
        for (int x = 0; x < bitmapBuffer->width; x++) {
            uint8 blue    = x + xOffset;
            uint8 green   = 0;
            uint8 red     = y + yOffset;
            uint8 padding = 0;

            *pixel++ =
                (blue << 0) | (green << 8) | (red << 16) | (padding << 24);
        }
    }
}

internal void Win64_ResizeDIBSection(Win64_BitmapBuffer* bitmapBuffer,
                                     const int width, const int height)
{
    // Device-Independent Bitmap

    if (bitmapBuffer->memory) {
        VirtualFree(bitmapBuffer->memory, 0, MEM_RELEASE);
    }

    bitmapBuffer->width  = width;
    bitmapBuffer->height = height;

    bitmapBuffer->info.bmiHeader.biSize  = sizeof(bitmapBuffer->info.bmiHeader);
    bitmapBuffer->info.bmiHeader.biWidth = bitmapBuffer->width;
    bitmapBuffer->info.bmiHeader.biHeight      = -bitmapBuffer->height;
    bitmapBuffer->info.bmiHeader.biPlanes      = 1;
    bitmapBuffer->info.bmiHeader.biBitCount    = 32;
    bitmapBuffer->info.bmiHeader.biCompression = BI_RGB;

    const uint8 bytesPerPixel = 4;

    int bitmapMemorySize =
        ((bitmapBuffer->width * bitmapBuffer->height) * bytesPerPixel);

    bitmapBuffer->memory =
        VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

internal void
Win64_DisplayBufferToWindow(const Win64_BitmapBuffer* bitmapBuffer,
                            HDC deviceContext, int windowWidth,
                            int windowHeight)
{ //

    StretchDIBits(deviceContext, 0, 0, windowWidth, windowHeight, 0, 0,
                  bitmapBuffer->width, bitmapBuffer->height,
                  bitmapBuffer->memory, &bitmapBuffer->info, DIB_RGB_COLORS,
                  SRCCOPY);
}

internal LRESULT Win64_WindowProc(HWND handle, UINT message, WPARAM wParam,
                                  LPARAM lParam)
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
            break;
        case WM_SYSKEYUP:
            break;
        case WM_KEYDOWN:
            break;
        case WM_KEYUP: {
            uint32 VKCode = wParam;
            bool wasDown  = ((lParam & (1 << 30)) != 0);
            bool isDown   = ((lParam & (1 << 31)) == 0);

            if (VKCode == 'W') {
                OutputDebugStringA("W\n");
            } else if (VKCode == 'A') {
                OutputDebugStringA("A\n");
            } else if (VKCode == 'S') {
                OutputDebugStringA("S\n");
            } else if (VKCode == 'D') {
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
                if (isDown) {
                    PostQuitMessage(0);
                }
            }

        } break;

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(handle, &paint);

            Win64_WindowDimensions dimension = Win64_GetDimensions(handle);

            Win64_DisplayBufferToWindow(&bitBuffer, deviceContext,
                                        dimension.width, dimension.height);

            EndPaint(handle, &paint);
        } break;

        default: {
            result = DefWindowProcA(handle, message, wParam, lParam);
        } break;
    }

    return result;
}

internal ATOM registerWindowClass(HINSTANCE instance,
                                  const char* windowClassName)
{
    WNDCLASSA windowClass     = {};
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc   = Win64_WindowProc;
    windowClass.hInstance     = instance;
    windowClass.lpszClassName = windowClassName;

    return RegisterClassA(&windowClass);
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance,
                     LPSTR commandLine, int windowShowCode)
{
    Win64_LoadXInput();
    Win64_ResizeDIBSection(&bitBuffer, 1200, 700);
    const char* windowClassName = "Inscription";

    registerWindowClass(instance, windowClassName);

    HWND handle =
        CreateWindowA(windowClassName, "Inscription Window",
                      WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instance, 0);

    SetFocus(handle);
    ShowWindow(handle, windowShowCode);
    windowIsRunning = true;

    uint8 xOffset = 0;
    uint8 yOffset = 0;

    while (windowIsRunning) {
        MSG message;
        while (PeekMessageA(&message, handle, 0, 0, PM_REMOVE)) {

            if (message.message == WM_QUIT) {
                windowIsRunning = false;
            }

            TranslateMessage(&message);
            DispatchMessageA(&message);
        }

        // TODO(Jovanni): Add DirectInput because its more supported
        for (int controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT;
             controllerIndex++) {
            XINPUT_STATE controllerState;

            if (XInputGetState(controllerIndex, &controllerState) ==
                ERROR_SUCCESS) {
                // NOTE(Jovanni): Controller is plugged in
                XINPUT_GAMEPAD* pad = &controllerState.Gamepad;
                WORD inputMask      = pad->wButtons;

                BOOL dPadUp    = (inputMask & XINPUT_GAMEPAD_DPAD_UP);
                BOOL dPadDown  = (inputMask & XINPUT_GAMEPAD_DPAD_DOWN);
                BOOL dPadLeft  = (inputMask & XINPUT_GAMEPAD_DPAD_LEFT);
                BOOL dPadRight = (inputMask & XINPUT_GAMEPAD_DPAD_RIGHT);

                BOOL startButton = (inputMask & XINPUT_GAMEPAD_START);
                BOOL backButton  = (inputMask & XINPUT_GAMEPAD_BACK);

                BOOL L_Stick_Press = (inputMask & XINPUT_GAMEPAD_LEFT_THUMB);
                BOOL R_Stick_Press = (inputMask & XINPUT_GAMEPAD_RIGHT_THUMB);

                BOOL L_Shoulder = (inputMask & XINPUT_GAMEPAD_LEFT_SHOULDER);
                BOOL R_Shoulder = (inputMask & XINPUT_GAMEPAD_RIGHT_SHOULDER);

                BOOL A_Button = (inputMask & XINPUT_GAMEPAD_A);
                BOOL B_Button = (inputMask & XINPUT_GAMEPAD_B);
                BOOL X_Button = (inputMask & XINPUT_GAMEPAD_X);
                BOOL Y_Button = (inputMask & XINPUT_GAMEPAD_Y);

                uint8 bumperLeftTrigger  = pad->bLeftTrigger;
                uint8 bumperRightTrigger = pad->bRightTrigger;

                int16 L_Thumb_Stick_X = pad->sThumbLX;
                int16 L_Thumb_Stick_Y = pad->sThumbLY;
                int16 R_Thumb_Stick_X = pad->sThumbRX;
                int16 R_Thumb_Stick_Y = pad->sThumbRY;

                if (X_Button) {
                    xOffset += 2;
                }
                if (A_Button) {
                    yOffset += 2;
                }

            } else {
                // NOTE(Jovanni): Controller is not available
            }
            xOffset++;
            yOffset++;
        }

        XINPUT_VIBRATION controllerVibrations;
        controllerVibrations.wLeftMotorSpeed  = 6000;
        controllerVibrations.wRightMotorSpeed = 6000;

        XInputSetState(0, &controllerVibrations);

        HDC deviceContext = GetDC(handle);

        Win64_WindowDimensions dimension = Win64_GetDimensions(handle);

        Win64_DisplayBufferToWindow(&bitBuffer, deviceContext, dimension.width,
                                    dimension.height);

        Win64_RenderBitmap(&bitBuffer, xOffset, yOffset);
        ReleaseDC(handle, deviceContext);
        // xOffset++;
        // yOffset++;
    }

    return 0;
}
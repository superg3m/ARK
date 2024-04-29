/*===========================================================
 * File: Win32_EntryPoint.cpp
 * Date: October 20, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_EntryPoint.h"

extern Win32_Window window;
extern Win32_BitmapBuffer bitBuffer;
extern Win32_soundOutput soundOutput;
extern LPDIRECTSOUNDBUFFER secondaryBuffer;

uint8 xOffset = 0;
uint8 yOffset = 0;

struct Testing {
    int a;
    int b;
    char c;
    char d;
};

#define EvalPrintStr(x) printf("%s = %s\n", #x, (char*)x);
#define EvalPrintInt(x) printf("%s = %d\n", #x, (int)x);
#define EvalPrintFloat(x) printf("%s = %f\n", #x, (float)x);
#define EvalPrintDouble(x) printf("%s = %lf\n", #x, (double)x);

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int windowShowCode)
{

    EvalPrintFloat(lerp(0.0f, 4.0f, 0.4f));
    EvalPrintFloat(fabs(-12321.2323f));
    Testing test = {4, 5, 3, 2};
    Testing copied;

    MemoryCopyStruct(&copied, &test);
    EvalPrintInt(copied.a);

    EvalPrintInt(OS_WINDOWS);
    EvalPrintInt(OS_LINUX);
    EvalPrintInt(OS_APPLE);

    int testing[45];
    EvalPrintInt(ArrayLength(testing));
    EvalPrintStr(Stringify(OS_WINDOWS));
    EvalPrintStr(Stringify(Join_String(OS_WINDOWS, TESING)));
    EvalPrintInt(Min(56756, 6));
    EvalPrintFloat(Max(56756.45f, 6.324f));
    EvalPrintDouble(Clamp(1.6, 41.34, 60.234));

    EvalPrintInt(offsetof(Testing, a));
    EvalPrintInt(offsetof(Testing, b));
    EvalPrintInt(offsetof(Testing, c));
    EvalPrintInt(offsetof(Testing, d));

    Win32_XInput xInputSystem = ark_xinput_create();

    Win32_ResizeDIBSection(&bitBuffer, 1200, 700);
    const char* windowClassName = "Inscription";

    window           = ark_window_create(instance, reinterpret_cast<WNDPROC>(Win32_WindowProc), windowClassName);
    window.isRunning = true;

    HWND windowHandle = CreateWindowA(windowClassName, "Inscription Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instance, 0);

    if (!windowHandle) {
        return -1;
    }

    soundOutput.samplesPerSecond    = 48000;
    soundOutput.hz                  = 256; // cycles per second
    soundOutput.volume              = 3000;
    soundOutput.runningSampleIndex  = 0;
    soundOutput.WavePeriod          = soundOutput.samplesPerSecond / soundOutput.hz;
    soundOutput.bytesPerSample      = sizeof(int32);
    soundOutput.secondaryBufferSize = soundOutput.samplesPerSecond * soundOutput.bytesPerSample;

    Win32_InitDirectSound(windowHandle, soundOutput.samplesPerSecond, soundOutput.secondaryBufferSize);
    Win32_FillSoundBuffer(&soundOutput, 0, soundOutput.secondaryBufferSize);
    secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

    SetFocus(windowHandle);
    ShowWindow(windowHandle, windowShowCode);

    MSG Win32_Message = {};
    while (window.isRunning) {
        while (PeekMessageA(&Win32_Message, windowHandle, 0, 0, PM_REMOVE)) {
            if (Win32_Message.message == WM_QUIT) {
                window.isRunning = false;
            }

            TranslateMessage(&Win32_Message);
            DispatchMessageA(&Win32_Message);
        }

        // TODO(Jovanni): Add DirectInput because its more supported

        for (int controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++) {
            XINPUT_STATE controllerState;

            if (xInputSystem.xinput_state_get(controllerIndex, &controllerState) == ERROR_SUCCESS) {
                // NOTE(Jovanni): Controller is plugged in
                XINPUT_GAMEPAD* pad = &controllerState.Gamepad;
                WORD inputMask      = pad->wButtons;

                bool dPadUp    = (inputMask & XINPUT_GAMEPAD_DPAD_UP);
                bool dPadDown  = (inputMask & XINPUT_GAMEPAD_DPAD_DOWN);
                bool dPadLeft  = (inputMask & XINPUT_GAMEPAD_DPAD_LEFT);
                bool dPadRight = (inputMask & XINPUT_GAMEPAD_DPAD_RIGHT);

                bool startButton = (inputMask & XINPUT_GAMEPAD_START);
                bool backButton  = (inputMask & XINPUT_GAMEPAD_BACK);

                bool L_Stick_Press = (inputMask & XINPUT_GAMEPAD_LEFT_THUMB);
                bool R_Stick_Press = (inputMask & XINPUT_GAMEPAD_RIGHT_THUMB);

                bool L_Shoulder = (inputMask & XINPUT_GAMEPAD_LEFT_SHOULDER);
                bool R_Shoulder = (inputMask & XINPUT_GAMEPAD_RIGHT_SHOULDER);

                bool A_Button = (inputMask & XINPUT_GAMEPAD_A);
                bool B_Button = (inputMask & XINPUT_GAMEPAD_B);
                bool X_Button = (inputMask & XINPUT_GAMEPAD_X);
                bool Y_Button = (inputMask & XINPUT_GAMEPAD_Y);

                uint8 bumperLeftTrigger  = pad->bLeftTrigger;
                uint8 bumperRightTrigger = pad->bRightTrigger;

                int16 L_Thumb_Stick_X = pad->sThumbLX;
                int16 L_Thumb_Stick_Y = pad->sThumbLY;
                int16 R_Thumb_Stick_X = pad->sThumbRX;
                int16 R_Thumb_Stick_Y = pad->sThumbRY;

                int16 stick_x = L_Thumb_Stick_X;
                int16 stick_y = L_Thumb_Stick_Y;

                if (X_Button) {
                    soundOutput.hz         = 512;
                    soundOutput.WavePeriod = soundOutput.samplesPerSecond / soundOutput.hz;
                    xOffset += 2;
                } else {
                    soundOutput.hz         = 256;
                    soundOutput.WavePeriod = soundOutput.samplesPerSecond / soundOutput.hz;
                }

                int integer16Max = 0xFFFF;

                int deadzone = integer16Max / 16;

                if (abs(stick_x) > deadzone) {
                    xOffset += stick_x >> 14;
                }
                if (abs(stick_y) > deadzone) {
                    yOffset += stick_y >> 14;
                }

            } else {
                // NOTE(Jovanni): Controller is not available
            }
        }

        DWORD playCursorPosition;
        DWORD writeCursorPosition;
        if (SUCCEEDED(secondaryBuffer->GetCurrentPosition(&playCursorPosition, &writeCursorPosition))) {

            DWORD bytesToLock =
                ((soundOutput.runningSampleIndex * soundOutput.bytesPerSample) % soundOutput.secondaryBufferSize);
            DWORD bytesToWrite = 0;

            if (playCursorPosition == bytesToLock) {
                bytesToWrite = 0;
            } else if (bytesToLock > playCursorPosition) {
                bytesToWrite = soundOutput.secondaryBufferSize - bytesToLock;
                bytesToWrite += playCursorPosition;
            } else {
                bytesToWrite = playCursorPosition - bytesToLock;
            }

            // LEFT RIGHT LEFT RIGHT [LEFT, RIGHT] <- one sample is a pair of LEFT and RIGHT

            Win32_FillSoundBuffer(&soundOutput, bytesToLock, bytesToWrite);
        }

        // xOffset++;
        // yOffset++;

        XINPUT_VIBRATION controllerVibrations = {};
        // controllerVibrations.wLeftMotorSpeed  = 6000;
        // controllerVibrations.wRightMotorSpeed = 6000;
        xInputSystem.xinput_state_set(0, &controllerVibrations);

        HDC deviceContext = GetDC(windowHandle);

        Win32_WindowDimensions dimension = ark_window_get_dimensions(windowHandle);

        Win32_DisplayBufferToWindow(&bitBuffer, deviceContext, dimension.width, dimension.height);

        Win32_RenderBitmap(&bitBuffer, xOffset, yOffset);

        ReleaseDC(windowHandle, deviceContext);
    }

    return 0;
}

LRESULT Win32_WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    uint32 VKCode  = wParam;

    switch (message) {
        case WM_SIZE: {

        } break;

        case WM_CLOSE: {
            window.isRunning = false;
        } break;

        case WM_DESTROY:

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYUP:
        case WM_KEYDOWN: {
            bool wasDown = ((lParam & (1 << 30)) != 0);
            bool isDown  = ((lParam & (1 << 31)) == 0);

            if (VKCode == 'W') {
                yOffset += 25;
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
                window.isRunning = false;
            }

            bool L_AltIsPressed = (lParam & (1 << 29)) != 0;
            if (L_AltIsPressed && VKCode == VK_F4) {
                window.isRunning = false;
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
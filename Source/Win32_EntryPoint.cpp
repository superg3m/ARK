/*
 * File: Win32_EntryPoint.cpp
 * Date: October 20, 2023
 * Creator: Jovanni Djonaj
 */

#include "../Header/Win32_EntryPoint.h"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int windowShowCode)
{
    Win32_LoadXInput();
    Win32_ResizeDIBSection(&bitBuffer, 1200, 700);
    const char* windowClassName = "Inscription";

    registerWindowClass(instance, windowClassName);

    HWND handle = CreateWindowA(windowClassName, "Inscription Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instance, 0);

    SetFocus(handle);
    ShowWindow(handle, windowShowCode);
    windowIsRunning = true;

    MSG Win32_Message = {};
    while (windowIsRunning) {
        while (PeekMessageA(&Win32_Message, handle, 0, 0, PM_REMOVE)) {
            if (Win32_Message.message == WM_QUIT) {
                windowIsRunning = false;
            }

            TranslateMessage(&Win32_Message);
            DispatchMessageA(&Win32_Message);
        }

        // TODO(Jovanni): Add DirectInput because its more supported
        for (int controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++) {
            XINPUT_STATE controllerState;

            if (XInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS) {
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

                int8 normalized_right_stick_x = (L_Thumb_Stick_X >> 13) | (1 << 15);
                int8 normalized_right_stick_y = (L_Thumb_Stick_Y >> 13) | (1 << 15);

                if (normalized_right_stick_x) {
                    xOffset += normalized_right_stick_x;
                }
                if (normalized_right_stick_y) {
                    yOffset += normalized_right_stick_y;
                }

            } else {
                // NOTE(Jovanni): Controller is not available
            }
        }
        // xOffset++;
        // yOffset++;

        XINPUT_VIBRATION controllerVibrations = {};
        // controllerVibrations.wLeftMotorSpeed  = 6000;
        // controllerVibrations.wRightMotorSpeed = 6000;
        XInputSetState(0, &controllerVibrations);

        HDC deviceContext = GetDC(handle);

        Win32_WindowDimensions dimension = Win32_GetDimensions(handle);

        Win32_DisplayBufferToWindow(&bitBuffer, deviceContext, dimension.width, dimension.height);

        Win32_RenderBitmap(&bitBuffer, xOffset, yOffset);
        ReleaseDC(handle, deviceContext);
        // xOffset++;
        // yOffset++;
    }

    return 0;
}
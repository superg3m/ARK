
/*===========================================================
 * File: Win32_EntryPoint.cpp
 * Date: October 20, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_EntryPoint.h"

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int windowShowCode)
{
    // TODO(Jovanni): FIX EVERYTHING ELSE!!!
    // ALSO BE AWARE Win32_Window is almsot guaranteed to be broken

    // DON'T use classes swap to structs and just seperate the functions to the right files

    Win32_XInput xInputSystem = ark_xinput_create(); // Function to initialize XinputSystem

    Win32_ResizeDIBSection(&bitBuffer, 1200, 700);
    const char* windowClassName = "Inscription";

    window            = new Win32_Window(instance, Win32_WindowProc, windowClassName);
    window->isRunning = true;

    // registerWindowClass(instance, "Inscription Window");

    HWND windowHandle = CreateWindowA(windowClassName, "Inscription Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, instance, 0);

    if (!windowHandle) {
        return -1;
    }

    uint8 xOffset = 0;
    uint8 yOffset = 0;

    Win32_soundOutput soundOutput   = {};
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
    DSBCAPS dsbCaps;
    dsbCaps.dwSize   = sizeof(DSBCAPS);
    HRESULT value    = secondaryBuffer->GetCaps(&dsbCaps);
    DWORD bufferSize = dsbCaps.dwBufferBytes;

    SetFocus(windowHandle);
    ShowWindow(windowHandle, windowShowCode);
    windowIsRunning = true;

    MSG Win32_Message = {};
    while (windowIsRunning) {
        while (PeekMessageA(&Win32_Message, windowHandle, 0, 0, PM_REMOVE)) {
            if (Win32_Message.message == WM_QUIT) {
                windowIsRunning = false;
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

                int8 stick_x = R_Thumb_Stick_X;
                int8 stick_y = R_Thumb_Stick_Y;

                soundOutput.hz         = 256 + (int)(256.0f * ((float)(stick_y / 3000.0f)));
                soundOutput.WavePeriod = soundOutput.samplesPerSecond / soundOutput.hz;

                if (stick_x) {
                    xOffset += stick_x >> 10;
                }
                if (stick_y) {
                    yOffset += stick_y >> 10;
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
        xInputSystem.xinput_state_set(0, &controllerVibrations);

        HDC deviceContext = GetDC(windowHandle);

        Win32_WindowDimensions dimension = window->getDimensions(windowHandle);

        Win32_DisplayBufferToWindow(&bitBuffer, deviceContext, dimension.width, dimension.height);

        Win32_RenderBitmap(&bitBuffer, xOffset, yOffset);

        DWORD playCursorPosition;
        DWORD writeCursorPosition;
        if (SUCCEEDED(secondaryBuffer->GetCurrentPosition(&playCursorPosition, &writeCursorPosition))) {

            DWORD bytesToLock =
                ((soundOutput.runningSampleIndex * soundOutput.bytesPerSample) % soundOutput.secondaryBufferSize);
            DWORD bytesToWrite = 0;

            if (bytesToLock > playCursorPosition) {
                bytesToWrite = soundOutput.secondaryBufferSize - bytesToLock;
                bytesToWrite += playCursorPosition;
            } else {
                bytesToWrite = playCursorPosition - bytesToLock;
            }

            // LEFT RIGHT LEFT RIGHT [LEFT, RIGHT] <- one sample is a pair of LEFT and RIGHT

            Win32_FillSoundBuffer(&soundOutput, bytesToLock, bytesToWrite);
        }

        ReleaseDC(windowHandle, deviceContext);
    }

    return 0;
}
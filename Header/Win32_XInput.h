/*===========================================================
 * File: Win32_XInput.h
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma once

#include <windows.h>
#include <xinput.h>

// Define the function pointer types
typedef DWORD Type_XInputGetState(DWORD, XINPUT_STATE*);
typedef DWORD Type_XInputSetState(DWORD, XINPUT_VIBRATION*);

class Win32_XInput
{
  public:
    Type_XInputGetState* XInput_Get_State = nullptr;
    Type_XInputSetState* XInput_Set_State = nullptr;

    Win32_XInput()
    {
        this->XInput_Get_State = Win32_XInput::XInputGetStateStub;
        this->XInput_Set_State = Win32_XInput::XInputSetStateStub;

        this->LoadXInput();
        if (this->XInput_Get_State == nullptr || this->XInput_Set_State == nullptr) {
            // TODO: Add a logging system here
            // "FAILED TO GET PROC ADDRESS OF XINPUT GET AND SET STATE"
            exit(-1);
        }
    }

    void LoadXInput();

    static DWORD XInputGetStateStub(DWORD, XINPUT_STATE*);

    static DWORD XInputSetStateStub(DWORD, XINPUT_VIBRATION*);
};
/*===========================================================
 * File: Win32_XInput.h
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma once

#include <windows.h>
#include <xinput.h>

#define internal static

// Define the function pointer types
typedef DWORD Type_XInputGetState(DWORD, XINPUT_STATE*);
typedef DWORD Type_XInputSetState(DWORD, XINPUT_VIBRATION*);

struct Win32_XInput {
    Type_XInputGetState* XInput_Get_State = nullptr;
    Type_XInputSetState* XInput_Set_State = nullptr;
};

internal DWORD XInputGetStateStub(DWORD, XINPUT_STATE*);
internal DWORD XInputSetStateStub(DWORD, XINPUT_VIBRATION*);
internal void LoadXInput(Win32_XInput* xInput);
Win32_XInput create_XInput();

// Functions here
/*===========================================================
 * File: Win32_XInput.cpp
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_XInput.h"

DWORD XInputGetStateStub(DWORD, XINPUT_STATE*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD XInputSetStateStub(DWORD, XINPUT_VIBRATION*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

/**
 * @brief Create a XInput object
 *
 * @return Win32_XInput
 */
Win32_XInput create_XInput()
{
    // clang-format off
    Win32_XInput XInput = {
        .XInput_Get_State = XInputGetStateStub,
        .XInput_Set_State = XInputSetStateStub
    };
    // clang-format on

    LoadXInput(&XInput);

    return XInput;
}

/**
 * @brief Load the xinput dll at runtime
 *
 * @return void
 */
void LoadXInput(Win32_XInput* xInput)
{
    HMODULE XInputLibrary;
    XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (XInputLibrary) {
        xInput->XInput_Get_State = (Type_XInputGetState*)GetProcAddress(XInputLibrary, "XInputGetState");
        xInput->XInput_Set_State = (Type_XInputSetState*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}

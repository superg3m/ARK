/*===========================================================
 * File: Win32_XInput.cpp
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_XInput.h"

/**
 * @brief Load the xinput dll at runtime
 *
 * @return void
 */
void Win32_XInput::LoadXInput()
{
    HMODULE XInputLibrary;
    XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (XInputLibrary) {
        this->XInput_Get_State = (Type_XInputGetState*)GetProcAddress(XInputLibrary, "XInputGetState");
        this->XInput_Set_State = (Type_XInputSetState*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}

DWORD Win32_XInput::XInputGetStateStub(DWORD, XINPUT_STATE*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD Win32_XInput::XInputSetStateStub(DWORD, XINPUT_VIBRATION*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

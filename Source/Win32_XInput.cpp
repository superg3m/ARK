/*===========================================================
 * File: Win32_XInput.cpp
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#include "../Header/Win32_XInput.h"

DWORD ark_xinput_get_state_stub(DWORD, XINPUT_STATE*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}
DWORD ark_xinput_set_state_stub(DWORD, XINPUT_VIBRATION*)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

/**
 * @brief Create a XInput object
 *
 * @return Win32_XInput
 */
Win32_XInput ark_xinput_create()
{
    // clang-format off
    Win32_XInput XInput = {
        .xinput_state_get = ark_xinput_get_state_stub,
        .xinput_state_set = ark_xinput_set_state_stub
    };
    // clang-format on

    ark_xinput_load(&XInput);

    return XInput;
}

/**
 * @brief Load the xinput dll at runtime
 *
 * @return void
 */
void ark_xinput_load(Win32_XInput* xInput)
{
    HMODULE XInputLibrary;
    XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if (!XInputLibrary) {
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }

    if (XInputLibrary) {
        xInput->xinput_state_get = (Type_XInputGetState*)GetProcAddress(XInputLibrary, "XInputGetState");
        xInput->xinput_state_set = (Type_XInputSetState*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}

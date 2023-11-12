/*===========================================================
 * File: Win32_XInput.h
 * Date: November 04, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma once

#include "Base.h"
#include <windows.h>
#include <xinput.h>

// Define the function pointer types
typedef DWORD func_XInputGetState(DWORD, XINPUT_STATE*);
typedef DWORD func_XInputSetState(DWORD, XINPUT_VIBRATION*);

// TODO(Jovanni): Remove these function pointers I think
struct Win32_XInput {
    func_XInputGetState* xinput_state_get;
    func_XInputSetState* xinput_state_set;
};

internal DWORD ark_xinput_get_state_stub(DWORD, XINPUT_STATE*);

internal DWORD ark_xinput_set_state_stub(DWORD, XINPUT_VIBRATION*);

internal void ark_xinput_load(Win32_XInput* xInput);

Win32_XInput ark_xinput_create();

// Functions here
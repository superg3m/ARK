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

// TODO(Jovanni): Remove these function pointers I think
struct Win32_XInput {
    Type_XInputGetState* xinput_state_get;
    Type_XInputSetState* xinput_state_set;
};

// NOTE(Jovanni): Template -> inscription_object_operation_data

// TODO(Jovanni): Change function name to xinput_get_state_stub(DWORD, XINPUT_STATE*)
internal DWORD ark_xinput_get_state_stub(DWORD, XINPUT_STATE*);

// TODO(Jovanni): Change function name to xinput_set_state_stub(DWORD, XINPUT_VIBRATION*)
internal DWORD ark_xinput_set_state_stub(DWORD, XINPUT_VIBRATION*);

// TODO(Jovanni): Change function name to inscription_xinput_load()
internal void ark_xinput_load(Win32_XInput* xInput);

// TODO(Jovanni): Change function name to inscription_()
Win32_XInput ark_xinput_create();

// Functions here
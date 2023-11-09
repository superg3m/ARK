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
    Type_XInputGetState* XInput_Get_State;
    Type_XInputSetState* XInput_Set_State;
};

// NOTE(Jovanni): Template -> inscription_object_operation_data

// TODO(Jovanni): Change function name to xinput_get_state_stub(DWORD, XINPUT_STATE*)
internal DWORD XInputGetStateStub(DWORD, XINPUT_STATE*);

// TODO(Jovanni): Change function name to xinput_set_state_stub(DWORD, XINPUT_VIBRATION*)
internal DWORD XInputSetStateStub(DWORD, XINPUT_VIBRATION*);

// TODO(Jovanni): Change function name to inscription_xinput_load()
internal void LoadXInput(Win32_XInput* xInput);

// TODO(Jovanni): Change function name to inscription_xinput_create()
Win32_XInput create_XInput();

// Functions here
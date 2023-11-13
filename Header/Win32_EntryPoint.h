/*===========================================================
 * File: Win32_EntryPoint.h
 * Date: October 29, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#pragma once

#include "Base.h"
#include "Win32_Audio.h"
#include "Win32_Window.h"
#include "Win32_XInput.h"
#include <stdio.h>

/**
 * @brief Custom Window Procedure to handle specific behavior.
 *
 * @param handle
 * @param message
 * @param wParam
 * @param lParam
 * @return LRESULT
 */
LRESULT Win32_WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

// Function Prototypes
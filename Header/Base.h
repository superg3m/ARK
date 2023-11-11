/*===========================================================
 * File: Base.h
 * Date: November 10, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

#ifndef BASE_H
#define BASE_H
// clang-format off

// TODO(Jovanni): Using pre-defined macros to 
// determine compiler, os, and other stuff
// Date: November 10, 2023

#if defined(_WIN32)
    #define OS_WINDOWS 1
#endif

#if defined(__linux__)
    #define OS_LINUX 1
#endif

#if defined(__APPLE__)
    #define OS_APPLE 1
#endif

#if !defined(OS_WINDOWS)
    #define OS_WINDOWS 0
#endif

#if !defined(OS_LINUX)
    #define OS_LINUX 0
#endif

#if !defined(OS_APPLE)
    #define OS_APPLE 0
#endif

// clang-format on
#endif // BASE_H
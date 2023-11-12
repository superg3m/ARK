/*===========================================================
 * File: Base.h
 * Date: November 10, 2023
 * Creator: Jovanni Djonaj
===========================================================*/

// No OS Dependencies
// No external Dependencies
// Minor Testing
// Extremely Minimal C++ mostly C

#ifndef BASE_H
#define BASE_H

#include <assert.h>

#pragma region OS Context
// clang-format off
#if defined(_WIN32)
    #define OS_WINDOWS 1
    #define OS_LINUX 0
    #define OS_APPLE 0

#elif defined(__linux__)
    #define OS_WINDOWS 0
    #define OS_LINUX 1
    #define OS_APPLE 0

#elif defined(__APPLE__)
    #define OS_WINDOWS 0
    #define OS_LINUX 0
    #define OS_APPLE 1
    
#endif
// clang-format on
#pragma endregion

#pragma region Static Defines
// clang-format off
#ifndef internal
    #define internal static
#endif

#ifndef local
    #define local static
#endif

#ifndef global
    #define global static
#endif
// clang-format on
#pragma endregion

#pragma region Typedefs
#include <stdint.h>
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
#pragma endregion

#pragma region Constants
const float PI  = 3.14159265359f;
const float TAU = 3.14159265359f * 2;
#pragma endregion

#pragma region Helper Macros
#define ArrayLength(arr) (int)(sizeof(arr) / sizeof(arr[0]))

#define Stringify_Impl(str) #str
#define Stringify(str) Stringify_Impl(str)

#define Stringify_Pre_Impl(str) str
#define Stringify_Pre(str) Stringify_Pre_Impl(#str)

#define Join_String_Impl(A, B) A##B
#define Join_String(A, B) Join_String_Impl(A, B)

#define Min(A, B) A > B ? B : A
#define Max(A, B) A > B ? A : B

#define Clamp(MIN, value, MAX) MIN > value ? MIN : MAX < value ? MAX : value

#include <string.h>

#define MemoryCopy(dest, source, size) memmove(dest, source, size);
#define MemoryCopyStruct(dest, source) MemoryCopy(dest, source, Min(sizeof(*dest), sizeof(*source)))
#define MemoryCopyArray(dest, source) MemoryCopy(dest, source, Min(sizeof(dest), sizeof(source)))

internal float lerp(float a, float b, float t);
internal double lerp(double a, double b, double t);

#pragma endregion

#endif // BASE_H
#pragma once

#include <assert.h>
#include <stdint.h>

// clang-format off
#ifndef internal
    #define internal static
#endif

#ifndef local_persist
    #define local_persist static
#endif

#ifndef global_variable
    #define global_variable static
#endif

// clang-format on

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#define ArraySize(a) sizeof(a) / sizeof(a[0])

#pragma once

#include <windows.h>


// uintptr_t for the generated leaf accessors (they cast pointers to u32 for
// the x86 custom calling convention). Defined manually so the PCH-compiled
// translation unit does not need <stdint.h>.
#ifndef _UINTPTR_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64 uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif
#define _UINTPTR_T_DEFINED
#endif

// 64-bit types for the generated leaf accessors: MSVC 13.10 (VC7.1) has no
// <stdint.h>, but it does have the __int64 extension. The magic-division
// idiom that ZUN's `x / 6` compiles to needs a 64-bit intermediate
// (`(uint64_t)magic * (uint64_t)value >> 32`), so these have to exist.
#ifndef _UINT64_T_DEFINED
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
#define _UINT64_T_DEFINED
#endif

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
typedef unsigned __int64 u64;
typedef __int64 i64;

// Ghidra renders the absolute-value idiom as ABS(). Defined here rather than
// only in the verifier's prelude: anything the prelude provides and this
// header does not is a function that verifies and then fails to build.
#ifndef ABS
#define ABS(v) ((v) < 0 ? -(v) : (v))
#endif

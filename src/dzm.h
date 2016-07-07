// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#if !defined(DZM_H)

#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef intptr_t intptr;
typedef uintptr_t uintptr;

typedef size_t memory_index;
typedef size_t mi;

typedef float real32;
typedef double real64;

typedef int8 s8;
typedef int8 s08;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;

typedef uint8 u8;
typedef uint8 u08;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef real32 r32;
typedef real64 r64;

typedef uintptr_t umm;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

#include "../dzm_ver.h"

#ifdef COMPILER_MSVC
#define TRAP() *(int *)0 = 0
#elif COMPILER_LLVM
#define TRAP() __builtin_trap()
#else
#define TRAP() volatile *(int *)0 = 0
#endif

#ifdef DZM_SLOW
#define zassert(Expression) if(!(Expression)) {TRAP();}
#else
#define zassert(Expression) 
#endif

#define InvalidCodePath zassert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break
#define Unreachable(Statement) return(Statement)

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define AlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & ~((Alignment) - 1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)

#define STRINGIFY(X) #X
#define CONCAT(X,Y) X##Y
#define SQUOTE(X, I, C) char __s_quote__I; sprintf(__s_quote__I, "'%s'", X); C = 
#define UL_ (u8 *)
#define L_  (s8 *)

#define MAX_STRING_SIZE 32768
#define MAX_VM_SIZE     4096 * 1024 * 128

static inline void
zero_size(memory_index Size, void *Ptr)
{
    uint8 *Byte = (uint8 *)Ptr;
    while(Size--)
    {
        *Byte++ = 0;
    }
}

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#ifdef DZM_ELEVATED
#undef _ELEVATED
#define _ELEVATED 1
#else
#undef _ELEVATED
#define _ELEVATED 0
#endif

// == Util
#include "dzm_utl.h"
#include "dzm_log.h"

// == Memory Manager
#include "dzm_mem.h"

// == Interpreter
#include "lang/dzm_mdl.h"
#include "lang/dzm_lex.h"
#include "lang/dzm_evl.h"
#include "lang/dzm_prt.h"
#include "lang/dzm_rep.h"

#define DZM_H
#endif
/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#ifndef LIBMCX_TYPES_H
#define LIBMCX_TYPES_H 1
#include <stdint.h>
#include <sys/types.h>

#ifdef MCXEDIT_SOURCE
#undef bool
typedef _Bool bool;

#ifdef __CHECKER__
#define __bitwise __attribute__((bitwise))
#define __force   __attribute__((force))
#else
#define __bitwise
#define __force
#endif /* __CHECKER__ */
#else
#include <stdbool.h>
#endif /* MCXEDIT_SOURCE */

typedef signed char        schar;
typedef long long          llong;
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

struct mcx;

#ifdef MCXEDIT_SOURCE
typedef __INT8_TYPE__   s8;
typedef __INT16_TYPE__  s16;
typedef __INT32_TYPE__  s32;
typedef __INT64_TYPE__  s64;
typedef __UINT8_TYPE__  u8;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

typedef u16 __bitwise be16;
typedef u32 __bitwise be32;
typedef u64 __bitwise be64;
typedef u16 __bitwise le16;
typedef u32 __bitwise le32;
typedef u64 __bitwise le64;

typedef float  f32;
typedef double f64;
#endif /* MCXEDIT_SOURCE */
#endif /* LIBMCX_TYPES_H */

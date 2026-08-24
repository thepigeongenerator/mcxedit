/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#ifndef MCXEDIT_ENDIAN_H
#define MCXEDIT_ENDIAN_H 1

#include <libmcx/atrb.h>
#include <libmcx/types.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define cvt_le16toh(x) ((FORCE u16)(le16)x)
#define cvt_le32toh(x) ((FORCE u32)(le32)x)
#define cvt_le64toh(x) ((FORCE u64)(le64)x)
#define cvt_htole16(x) ((FORCE le16)(u16)x)
#define cvt_htole32(x) ((FORCE le32)(u32)x)
#define cvt_htole64(x) ((FORCE le64)(u64)x)
#define cvt_be16toh(x) (__builtin_bswap16((FORCE u16)(be16)x))
#define cvt_be32toh(x) (__builtin_bswap32((FORCE u32)(be32)x))
#define cvt_be64toh(x) (__builtin_bswap64((FORCE u64)(be64)x))
#define cvt_htobe16(x) ((FORCE be16)__builtin_bswap16(x))
#define cvt_htobe32(x) ((FORCE be32)__builtin_bswap32(x))
#define cvt_htobe64(x) ((FORCE be64)__builtin_bswap64(x))
#define readbe16  readbswap16
#define readbe32  readbswap32
#define readbe64  readbswap64
#define readle16  read16
#define readle32  read32
#define readle64  read64
#define writebe16 writebswap16
#define writebe32 writebswap32
#define writebe64 writebswap64
#define writele16 write16
#define writele32 write32
#define writele64 write64
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define cvt_le16toh(x) (__builtin_bswap16((FORCE u16)(le16)x))
#define cvt_le32toh(x) (__builtin_bswap32((FORCE u32)(le32)x))
#define cvt_le64toh(x) (__builtin_bswap64((FORCE u64)(le64)x))
#define cvt_htole16(x) ((FORCE le16)__builtin_bswap16(x))
#define cvt_htole32(x) ((FORCE le32)__builtin_bswap32(x))
#define cvt_htole64(x) ((FORCE le64)__builtin_bswap64(x))
#define cvt_be16toh(x) ((FORCE u16)(be16)x)
#define cvt_be32toh(x) ((FORCE u32)(be32)x)
#define cvt_be64toh(x) ((FORCE u64)(be64)x)
#define cvt_htobe16(x) ((FORCE be16)(u16)x)
#define cvt_htobe32(x) ((FORCE be32)(u32)x)
#define cvt_htobe64(x) ((FORCE be64)(u64)x)
#define readbe16  read16
#define readbe32  read32
#define readbe64  read64
#define readle16  readbswap16
#define readle32  readbswap32
#define readle64  readbswap64
#define writebe16 write16
#define writebe32 write32
#define writebe64 write64
#define writele16 writebswap16
#define writele32 writebswap32
#define writele64 writebswap64
#else
#error "Machine architecture unsupported! Expected either big-endian or little-endian."
#endif

/* Reads a 16-bit integer from an unaligned buffer. */
static inline PURE u16 read16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u16 *)buf;
#else
	return (u16)buf[0] << 000 | (u16)buf[1] << 010;
#endif
}

/* Reads and bswaps a 16-bit integer from an unaligned buffer. */
static inline PURE u16 readbswap16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap16(*(u16 *)buf);
#else
	return (u16)buf[1] << 000 | (u16)buf[0] << 010;
#endif
}

/* Writes a 16-bit integer to an unaligned buffer. */
static inline void write16(u8 *buf, u16 v)
{
#if defined(MAY_UNALIGNED_ACCESS)
	*(u16 *)buf = v;
#else
	buf[0] = v >> 000; buf[1] = v >> 010;
#endif
}

/* Writes and bswaps a 16-bit integer to an unaligned buffer. */
static inline void writebswap16(u8 *buf, u16 v)
{
#if defined(MAY_UNALIGNED_ACCESS)
	*(u16 *)buf = __builtin_bswap16(*(u16 *)buf);
#else
	buf[1] = v >> 000; buf[0] = v >> 010;
#endif
}

/* Reads a 32-bit integer from an unaligned buffer. */
static inline PURE u32 read32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u32 *)buf;
#else
	return (u32)buf[0] << 000 | (u32)buf[1] << 010 |
	       (u32)buf[2] << 020 | (u32)buf[3] << 030;
#endif
}

/* Reads and bswaps a 32-bit integer from an unaligned buffer. */
static inline PURE u32 readbswap32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap32(*(be32 *)buf);
#else
	return (u32)buf[3] << 000 | (u32)buf[2] << 010 |
	       (u32)buf[1] << 020 | (u32)buf[0] << 030;
#endif
}

/* Writes a 32-bit integer to an unaligned buffer. */
static inline void write32(u8 *buf, u32 v)
{
#if defined(MAY_UNALIGNED_ACCESS)
	*(u32 *)buf = v;
#else
	buf[0] = v >> 000; buf[1] = v >> 010;
	buf[2] = v >> 020; buf[3] = v >> 030;
#endif
}

/* Writes and bswaps a 32-bit integer to an unaligned buffer. */
static inline void writebswap32(u8 *buf, u32 v)
{
#if defined(MAY_UNALIGNED_ACCESS)
	*(u32 *)buf = __builtin_bswap32(v);
#else
	buf[3] = v >> 000; buf[2] = v >> 010;
	buf[1] = v >> 020; buf[0] = v >> 030;
#endif
}

/* Reads a 64-bit integer from an unaligned buffer. */
static inline PURE u64 read64(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u64 *)buf;
#else
	return (u64)buf[0] << 000 | (u64)buf[1] << 010 |
	       (u64)buf[2] << 020 | (u64)buf[3] << 030 |
	       (u64)buf[4] << 040 | (u64)buf[5] << 050 |
	       (u64)buf[6] << 060 | (u64)buf[7] << 070;
#endif
}

/* Reads and bswaps a 64-bit integer from an unaligned buffer. */
static inline PURE u64 readbswap64(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap64(*(u64 *)buf);
#else
	return (u64)buf[7] << 000 | (u64)buf[6] << 010 |
	       (u64)buf[5] << 020 | (u64)buf[4] << 030 |
	       (u64)buf[3] << 040 | (u64)buf[2] << 050 |
	       (u64)buf[1] << 060 | (u64)buf[0] << 070;
#endif
}

/* Writes a 64-bit integer to an unaligned buffer. */
static inline void write64(u8 *buf, u64 v)
{
#if defined(MAY_UNALIGNED_ACCESS)
	*(u64 *)buf = v;
#else
	buf[0] = v >> 000; buf[1] = v >> 010;
	buf[2] = v >> 020; buf[3] = v >> 030;
	buf[4] = v >> 040; buf[5] = v >> 050;
	buf[6] = v >> 060; buf[7] = v >> 070;
#endif
}

/* Writes and bswaps a 64-bit integer to an unaligned buffer. */
static inline void writebswap64(u8 *buf, u64 v)
{
#if defined(MAY_UNALIGNED_ACCESS)
	*(u64 *)buf = __builtin_bswap64(v);
#else
	buf[7] = v >> 000; buf[6] = v >> 010;
	buf[5] = v >> 020; buf[4] = v >> 030;
	buf[3] = v >> 040; buf[2] = v >> 050;
	buf[1] = v >> 060; buf[0] = v >> 070;
#endif
}
#endif /* MCXEDIT_ENDIAN_H */

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
#define loadbe16 loadbswap16
#define loadbe32 loadbswap32
#define loadbe64 loadbswap64
#define loadle16 load16
#define loadle32 load32
#define loadle64 load64
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
#define loadbe16 load16
#define loadbe32 load32
#define loadbe64 load64
#define loadle16 loadbswap16
#define loadle32 loadbswap32
#define loadle64 loadbswap64
#else
#error "Machine architecture unsupported! Expected either big-endian or little-endian."
#endif

/* Loads a 16-bit integer from an unaligned buffer. */
static inline PURE u16 load16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u16 *)buf;
#else
	return (u16)buf[0] << 000 | (u16)buf[1] << 010;
#endif
}

/* Loads and bswaps a 16-bit integer from an unaligned buffer. */
static inline PURE u16 loadbswap16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap16(*(u16 *)buf);
#else
	return (u16)buf[1] << 000 | (u16)buf[0] << 010;
#endif
}

/* Loads a 32-bit integer from an unaligned buffer. */
static inline PURE u32 load32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u32 *)buf;
#else
	return (u32)buf[0] << 000 | (u32)buf[1] << 010 |
	       (u32)buf[2] << 020 | (u32)buf[3] << 030;
#endif
}

/* Loads and bswaps a 32-bit integer from an unaligned buffer. */
static inline PURE u32 loadbswap32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap32(*(be32 *)buf);
#else
	return (u32)buf[3] << 000 | (u32)buf[2] << 010 |
	       (u32)buf[1] << 020 | (u32)buf[0] << 030;
#endif
}

/* Loads a 64-bit integer from an unaligned buffer. */
static inline PURE u64 load64(const u8 *buf)
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

/* Loads and bswaps a 64-bit integer from an unaligned buffer. */
static inline PURE u64 loadbswap64(const u8 *buf)
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
#endif /* MCXEDIT_ENDIAN_H */

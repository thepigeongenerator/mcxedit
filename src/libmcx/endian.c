#include "endian.h"

/* Mostly copied from Linux using:
 * $ grep HAVE_EFFICIENT_UNALIGNED_ACCESS arch/.../Kconfig
 */
#if defined(__i386__) || defined(__x86_64__) || defined(__aarch64__) || defined(__riscv__)
#define MAY_UNALIGNED_ACCESS
#endif

/* Loads a 16-bit integer from an unaligned buffer. */
PURE u16 load16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u16 *)buf;
#else
	return (u16)buf[0] << 000 | (u16)buf[1] << 010;
#endif
}

/* Loads and bswaps a 16-bit integer from an unaligned buffer. */
u16 loadbswap16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap16(*(u16 *)buf);
#else
	return (u16)buf[1] << 000 | (u16)buf[0] << 010;
#endif
}

/* Loads a 32-bit integer from an unaligned buffer. */
u32 load32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return *(u32 *)buf;
#else
	return (u32)buf[0] << 000 | (u32)buf[1] << 010 |
	       (u32)buf[2] << 020 | (u32)buf[3] << 030;
#endif
}

/* Loads and bswaps a 32-bit integer from an unaligned buffer. */
u32 loadbswap32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return __builtin_bswap32(*(be32 *)buf);
#else
	return (u32)buf[3] << 000 | (u32)buf[2] << 010 |
	       (u32)buf[1] << 020 | (u32)buf[0] << 030;
#endif
}

/* Loads a 64-bit integer from an unaligned buffer. */
u64 load64(const u8 *buf)
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
u64 loadbswap64(const u8 *buf)
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


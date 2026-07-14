#include "endian.h"

/* Mostly copied from Linux using:
 * $ grep HAVE_EFFICIENT_UNALIGNED_ACCESS arch/.../Kconfig
 */
#if defined(__i386__) || defined(__x86_64__) || defined(__aarch64__) || defined(__riscv__)
#define MAY_UNALIGNED_ACCESS
#endif

u16 loadbe16(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return cvt_be16toh(*(be16 *)buf);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	return (u16)buf[0] << 000 | (u16)buf[1] << 010;
#else
	return (u16)buf[1] << 000 | (u16)buf[0] << 010;
#endif
}

u32 loadbe32(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return cvt_be32toh(*(be32 *)buf);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	return (u32)buf[0] << 000 | (u32)buf[1] << 010 |
	       (u32)buf[2] << 020 | (u32)buf[3] << 030;
#else
	       (u32)buf[3] << 000 | (u32)buf[2] << 010 |
	       (u32)buf[1] << 020 | (u32)buf[0] << 030;
#endif
}

u64 loadbe64(const u8 *buf)
{
#if defined(MAY_UNALIGNED_ACCESS)
	return cvt_be64toh(*(be64 *)buf);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	return (u64)buf[0] << 000 | (u64)buf[1] << 010 |
	       (u64)buf[2] << 020 | (u64)buf[3] << 030 |
	       (u64)buf[4] << 040 | (u64)buf[5] << 050 |
	       (u64)buf[6] << 060 | (u64)buf[7] << 070;
#else
	return (u64)buf[7] << 000 | (u64)buf[6] << 010 |
	       (u64)buf[5] << 020 | (u64)buf[4] << 030 |
	       (u64)buf[3] << 040 | (u64)buf[2] << 050 |
	       (u64)buf[1] << 060 | (u64)buf[0] << 070;
#endif
}

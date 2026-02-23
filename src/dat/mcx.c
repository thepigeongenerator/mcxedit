/* Copyright (C)2025-2026 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#include "mcx.h"

#include "../endian.h"
#include "../types.h"

usize mcx_table_calcsize(const be32 *tbl)
{
	const be32 *end = tbl + MCX_TABLE_LEN;

	/* We can get away with comparing without masking,
	 * Since the sector data is the least significant byte,
	 * and thus won't weigh into our results. */
	u32 max = 0, tmp;
	while (tbl < end) {
		tmp = cvt_be32toh(*tbl++);
		max = max < tmp ? tmp : max;
	}
	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

usize mcx_table_minsize(const be32 *tbl)
{
	const be32 *end = tbl + MCX_TABLE_LEN;

	usize sum = 0;
	while (tbl < end) {
		const u8 *m = (const u8 *)tbl;
		sum += m[3];
	}
	return sum * MCX_SECTOR + MCX_TABLES;
}

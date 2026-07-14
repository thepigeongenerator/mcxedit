/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include <libmcx/mcx.h>

#include <assert.h>
#include <libmcx/types.h>
#include <stdlib.h>
#include <string.h>

#include "endian.h"

off_t mcx_repair(struct mcx *mcx, off_t size)
{
	assert(size > MCX_TABLES);
	be32 *tbl = mcx->table;
	be32 *end = tbl + MCX_TABLE;
	u32   chpos, chlen, chend;
	u32   max = 0, tmp;
	do {
		if (!*tbl) continue; /* TODO: may be faster without? */
		tmp   = cvt_be32toh(*tbl);
		chpos = tmp >> 8;
		chlen = tmp & 0xFF;
		chend = (chpos + chlen) * MCX_SECTOR;

		if (chpos < 2 || !chlen) {
			*tbl = 0;
			continue;
		} else if (chend > (uintmax_t)size) {
			/* TODO: Add functionality that looks at the payload data,
			 * and attempts to restore from that, if possible. */
			*tbl = 0;
			continue;
		}
		max = max < tmp ? tmp : max;
	} while (++tbl < end);
	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

/* Comparison function used for qsort.
 * Returns <0 if ma is less, =0 if equal, >0 if mb is less. */
static int mcx_defrag_compar(const void *ma, const void *mb)
{
	const u32 *a = ma, *b = mb;
	return *a - *b;
}

/* Sort the table based on offset,
 * Then move the chunks down into empty space. */
off_t mcx_defrag(struct mcx *mcx, off_t size)
{
	assert(size > MCX_TABLES);
	u32   chunks[MCX_TABLE_LEN * 2];
	u32  *chunk = chunks;
	be32 *tbl   = mcx->table;
	for (int i = 0; i < MCX_TABLE_LEN; ++i) {
		*chunk++ = cvt_be32toh(tbl[i]);
		*chunk++ = i;
	}
	qsort(chunks, MCX_TABLE_LEN, sizeof(*chunks) * 2, mcx_defrag_compar);
	u32 *end = chunk;
	chunk    = chunks;

	u32   pos = 2;
	u32   chpos, chlen;
	off_t fpos, fchpos, fchlen;
	do {
		chpos  = *chunk >> 8;
		chlen  = *chunk & 0xFF;
		fpos   = pos * MCX_SECTOR;
		fchpos = chpos * MCX_SECTOR;
		fchlen = chlen * MCX_SECTOR;
		if (fchpos + fchlen > size) {
			tbl[chunk[1]] = 0;
			/* BUG: Whilst this may correctly handle out-of-bounds chunks, this
			 * will corrupt the shit out of any valid data after overlapping chunks. */
			continue;
		}
		if (chpos == pos || chpos < 2)
			goto next_table_item;

		memmove(mcx + fpos, mcx + fchpos, fchlen);
		tbl[chunk[1]] = cvt_htobe32(chlen | (pos << 8));
next_table_item:
		pos += chlen;
	} while ((chunk += 2) < end);
	if (pos == 2)
		return 0;
	return pos * MCX_SECTOR;
}

off_t mcx_calcsize(const struct mcx *mcx)
{
	const be32 *tbl = mcx->table;
	const be32 *end = tbl + MCX_TABLE_LEN;
	u32         max = 0, tmp;
	do {
		tmp = cvt_be32toh(*tbl);
		max = max < tmp ? tmp : max;
	} while (++tbl < end);
	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

off_t mcx_sumsize(const struct mcx *mcx)
{
	const u8 *ptr = (const u8 *)mcx->table + 3;
	size_t n = MCX_TABLE;

	off_t sum = 0;
	do {
		sum += *ptr;
		ptr += 4;
	} while(--n);
	return sum * MCX_SECTOR + MCX_TABLES;
}

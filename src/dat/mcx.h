/* Copyright (C)2025-2026 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#ifndef MCASELECTOR_LITE_MCX_H
#define MCASELECTOR_LITE_MCX_H

#include "../atrb.h"
#include "../types.h"

#define MCX_SECTOR     0x1000 /* Size in bytes for a sector. */
#define MCX_TABLE      0x1000 /* Size in bytes for one table. */
#define MCX_TABLES     0x2000 /* Size in bytes for the tables. */
#define MCX_TABLE_LEN  0x400  /* Amount of elements within one table. */
#define MCX_TABLES_LEN 0x800  /* Amount of elements within the tables. */

/* Computes the size in bytes of the entire .mcX file,
 * according to the table.
 * It is assumed that mcx points to a valid address
 * with at least 8KiB (two sectors) allocated space.
 * NOTE: This value may not be reflective of the actual value,
 * Minecraft leaves unallocated sectors at the end of the file. If this value
 * is higher than the actual file size, then corruption has taken place. */
usize mcx_table_calcsize(const void *mcx);

/* Computes the minimum size in bytes required for the entire .mcX file,
 * according to the table.
 * It is assumed that mcx points to a valid address
 * with at least 8KiB (two sectors) allocated space. */
usize mcx_table_minsize(const void *mcx);

#endif /* MCASELECTOR_LITE_MCX_H */

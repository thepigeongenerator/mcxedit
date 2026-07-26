/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include "libmcx/atrb.h"
#ifndef MCXEDIT_NBT_H
#define MCXEDIT_NBT_H 1
#include <libmcx/types.h>
#include <stdint.h>
#include <sys/types.h>

#define NBT_NEST_MAX 512

/* https://minecraft.wiki/w/NBT_format#Binary_format */
enum nbt_tagid {
	NBT_END      = 0x00,
	NBT_S8       = 0x01,
	NBT_S16      = 0x02,
	NBT_S32      = 0x03,
	NBT_S64      = 0x04,
	NBT_F32      = 0x05,
	NBT_F64      = 0x06,
	NBT_ARR_S8   = 0x07,
	NBT_STR      = 0x08,
	NBT_LIST     = 0x09,
	NBT_COMPOUND = 0x0A,
	NBT_ARR_S32  = 0x0B,
	NBT_ARR_S64  = 0x0C,
};

/* Returns the tag length.
 * "tag"  has to be a fully-fledged tag. It cannot reside within a list.
 * "root" must be <NBT_NEST_MAX, and will be the current depth.
 * "tagcache" points to a list of tags, where everything >root may be written.
 * <root won't be touched. Same goes for "lencache".
 * Returns the size in bytes of the current tag, or <0 for an error.
 * Negating this error value will yield a valid value for nbt_errstr. */
ssize_t nbt_taglen(const mcx_u8 *restrict tag, int root,
	u8 *restrict tagcache, s32 *lencache);

/* Compares the name of the fully-fledged tag
 * against a NUL-terminated string.
 * Returns 0 if both are equal. */
int nbt_tagnamecmp(const mcx_u8 *tag, const char *str);

/* Destructively formats a dot-seperated path to be NUL-seperated.
 * Returns the pointer to the next node,
 * or NULL if the end of the path was reached. */
char *nbt_popnode(char *path);

#define ENBT_TAG   1
#define ENBT_IND   2
#define ENBT_DEPTH 3
const char *nbt_errstr(int code) CONST COLD;

#endif /* MCXEDIT_NBT_H */

/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#ifndef LIBMCX_NBT_H
#define LIBMCX_NBT_H 1
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

/* Cache that is used for the recursive operations on NBT trees. */
struct nbt_cache {
	mcx_u8  tags[NBT_NEST_MAX];
	mcx_s32 lens[NBT_NEST_MAX];
};

/* Returns the tag length.
 * "tag"    has to be a fully-fledged tag. It cannot reside within a list.
 * "maxlen" is the upper bound from which "tag" is known to be valid.
 * "root"   must be <NBT_NEST_MAX, and will be the current depth.
 * "cache"  points to the cache arrays, where everything >root may be written.
 *          <root won't be touched.
 * Returns the size in bytes of the current tag, or <0 for the error value as
 * "-error". */
ssize_t nbt_taglen(const mcx_u8 *restrict tag, size_t maxlen, int root,
	struct nbt_cache *restrict cache);

/* Writes an end tag at "buf".
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_end(u8 *buf, u8 *max);

/* Writes an integer tag at "buf".
 * "name" is a NUL-terminated string, if NULL no string nor tag is written.
 * "id" sets the kind of integer.
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_int(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, u64 val);

/* Writes an floating-point tag at "buf".
 * "name" is a NUL-terminated string, if NULL no string nor tag is written.
 * "id" sets the kind of floating-point tag.
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_float(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, f64 val);

/* Writes an array tag at "buf".
 * "name" is a NUL-terminated string, if NULL no string nor tag is written.
 * "id"   sets the kind of array tag.
 * "dat"  contains the payload.
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_arr(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, s32 len,
	const void *restrict dat);

/* Writes a string tag at "buf".
 * "name" is a NUL-terminated string, if NULL no string nor tag is written.
 * "str" is the actual string payload.
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_str(u8 *buf, u8 *max,
	const char *restrict name, const char *restrict str);

/* Writes a list tag at "buf".
 * "name" is a NUL-terminated string, if NULL no string nor tag is written.
 * "id"  sets the list kind.
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_list(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, s32 len);

/* Writes a compound tag at "buf".
 * "name" is a NUL-terminated string, if NULL no string nor tag is written.
 * Returns the offset in bytes for the new "head" of "buf",
 * and <0 for the error value is -error. */
ssize_t nbt_addkey_compound(u8 *buf, u8 *max,
	const char *restrict name);

/* Compares the name of the fully-fledged tag
 * against a NUL-terminated string.
 * Returns 0 if both are equal. */
int nbt_tagnamecmp(const mcx_u8 *tag, const char *str);

/* Destructively formats a dot-seperated path to be NUL-seperated.
 * Returns the pointer to the next node,
 * or NULL if the end of the path was reached. */
char *nbt_popnode(char *path);

#endif /* LIBMCX_NBT_H */

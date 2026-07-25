/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include <libmcx/nbt.h>

#include "endian.h"
#include <assert.h>
#include <errno.h>
#include <libmcx/types.h>
#include <stddef.h>
#include <sys/types.h>

static ssize_t nbt_primitive_size(u8 id)
{
	if (id == NBT_END) return 0;
	if (id <= NBT_S64) return 1 << --id;
	if (id <= NBT_F64) return 1 << (id - 3);
	return -1;
}

ssize_t nbt_taglen(const u8 *restrict tag, int root,
	u8 *restrict tagcache, s32 *lencache)
{
	assert(root < NBT_NEST_MAX);
	tagcache += root;
	int depth = root;
	u8 id;
	const u8 *tmp = tag;
	do {
		/* Because it is a recursive algorithm,
		 * we must keep track of the tags in a cache. */
		if (!depth || tagcache[-1] != NBT_LIST) {
			id = *tmp++;

			if (id == NBT_END)
				goto depth_decrease;

			/* Skip the tag name. */
			tmp += loadbe16(tmp) + 2;

			ssize_t size = nbt_primitive_size(id);
			if (size >= 0) {
				tmp += size;
				continue;
			}
		} else {
			/* Lists are a bitch to parse. */
			if (lencache[-1]-- == 0)
				goto depth_decrease;
			id = *tagcache;
		}

		if (id == NBT_ARR_S8) {
			s32 n = loadbe32(tmp);
			if (n < 0) return -ENBT_IND;
			tmp += n + 4;
			continue;
		}

		if (id == NBT_STR) {
			tmp += loadbe16(tmp) + 2;
			continue;
		}

		if (id == NBT_LIST) {
			*tagcache = id;
			/* WARN: May want to increment and check here.
			 * Since we'd skip the limit if it's a primitive.
			 * Then again, it wouldn't cause much issue. */
			id = *tmp++;
			s32 n = loadbe32(tmp);
			if (n < 0) return -ENBT_IND;
			tmp += 4;

			/* NOTE: TAG_END is allowed, but has a size of 0. */
			ssize_t size = nbt_primitive_size(id);
			if (size >= 0) {
				size *= n;
				tmp  += size;
				continue;
			}
			*lencache = n;
			goto depth_increase;
		}
		if (id == NBT_COMPOUND)
			goto depth_increase;

		if (id <= NBT_ARR_S64) {
			size_t size = (id == NBT_ARR_S32) ? 4 : 8;
			s32 n = loadbe32(tmp);
			if (n < 0) return -ENBT_IND;
			tmp += size * n + 5;
			continue;
		}
		return -ENBT_TAG;
depth_increase:
		depth++;
		tagcache++;
		lencache++;
		if (depth >= NBT_NEST_MAX)
			return -ENBT_DEPTH;
		/* This is safe for compound tags, even though it isn't used.
		 * It mainly simplifies the code branches. */
		*tagcache = id;
		continue;
depth_decrease:
		if (depth == root) break;
		depth--;
		tagcache--;
		lencache--;
	} while (depth != root);
	return tmp - tag;
}

int nbt_tagnamecmp(const u8 *tag, const char *str)
{
	u16 n = loadbe16(++tag);
	tag += 2;
	int v;

	if (!n) return !!*str;
	while (n-- && *str && !(v = *str++ - *tag++));
	return v;
}

char *nbt_popnode(char *path)
{
	char c;
	do c = *path++;
	while (c && c != '.');
	path[-1] = '\0';
	return c ? path : NULL;
}

static const char *errors[ENBT_DEPTH+1] = {
	[0]          = "Success",
	[ENBT_TAG]   = "Invalid tag",
	[ENBT_IND]   = "Invalid index",
	[ENBT_DEPTH] = "Too many nested lists and compound tags",
};
const char *nbt_errstr(int code)
{
	if (code < 0 || code > ENBT_DEPTH)
		return NULL;
	return errors[code];
}

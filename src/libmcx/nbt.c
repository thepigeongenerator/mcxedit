/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2025 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include <libmcx/nbt.h>

#include "endian.h"
#include <assert.h>
#include <errno.h>
#include <libmcx/err.h>
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

ssize_t nbt_taglen(const u8 *restrict tag, size_t maxlen, int root,
	struct nbt_cache *restrict cache)
{
	if (0 > root || root >= NBT_NEST_MAX)
		return -MCX_EINVAL;
	if ((ssize_t)maxlen < 0)
		return -MCX_EINVAL;

	int depth = root;
	u8 id;
	const u8 *tmp = tag;
	const u8 *max = tag + maxlen;
	do {
		/* Because it is a recursive algorithm,
		 * we must keep track of the tags in a cache. */
		if (!depth || cache->tags[depth-1] != NBT_LIST) {
			id = *tmp++;
			if (tmp >= max)
				return -MCX_EFAULT;

			if (id == NBT_END)
				goto depth_decrease;

			/* Skip the tag name. */
			tmp += loadbe16(tmp) + 2;
			if (tmp >= max)
				return -MCX_EFAULT;

			ssize_t size = nbt_primitive_size(id);
			if (size >= 0) {
				tmp += size;
				goto check_and_continue;
			}
		} else {
			/* Lists are a bitch to parse. */
			if (cache->lens[depth-1]-- == 0)
				goto depth_decrease;
			id = cache->tags[depth];
		}

		s32 n;
		switch (id) {
		case NBT_ARR_S8:
			n = loadbe32(tmp);
			if (n < 0) return -MCX_ERANGE;
			tmp += n + 4;
			goto check_and_continue;
		case NBT_STR:
			tmp += loadbe16(tmp) + 2;
			goto check_and_continue;
		case NBT_LIST:
			cache->tags[depth] = id;
			/* WARN: May want to increment and check here.
			 * Since we'd skip the limit if it's a primitive.
			 * Then again, it wouldn't cause much issue. */
			id = *tmp++;
			n = loadbe32(tmp);
			if (n < 0) return -MCX_ERANGE;
			tmp += 4;

			/* NOTE: TAG_END is allowed, but has a size of 0. */
			ssize_t size = nbt_primitive_size(id);
			if (size >= 0) {
				size *= n;
				tmp  += size;
				goto check_and_continue;
			}
			cache->lens[depth] = n;
			if (tmp >= max) return -MCX_EFAULT;
			goto depth_increase;
		case NBT_COMPOUND:
			goto depth_increase;
		}

		if (id <= NBT_ARR_S64) {
			size_t size = (id == NBT_ARR_S32) ? 4 : 8;
			s32 n = loadbe32(tmp);
			if (n < 0) return -MCX_ERANGE;
			tmp += size * n + 5;
			goto check_and_continue;
		}
		return -MCX_ETAG;
depth_increase:
		depth++;
		if (depth >= NBT_NEST_MAX)
			return -MCX_EITER;
		/* This is safe for compound tags, even though it isn't used.
		 * It mainly simplifies the code branches. */
		cache->tags[depth] = id;
		continue;
depth_decrease:
		if (depth == root) break;
		depth--;
		continue;
check_and_continue:
		if (tmp < max) continue;
		return -MCX_EFAULT;
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

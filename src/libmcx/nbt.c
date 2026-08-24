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
#include <string.h>
#include <sys/types.h>

#define PRIMITIVES 7

static const u8 nbt_primitive_size[] = {
	[NBT_END]      = 0,
	[NBT_S8]       = 1,
	[NBT_S16]      = 2,
	[NBT_S32]      = 4,
	[NBT_S64]      = 8,
	[NBT_F32]      = 4,
	[NBT_F64]      = 8,
	[NBT_ARR_S8]   = 1,
	[NBT_STR]      = 1,
	[NBT_LIST]     = -1,
	[NBT_COMPOUND] = -1,
	[NBT_ARR_S32]  = 4,
	[NBT_ARR_S64]  = 8,
};

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
			tmp += readbe16(tmp) + 2;
			if (tmp >= max)
				return -MCX_EFAULT;

			if (id < PRIMITIVES) {
				tmp += nbt_primitive_size[id];
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
			n = readbe32(tmp);
			if (n < 0) return -MCX_ERANGE;
			tmp += n + 4;
			goto check_and_continue;
		case NBT_STR:
			tmp += readbe16(tmp) + 2;
			goto check_and_continue;
		case NBT_LIST:
			/* WARN: Setting ID to current depth? */
			cache->tags[depth] = id;
			/* WARN: May want to increment and check here.
			 * Since we'd skip the limit if it's a primitive.
			 * Then again, it wouldn't cause much issue. */
			id = *tmp++;
			n = readbe32(tmp);
			if (n < 0) return -MCX_ERANGE;
			tmp += 4;

			/* NOTE: TAG_END is allowed, but has a size of 0. */
			if (id < PRIMITIVES) {
				tmp += nbt_primitive_size[id] * n;
				goto check_and_continue;
			}
			cache->lens[depth] = n;
			if (tmp >= max) return -MCX_EFAULT;
			goto depth_increase;
		case NBT_COMPOUND:
			goto depth_increase;
		}

		if (id <= NBT_ARR_S64) {
			size_t size = nbt_primitive_size[id];
			s32 n = readbe32(tmp);
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
		/* WARN: If taglen is correct,
		 * but means moving to the end of the buffer,
		 * will this result in EFAULT? */
		if (tmp < max) continue;
		return -MCX_EFAULT;
	} while (depth != root);
	return tmp - tag;
}

static ssize_t nbt_copy_str(u8 *buf, u8 *max,
	const char *restrict str)
{
	u8 *head = buf+2;
	size_t n = head - buf;
	while (*str) {
		if (head >= max)
			return -MCX_EFAULT;
		if (n > 0xFFFF)
			return -MCX_EBIG;
		*head++ = *str++;
		n = head - buf;
	}
	writebe16(buf, n);
	return n;
}

ssize_t nbt_addkey_end(u8 *buf, u8 *max)
{
	if ((ssize_t)(max - buf) < 0)
		return -MCX_EINVAL;
	u8 *head = buf;
	*head++ = NBT_END;
	return 1;
}

ssize_t nbt_addkey_int(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, u64 val)
{
	if ((ssize_t)(max - buf) < 0)
		return -MCX_EINVAL;
	u8 *head = buf;

	if (name) {
		*head = id;
		head += 3;
		ssize_t n = nbt_copy_str(head, max, name);
		if (n < 0) return n;
		head += n;
	}

	switch (id) {
	case NBT_S8:
		if (head+1 > max)
			return -MCX_EFAULT;
		*head++ = val;
		break;
	case NBT_S16:
		if (head+2 > max)
			return -MCX_EFAULT;
		writebe16(head, val);
		head += 2;
		break;
	case NBT_S32:
		if (head+4 > max)
			return -MCX_EFAULT;
		writebe32(head, val);
		head += 4;
		break;
	case NBT_S64:
		if (head+8 > max)
			return -MCX_EFAULT;
		writebe64(head, val);
		head += 8;
		break;
	default:
		return -MCX_EINVAL;
	}
	return head - buf;
}

ssize_t nbt_addkey_float(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, f64 val)
{
	if ((ssize_t)(max - buf) < 0)
		return -MCX_EINVAL;
	u8 *head = buf;

	if (name) {
		*head = id;
		head += 3;
		ssize_t n = nbt_copy_str(head, max, name);
		if (n < 0) return n;
		head += n;
	}

	switch (id) {
	case NBT_F32:
		if (head+4 > max)
			return -MCX_EFAULT;
		writebe32(head, val);
		head += 4;
	case NBT_F64:
		if (head+8 > max)
			return -MCX_EFAULT;
		writebe64(head, val);
		head += 8;
	default:
		return -MCX_EINVAL;
	}
	return head - buf;
}

ssize_t nbt_addkey_arr(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, s32 len,
	const void *restrict dat)
{
	if (id != NBT_ARR_S8 && id != NBT_ARR_S32 && id != NBT_ARR_S64)
		return -MCX_EINVAL;
	if ((ssize_t)(max - buf) < 0)
		return -MCX_EINVAL;
	u8 *head = buf;

	if (name) {
		*head = id;
		head += 3;
		ssize_t n = nbt_copy_str(head, max, name);
		if (n < 0) return n;
		head += n;
	}

	int membsize = nbt_primitive_size[id];
	size_t size = len * membsize;
	if (head+4+size >= max)
		return MCX_EFAULT;
	writebe32(head, len);
	head += 4;
	switch (membsize) {
	case 1:
		memcpy(head, dat, size);
		head += size;
		break;
	case 4:
		while (len--) {
			writebe32(head, *( u32*)dat);
			head += 4;
			dat  += 4;
		}
		break;
	case 8:
		while (len--) {
			writebe64(head, *( u64*)dat);
			head += 8;
			dat  += 8;
		}
		break;
	}
	return head - buf;
}

ssize_t nbt_addkey_list(u8 *buf, u8 *max,
	const char *restrict name, enum nbt_tagid id, s32 len)
{
	if ((ssize_t)(max - buf) < 0)
		return -MCX_EINVAL;
	if (len < 0)
		return -MCX_EINVAL;
	u8 *head = buf;

	if (name) {
		*head = NBT_LIST;
		head += 3;
		ssize_t n = nbt_copy_str(head, max, name);
		if (n < 0) return n;
		head += n;
	}

	if (head+5 >= max)
		return -MCX_EFAULT;
	*head++ = id;
	writebe32(head, len);
	head += 4;
	return head - buf;
}

ssize_t nbt_addkey_compound(u8 *buf, u8 *max,
	const char *restrict name)
{
	if ((ssize_t)(max - buf) < 0)
		return -MCX_EINVAL;
	u8 *head = buf;

	if (name) {
		*head = NBT_LIST;
		head += 3;
		ssize_t n = nbt_copy_str(head, max, name);
		if (n < 0) return n;
		head += n;
	}
	return head - buf;
}


int nbt_tagnamecmp(const u8 *tag, const char *str)
{
	u16 n = readbe16(++tag);
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

/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include <libmcx/err.h>

#include <stddef.h>

static const char *errors[] = {
	[0] = "Success",
	[MCX_ETAG]   = "Invalid NBT tag",
	[MCX_ERANGE] = "Out of range",
	[MCX_EITER]  = "Too many iterations",
	[MCX_EBIG]   = "Result too large",
	[MCX_EFAULT] = "Bad address",
	[MCX_EINVAL] = "Invalid argument",
};
const char *mcx_errstr(int code)
{
	if (code < 0 || code > sizeof(errors) / sizeof(*errors))
		return NULL;
	return errors[code];
}

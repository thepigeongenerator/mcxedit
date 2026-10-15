/* SPDX-License-Identifier: BSD-2-Clause
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include <libmcx/error.h>

#include <stddef.h>
#include <string.h>

static const char *mcx_errors[] = {
	[-0x1000+EMCXTAG]  = "Invalid binary tag",
	[-0x1000+EMCXITER] = "Too many iterations",
};

const char *mcx_strerror(int code)
{
	if (code >= 0x1000) {
		code -= 0x1000;
		if (likely(code < lengthof(mcx_errors)))
			return mcx_errors[code];
		code += 0x1000;
	}
	return strerror(code);
}

/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#ifndef MCXEDIT_ERR_H
#define MCXEDIT_ERR_H 1
#include <libmcx/atrb.h>

/* libmcx error codes */
#define MCX_ETAG   1 /* Invalid NBT tag */
#define MCX_ERANGE 2 /* Out of range */
#define MCX_EITER  3 /* Too many iterations */
#define MCX_EBIG   4 /* Result too large */
#define MCX_EFAULT 5 /* Bad address */
#define MCX_EINVAL 6 /* Invalid argument */

/* Returns a pointer to the error string,
 * or NULL if the error code was invalid. */
const char *mcx_errstr(int code) CONST COLD;

#endif /* MCXEDIT_ERR_H */

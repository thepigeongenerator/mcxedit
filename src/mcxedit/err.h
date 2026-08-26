/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#ifndef MCXEDIT_ERR_H
#define MCXEDIT_ERR_H 1

#include <libmcx/atrb.h>
#include <stdarg.h>

void verr(int code, const char *fmt, va_list args)  MCX_COLD MCX_NORET;
void verrx(int code, const char *fmt, va_list args) MCX_COLD MCX_NORET;
void vwarn(const char *fmt, va_list args)  MCX_COLD;
void vwarnx(const char *fmt, va_list args) MCX_COLD;
void err(int code, const char *fmt, ...)  MCX_COLD MCX_NORET MCX_PRINTF(2, 3);
void errx(int code, const char *fmt, ...) MCX_COLD MCX_NORET MCX_PRINTF(2, 3);
void warn(const char *fmt, ...)  MCX_COLD MCX_PRINTF(1, 2);
void warnx(const char *fmt, ...) MCX_COLD MCX_PRINTF(1, 2);

#endif /* MCXEDIT_ERR_H */

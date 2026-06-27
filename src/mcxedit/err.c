/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#include "err.h"

#include "main.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <errhandlingapi.h>
#include <windows.h>
#endif

#if defined(_WIN32)
static void windows_print_error(void)
{
	void *buf;
	DWORD e = GetLastError();
	DWORD n = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		e,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&buf, 0, NULL);
	if (n != 0) {
		fputs(buf, stderr);
		LocalFree(buf);
	} else {
		fputc('\n', stderr);
	}
}
#endif

void verr(int code, const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	if (fmt) {
		vfprintf(stderr, fmt, args);
		fputs(": ", stderr);
	}
#if defined(_WIN32)
	if (!errno) {
		windows_print_error();
		exit(code);
	}
#endif
	fputs(strerror(errno), stderr);
	fputc('\n', stderr);
	exit(code);
}

void verrx(int code, const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	vfprintf(stderr, fmt, args);
	fputc('\n', stderr);
	exit(code);
}

void vwarn(const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	if (fmt) {
		vfprintf(stderr, fmt, args);
		fputs(": ", stderr);
	}
#if defined(_WIN32)
	if (!errno) {
		windows_print_error();
		return;
	}
#endif
	fputs(strerror(errno), stderr);
	fputc('\n', stderr);
}

void vwarnx(const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	vfprintf(stderr, fmt, args);
	fputc('\n', stderr);
}

void err(int code, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	verr(code, fmt, args);
	va_end(args);
}

void errx(int code, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	verrx(code, fmt, args);
	va_end(args);
}

void warn(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwarn(fmt, args);
	va_end(args);
}

void warnx(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwarnx(fmt, args);
	va_end(args);
}

/* Copyright (C)2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include <stdio.h>
#ifndef MCXEDIT_COMPAT_H
#define MCXEDIT_COMPAT_H 1

/* Provide the COMPAT_NOMAP constant. */
#if defined(__unix__)
#include <sys/mman.h>
#define COMPAT_NOMAP MAP_FAILED
#elif defined(_WIN32)
#include <stdlib.h>
#define COMPAT_NOMAP NULL
#else
#error "Platform unsupported"
#endif

#if defined(__unix__)
#define ISUNIX 1
#define ISWIN  0
#elif defined(_WIN32)
#define ISUNIX 0
#define ISWIN  1
#else
#error "Platform unsupported"
#endif

struct file {
#if defined(__unix__)
	int fd;
#elif defined(_WIN32)
	HANDLE h;
#else
#error "Platform unsupported"
#endif
};

/* Opens a file.
 * "pat" specifies the file-path.
 * need_write sets whether the file is writeable, or read-only.
 * Returns the file size, or -1 upon failure. */
off_t compat_open(const char *path, struct file *f, int need_write);

/* Closes a file from a handle created with "compat_open".
 * Returns 0 if successful, or -1 upon failure. */
int compat_close(struct file f);

/* Maps "size" in bytes from f to a memory address.
 * "need_write" dictates whether the mapping is writeable or read-only.
 * Returns COMPAT_NOMAP upon failure. (a platform-dependent constant) */
void *compat_map(struct file f, size_t size, int need_write);

/* Removes the mapping created with "compat_map".
 * "size" should be the amount of bytes that we're unmapping.
 * Returns 0 if successful, or -1 upon failure. */
int compat_unmap(struct file f, void *map, size_t size);

/* Truncates file f to a specified size.
 * Returns 0 if successful, or -1 upon failure.
 * NOTE:
 * - On POSIX systems growing the size requires a remap.
 * - On Windows systems this always requires a remap. */
int compat_truncate(struct file f, off_t size);

#endif /* MCXEDIT_COMPAT_H */

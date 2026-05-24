/* Copyright (C)2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include "compat.h"
#include "err.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#if defined(__unix__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#elif defined(_WIN32)
#include <fileapi.h>
#include <handleapi.h>
#include <windows.h>
#include <winnt.h>
#else
#error "Platform unsupported"
#endif

off_t compat_open(const char *pat, struct file *f, int need_write)
{
	assert(pat != NULL);
	assert(f != NULL);
#if defined(__unix__)
	f->fd = open(pat, need_write ? O_RDWR : O_RDONLY);
	if (f->fd < 0)
		goto err_open;
	struct stat st;
	if (fstat(f->fd, &st) < 0)
		goto err_stat;
	return st.st_size;
#elif defined(_WIN32)
	/* TODO: Look into CreateFileMappingA */
	f->h = CreateFileA((char *)pat,
		need_write ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (f->h == INVALID_HANDLE_VALUE)
		goto err_open;
	LARGE_INTEGER size;
	if (!GetFileSizeEx(f->h, &size))
		goto err_stat;
	return size.QuadPart; /* Praise be, MicroSlop! */
#else
#error "Platform unsupported"
#endif
err_open:
	warn("cannot open '%s'", pat);
	return -1;
err_stat:
	warn("cannot stat '%s'", pat);
	compat_close(*f);
	return -1;
}

int compat_close(struct file f)
{
#if defined(__unix__)
	/* Decided to not handle the SIGINT edge case, since it's rare and
	 * the programme exits after catching an interupt. */
	assert(f.fd >= 0);
	return close(f.fd);
#elif defined(_WIN32)
	assert(f.h != NULL);
	return -!CloseHandle(f.h);
#else
#error "Platform unsupported"
#endif
}

void *compat_map(struct file f, size_t size, int need_write)
{
	void *map;
#if defined(__unix__)
	assert(f.fd >= 0);
	int map_prot = need_write ? (PROT_READ | PROT_WRITE) : PROT_READ;
	map          = mmap(NULL, size, map_prot, MAP_SHARED, f.fd, 0);
#elif defined(_WIN32)
	/* TODO: Implement usage of MapViewOfFile */
	(void)need_write;
	assert(f.h != NULL);
	map = malloc(size);
	if (!map) return COMPAT_NOMAP;
	DWORD n;
	if (!ReadFile(f.h, map, size, &n, NULL) || n != (uintmax_t)size) {
		free(map);
		return COMPAT_NOMAP;
	}
#else
#error "Platform unsupported"
#endif
	return map;
}

int compat_unmap(const struct file f, void *map, size_t size)
{
	assert(map != COMPAT_NOMAP);
	assert(size > 0);
#if defined(__unix__)
	assert((uintptr_t)map % sysconf(_SC_PAGESIZE) == 0);
	assert(f.fd >= 0);
	(void)f;
	return munmap(map, size);
#elif defined(_WIN32)
	/* TODO: Implement the usage of UnmapViewOfFile */
	assert(f.h != NULL);
	DWORD n;
	if (!WriteFile(f.h, map, size, &n, NULL) || n != (uintmax_t)size)
		return -1;
	free(map);
	return 0;
#else
#error "Platform unsupported"
#endif
}

int compat_truncate(struct file f, off_t size)
{
	assert(size >= 0);
	int e;
#if defined(__unix__)
	assert(f.fd >= 0);
	do e = ftruncate(f.fd, size);
	while (e < 0 && errno == EINTR); /* e = 0 or -1*/
#elif defined(_WIN32)
	assert(f.h != NULL);
	LARGE_INTEGER slopsize = {.QuadPart = size};

	e = -1;
	if (SetFilePointerEx(f.h, slopsize, NULL, FILE_BEGIN))
		e = -!SetEndOfFile(f.h);
#else
#error "Platform unsupported"
#endif
	return e;
}

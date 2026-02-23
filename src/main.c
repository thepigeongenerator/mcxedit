/* Copyright (C)2025-2026 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(__unix__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "dat/mcx.h"
#include "types.h"

_Static_assert(-3 >> 5 == -1,
	"The platform does not compile "
	"right-shifting signed integers to an arithmetic shift!");

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i) {
		int fd = open(argv[i], O_RDWR);
		if (fd < 0) {
			warn("Failed to open '%s'", argv[i]);
			continue;
		}

		usize size, esize, tmp;

		struct stat st;
		fstat(fd, &st);
		size = st.st_size;
		if (size < MCX_TABLES) {
			warnx("Skipping; too small to contain table: (%zuB)'%s'", size, argv[i]);
			close(fd);
			continue;
		}
		tmp = size % MCX_SECTOR;
		if (tmp) warnx("Not 4KiB sector aligned! (+%zuB) '%s'", tmp, argv[i]);

		be32 *tbl = mmap(NULL, MCX_TABLES, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (tbl == MAP_FAILED) {
			warn("Skipping; failed to map to '%s'", argv[i]);
			close(fd);
			continue;
		}

		size /= MCX_SECTOR;
		esize = mcx_table_calcsize(tbl) / MCX_SECTOR;
		tmp   = size - esize;

		if (tmp) printf("%s:  \test:%4zu act:%4zu diff:%+4zd\n", argv[i], esize, size, tmp);
		munmap(tbl, MCX_TABLES);
		close(fd);
	}

	return 0;
}

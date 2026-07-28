# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
.POSIX:         # Provides well-defined defaults
.SUFFIXES:
.SECONDARY:     # Don't clean up intermediary files
SHELL = /bin/sh

VERONLY  = v0.1
ifndef VERSION
GIT_TAG := $(shell git describe --tags --match=${VERONLY} 2>/dev/null)
VERSION := $(if ${GIT_TAG},${GIT_TAG},${VERONLY})
export VERSION
endif

modules = mcxedit libmcx

srcdir  ?= ${CURDIR}
prefix   = /usr/local
bindir   = ${prefix}/bin
libdir   = ${prefix}/lib
sharedir = ${prefix}/share
mandir   = ${sharedir}/man
export srcdir

AR   = ar
CC   = cc
RM   = rm -vf
GZIP = gzip
INSTALL      = install -vD -m0644
INSTALL_EXEC = install -vD -m755

# Flags that may be overridden by the command-line
CPPFLAGS = -DNDEBUG
CFLAGS    = -O2 -g
LDFLAGS   =
LDLIBS    =

# Alternative method of inserting custom configurations.
# Mostly useful for development where you'd otherwise have to re-type the same
# flags over and over again.
-include ${srcdir}/.config.mk

# Flags that may not be overridden by the command-line
override\
CPPFLAGS += -U_GNU_SOURCE\
	    -DMCXEDIT_VERSION=\"${VERSION}\" -DMCXEDIT_SOURCE\
	    -I${srcdir}/include
override\
CFLAGS   += -std=gnu17\
	    -funsigned-char -fno-strict-overflow\
	    -Wall -Wextra -Wpedantic\
	    -Wmissing-declarations -Wmissing-prototypes -Wundef -Wvla\
	    -Werror=date-time -Werror=designated-init -Werror=return-type\
	    -Werror=strict-prototypes\
	    -Wno-pointer-arith -Wno-pointer-sign -Wno-sign-compare -Wno-switch\
	    -Wno-unused-but-set-variable -Wno-unused-const-variable\
	    -Wno-unused-parameter
override\
LDFLAGS  += $(foreach mod,${modules},-L${srcdir}/src/${mod})

MAKEFLAGS += -rR
ARFLAGS    = -r

# Set Q to @ to silence commands being printed, unless --no-silent has been set
ifeq (0, $(words $(findstring --no-silent,${MAKEFLAGS})))
MAKEFLAGS += --no-print-directory
msg=printf ' %-8s %s\n'
Q=@
else
msg=true
Q=
endif

# Rule Definitions
%.o: %.c
	@${msg} CC $@
	$Q${CC} -c -MMD\
		${CPPFLAGS} ${M-CPPFLAGS}\
		${CFLAGS} ${M-CFLAGS} -o $@ $<
%.so.o: %.c
	@${msg} CC $@
	$Q${CC} -c -MMD\
		${CPPFLAGS} ${M-CPPFLAGS}\
		${CFLAGS} -fPIC ${M-CFLAGS} -o $@ $<
%.gz: %
	@${msg} GZIP $@
	$Qgzip -fk $<

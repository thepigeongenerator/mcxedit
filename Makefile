# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
.POSIX:         # Provides well-defined defaults
.SUFFIXES:
.SECONDARY:     # Don't clean up intermediary files
SHELL = /bin/sh

# Versioning info, more granular version information
# (e.g. dev build) is retreived from git-describe.
# I'll assume if you use a tarball of a specific commit,
# you'll know what you're doing.
VERONLY  = v0.1
GIT_TAG := $(shell git describe --tags --match=${VERONLY} 2>/dev/null)
VERSION := $(if ${GIT_TAG},${GIT_TAG},${VERONLY})

# Definitions of the available modules and manuals.
# This is used in later rules for calling
modules = mcxedit libmcx
include man/Makefile
manpaths := $(addprefix man/, ${manuals})

# Set Q to @ to silence commands being printed, unless --no-silent has been set
ifeq (0, $(words $(findstring --no-silent,${MAKEFLAGS})))
MAKEFLAGS += --no-print-directory
msg=printf ' %-8s %s\n'
Q=@
else
msg=true
Q=
endif

# "all" is the default target when none has been specified.
# and must be the first target that make encounters.
all:

# Set-up directory information
prefix   = /usr/local
bindir   = ${prefix}/bin
libdir   = ${prefix}/lib
sharedir = ${prefix}/share
mandir   = ${sharedir}/man

ifeq (${OS},Windows_NT)
all: mcxedit.exe libmcx.dll libmcx.a
else
all: mcxedit libmcx.so libmcx.a
endif # Windows_NT

manpages: ${manpaths}
# Programmes used with some (minor) flags
AR   = ar
CC   = cc
RM   = rm -f
CP   = cp -f
GZIP = gzip
INSTALL      = install -D -m644
INSTALL_EXEC = install -D -m755

# Flags that may be overridden by the command-line
CPPFLAGS   = -DNDEBUG
CFLAGS     = -O2 -g
LDFLAGS    =
LDLIBS     =
MAKEFLAGS += -rR
ARFLAGS    = -r

# Alternative method of inserting custom configurations.
# Mostly useful for development where you'd otherwise have to re-type the same
# flags over and over again.
-include .config.mk

# Flags that may not be overridden by the command-line
override\
CPPFLAGS += -U_GNU_SOURCE\
	    -DMCXEDIT_VERSION=\"${VERSION}\" -DMCXEDIT_SOURCE\
	    -Iinclude
override\
CFLAGS   += -std=gnu17\
	    -funsigned-char -fno-strict-overflow\
	    -Wall -Wextra -Wpedantic\
	    -Wmissing-declarations -Wmissing-prototypes -Wvla\
	    -Werror=date-time -Werror=designated-init -Werror=return-type\
	    -Werror=strict-prototypes\
	    -Wno-pointer-arith -Wno-pointer-sign -Wno-sign-compare -Wno-switch\
	    -Wno-unused-but-set-variable -Wno-unused-const-variable\
	    -Wno-unused-parameter

# Include the Makefiles that define the objects and their dependencies.
-include $(modules:%=src/%/Makefile)

# Perform actions for each module:
#  * Modify the *-obj variables to point to the correct path.
#  * Modify the CPPFLAGS/CFLAGS variables in the compilation of cppflags/cflags.
#  * Generate rules for creating .a files for each module.
$(foreach mod,${modules},\
$(eval ${mod}-obj := $(addprefix src/${mod}/,${${mod}-obj}))\
$(eval src/${mod}/%.o: CPPFLAGS += ${${mod}-cppflags})      \
$(eval src/${mod}/%.o: CFLAGS   += ${${mod}-cflags})        \
$(eval src/${mod}/${mod}.a: ${${mod}-obj})                  \
)

mcxedit: libmcx.so
mcxedit.exe: libmcx.dll
mcxedit mcxedit.exe: src/mcxedit/mcxedit.a
	@${msg} LD $@
	$Q${CC} ${LDFLAGS} ${LDLIBS} -o $@ $^
libmcx.a: ${libmcx-obj}
libmcx.so libmcx.dll: ${libmcx-obj}
	@${msg} LD $@
	$Q${CC} -shared ${LDFLAGS} ${LDLIBS} -o $@ $^

%.a:
	@${msg} AR $@
	$Q${AR} ${ARFLAGS} -sc $@ $^

# Generic rule Definitions
%.s: %.c
	# Used for debugging the assembly of a given translation unit.
	@${msg} CC $@
	$Q${CC} -S ${CPPFLAGS} ${CFLAGS} -o $@ $<
%.o: %.c
	@${msg} CC $@
	$Q${CC} -c -MMD ${CPPFLAGS} ${CFLAGS} -o $@ $<
%.gz: %
	@${msg} GZIP $@
	$Qgzip -fk $<

# Manpage generation
%.0: %.0.rst
%.1: %.1.rst
%.2: %.2.rst
%.3: %.3.rst
%.4: %.4.rst
%.5: %.5.rst
%.6: %.6.rst
%.7: %.7.rst
%.8: %.8.rst
%.9: %.9.rst
%: %.rst
	@${msg} RST2MAN $@
	${Q}sed 's/@@VERSION@@/'${VERONLY}'/g;'\
	's/@@DATE@@/'$(shell date +%Y-%m-%d)'/g' $< | rst2man >$@

# Uses "find" to clean, since it'll have a better result
# It does have the potential side-effect of deleting unintended files,
# so this target should only remove easily regenerated files.
clean:
	@find . -type f \
		\( -name '*.[asod]' -o -name '*.so'\
		-o -name '*.obj'  -o -name '*.dll' -o -name '*.exe'\
		-o -name '*.gz' -o -name 'mcxedit'\
		\) -print | xargs ${RM} -v

# Installation targets
ifeq (${OS},Windows_NT)
# TODO: Windows installation
install: libmcx.dll
	$(error Windows install unsupported at this time.)
uninstall:
	$(error Windows uninstall unsupported at this time.)
else
__install_bin: mcxedit
	@${msg} INSTALL $<
	$Q${INSTALL_EXEC} $< ${DESTDIR}${bindir}
__install_lib: libmcx.so libmcx.a
	@${msg} INSTALL $<
	$Q${INSTALL_EXEC} $< ${DESTDIR}${libdir}
__install_man: ${manpaths}
	@${msg} INSTALL $<
	$Q${INSTALL} $< ${DESTDIR}${mandir}/man$(patsubst .%,%,\
		$(suffix $(basename $<)))
install: __install_bin __install_lib __install_man
uninstall:
	$Q${RM} -v\
		${DESTDIR}${libdir}/libmcx.so\
		${DESTDIR}${libdir}/libmcx.a\
		${DESTDIR}${bindir}/mcxedit\
		$(foreach man,${manuals},\
			${DESTDIR}${mandir}/man$(patsubst .%,%,\
			$(suffix $(basename ${man})))/${man})
PHONY += __install_bin __install_lib __install_man
endif

.PHONY: all manpages clean install uninstall ${PHONY}

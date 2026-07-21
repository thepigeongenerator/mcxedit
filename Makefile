# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
include Common.mk

all: ${modules}
	$Q${MAKE} -C man $@
clean:
	$Q$(foreach mod,${modules}, ${MAKE} -C src/${mod} $@ &&) true
	$Q${MAKE} -C man $@
manpages:
	$Q${MAKE} -C man $@
${modules}:
	${MAKE} -C src/$@ all
mcxedit: libmcx

install:
	$Q$(foreach mod,${modules}, ${MAKE} -C src/${mod} $@ &&) true
	$Q${MAKE} -C man $@
uninstall:
	$Q$(foreach mod,${modules}, ${MAKE} -C src/${mod} $@ &&) true
	$Q${MAKE} -C man $@

.PHONY: all clean manpages ${modules} install uninstall

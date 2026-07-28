# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
include ../../Common.mk
override M-CFLAGS = -flto
override LDFLAGS += -flto -O2
override LDLIBS  += -lmcx

src  := $(wildcard *.c)
obj  := $(src:.c=.o)
dep  := $(obj:.o=.d)

ifeq (${OS},Windows_NT)
all: mcxedit.exe
else
all: mcxedit
endif

mcxedit.a: ${obj}
	@${msg} AR $@
	$Q${AR} ${ARFLAGS} -sc $@ $^
mcxedit mcxedit.exe: mcxedit.a
	@${msg} LD $@
	$Q${CC} ${LDFLAGS} ${LDLIBS} -o $@ $^
mcxedit.static mcxedit.static.exe: mcxedit.a
	@${msg} LD $@
	$Q${CC} ${LDFLAGS} -static ${LDLIBS} -o $@ $^

clean:; $Q${RM} mcxedit.a \
	mcxedit mcxedit.exe\
	mcxedit.static mcxedit.static.exe\
	${obj} ${dep}

ifeq (${OS},Windows_NT)
# TODO: Windows installation
install: mcxedit.exe
	$(error Windows install unsupported at this time.)
uninstall:
	$(error Windows uninstall unsupported at this time.)
else
install: mcxedit
	$Q${INSTALL_EXEC} $< ${DESTDIR}${bindir}/$<
uninstall:
	$Q${RM} ${DESTDIR}${bindir}/$<
endif

.PHONY: all clean install uninstall

-include ${dep}

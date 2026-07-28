# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
include ../../Common.mk
override M-CFLAGS = -flto
override LDFLAGS += -shared -flto -O2 # No point for this, but eh.

obj  := err.o mcx.o nbt.o
sobj := err.so.o mcx.so.o nbt.so.o
dep  := $(obj:.o=.d)

ifeq (${OS},Windows_NT)
all: libmcx.a libmcx.dll
else
all: libmcx.a libmcx.so
endif

libmcx.a: ${obj}
	@${msg} AR $@
	$Q${AR} ${ARFLAGS} -sc $@ $^
libmcx.so libmcx.dll: ${sobj}
	@${msg} LD $@
	$Q${CC} ${LDFLAGS} ${LDLIBS} -o $@ $^

clean:; $Q${RM} libmcx.a libmcx.so libmcx.dll ${obj} ${sobj} ${dep}

ifeq (${OS},Windows_NT)
# TODO: Windows installation
install: libmcx.dll
	$(error Windows install unsupported at this time.)
uninstall:
	$(error Windows uninstall unsupported at this time.)
else
install: libmcx.so libmcx.a
	$Q${INSTALL_EXEC} $< ${DESTDIR}${libdir}/$<
uninstall:
	$Q${RM}\
		${DESTDIR}${libdir}/libmcx.so\
		${DESTDIR}${libdir}/libmcx.a
endif
.PHONY: all clean install uninstall

-include ${dep}

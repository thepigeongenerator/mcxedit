# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
include ../Common.mk
src   := $(wildcard  *.[0123456789].rst)
groff := $(patsubst  %.rst,%,${src})
man   := $(addsuffix .gz,${groff})

all: ${man}
manpages: ${groff}

%: %.rst
	@${msg} RST2MAN $@
	${Q}sed 's/@@VERSION@@/'${VERONLY}'/g;'\
	's/@@DATE@@/'$(shell date +%Y-%m-%d)'/g' $< | rst2man >$@

clean:; $Q${RM} ${man}

# Gotta be careful with the syntax here, since it's quite delicate.
# If the shell encounters a '\' in the wrong spot it'll fail to work.
install: ${man}
	$Qcase "$<" in\
		${foreach n,0 1 2 3 4 5 6 7 8 9,\
		*.$n.gz) ${INSTALL} $< ${DESTDIR}${mandir}/man$n/$< ;;\
	} esac
uninstall:
	$Qfor page in ${man}; do case "$$page" in\
		${foreach n,0 1 2 3 4 5 6 7 8 9,\
		*.$n.gz) ${RM} ${DESTDIR}${mandir}/man$n/$$page ;;\
	} esac done

.PHONY: all manpages clean install uninstall

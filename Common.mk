# SPDX-License-Identifier: GPL-2.0-only
# SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>

VERONLY  = v0.0
ifndef GIT_TAG
GIT_TAG := $(shell git describe --tags --match=${VERONLY} 2>/dev/null)
endif
VERSION := $(if ${GIT_TAG},${GIT_TAG},${VERONLY})
export VERSION GIT_TAG


# Configure the prefix directories for installation rules.
prefix = /usr/local
bindir = ${prefix}/bin
libdir = ${prefix}/lib
mandir = ${prefix}/share

# srcdir shall be set by the first make, and no subsequent makes.
export srcdir ?= ${CURDIR}

# Include persistent user configurations.
-include ${srcdir}/.config.mk

# Flags, including the flag in the definition so it may be overridden.
# Generally speaking, anything prior to the recursive reference of the variable
# is able to be overridden, anything afterwards is applied no matter what.
CPPFLAGS := -DNDEBUG -U_GNU_SOURCE ${CPPFLAGS}\
	    -DMCXEDIT_VERSION=\"${VERSION}\" -DMCXEDIT_SOURCE\
	    -I${srcdir}/include
CFLAGS   := -O2 ${CFLAGS} -g -std=gnu17
CFLAGS   += -funsigned-char
CFLAGS   += -fno-strict-overflow         # Signed arithmetic overflow is allowed.
CFLAGS   += -Wall -Wextra -Wpedantic     # Standard Warnings
CFLAGS   += -Wmissing-declarations
CFLAGS   += -Wmissing-prototypes
CFLAGS   += -Wvla                        # Kills the stack
CFLAGS   += -Werror=date-time            # Prevents reproducible binaries
CFLAGS   += -Werror=designated-init      # The attribute must be respected.
CFLAGS   += -Werror=return-type
CFLAGS   += -Werror=strict-prototypes    # Arguments must have a type.
CFLAGS   += -Wno-pointer-arith           # void* arithmetic.
CFLAGS   += -Wno-pointer-sign            # s32* can be implicitly cast to u32*.
CFLAGS   += -Wno-switch                  # switch needn't have all enum definitions.
CFLAGS   += -Wno-unused-but-set-variable # Preprocessor may mask variable usage.
CFLAGS   += -Wno-unused-const-variable   # Same as above.
CFLAGS   += -Wno-unused-parameter        # May not be used and see above.
LDFLAGS  := ${LDFLAGS} -L${srcdir}

# Rules so simple that it is fine to put them here.
# Not putting stuff such as C compilation here, since that should
# be handled by the module itself.
%.gz: %
	$(call msg,GZIP,$@)
	${Q}gzip -fk $<
%/:
	$(call msg,MKDIR,$@)
	${Q}mkdir -p $@

# Set Q to @ to silence commands being printed, unless --no-silent has been set
ifeq (0, $(words $(findstring --no-silent,${MAKEFLAGS})))
msg=@printf ' %-8s %s\n' "${1}" "${2}"
Q=@
else
msg=
Q=
endif

# Generate and include dependencies,
# ignoring any errors that may occur when doing so.
%.c.d: %.c; ${Q}${CC} -MM ${CPPFLAGS} -MF $@ $<
ifeq (0, $(words $(findstring ${MAKECMDGOALS}, clean)))
-include ${DEP}
endif

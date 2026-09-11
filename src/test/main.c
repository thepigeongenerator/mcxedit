/* SPDX-License-Identifier: GPL-2.0-only
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#define TEST_IMPLEMENTATION
#include "utest/test.h"

int main(int argc, char **argv)
{
	return test_main(argc, argv);
}

/* Compilation preprocessor test,
 * to ensure I don't use bad types in my headers.
 * Or other non-source related mistakes. */
#undef MCXEDIT_SOURCE
#include <libmcx/error.h>
#include <libmcx/mcx.h>
#include <libmcx/nbt.h>
#include <libmcx/stddef.h>
#include <libmcx/types.h>

/* Copyright (C)2025 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_MACRO_H
#define MCXEDIT_MACRO_H 1

#define WIDTHOF(t)       (sizeof(t) * 8) // gets the bit width of a type
#define MACRO_CAT(x, y)  x##y            // concatenate two macro variables together
#define MACRO_CAT2(x, y) MACRO_CAT(x, y) // concatenate two macro variables together
#define MACRO_STR(v)     #v              // for converting macro variable into a string
#define MACRO_STR2(v)    MACRO_STR(v)    // for a recursive string generation

#endif /* MCXEDIT_MACRO_H */

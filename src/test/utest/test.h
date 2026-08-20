/* SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: ©2026 Quinn Zieltjens <zieltjens@pigeonware.org>
 */
#ifndef UTEST_TEST_H
#define UTEST_TEST_H 1
#include <errno.h>
#include <error.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct test {
	int (*test)(void);
	const char *name;
};

extern struct test *tests;
extern size_t testlen;
extern size_t testcap;

#define test_register(func)                                                    \
	__attribute__((__constructor__))                                       \
	static void test_register_##func(void)                                 \
	{                                                                      \
		if (testlen >= testcap) {                                      \
			testcap = testcap ? testcap : 1;                       \
			testcap *= 2;                                          \
			tests = realloc(tests, testcap * sizeof(*tests));      \
			if (!tests) {                                          \
				error(1, errno, NULL);                         \
				__builtin_unreachable();                       \
			}                                                      \
		}                                                              \
		tests[testlen].test =  func;                                   \
		tests[testlen].name = #func;                                   \
		testlen++;                                                     \
	}                                                                      \
	void test_register_##func(void)

/* Execute all tests,
 * return value is 0 if all tests were successful. */
int test_main(int argc, char **argv);

/* Prints in a format similar to what returning a test would do,
 * but visually distinct to indicate this is a "subtask". */
__attribute__((__format__(printf, 3, 4)))
int test_printf(int res, const char *func, const char *fmt, ...);
int test_vprintf(int res, const char *func, const char *fmt, va_list args);

/* Short-hand for test_printf, omitting the function name. */
#define testlog(res, ...) test_printf(res, __func__, __VA_ARGS__)

/* Useful for chaining conditions, whilst having logging on each step. */
#define testeval(cond) (testlog(!(cond), #cond "\n"), cond)

#ifdef TEST_IMPLEMENTATION
struct test *tests = NULL;
size_t testlen = 0;
size_t testcap = 0;

int test_main(int argc, char **argv)
{
	(void)argc, (void)argv; /* NOTE: Flags unneeded at this time. */
	/* Execute Tests. */
	size_t n = testlen;
	size_t success = 0;
	struct test *test = tests;
	while (n--) {
		const char *str = "[\033[1;31mFAIL\033[0m] %s %zu/%zu\n";
		if (!test->test()) {
			success++;
			str = "[\033[1;32m OK \033[0m] %s %zu/%zu\n";
		}
		printf(str, test->name, testlen - n, testlen);
		test++;
	}
	printf("%zu/%zu tests successful.\n", success, testlen);
	return testlen != success;
}

int test_vprintf(int res, const char *func, const char *fmt, va_list args)
{
	const char *str = res
		? " \033[1;31mFAIL\033[0m  %s\t"
		: " \033[1;32m OK \033[0m  %s\t";
	return printf(str, func) +
		vprintf(fmt, args);
}
int test_printf(int res, const char *func, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int ret = test_vprintf(res, func, fmt, args);
	va_end(args);
	return ret;
}
#endif /* TEST_IMPLEMENTATION */
#endif /* UTEST_TEST_H */

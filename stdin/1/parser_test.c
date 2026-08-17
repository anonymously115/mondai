#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "minunit.h"
#include "parser.h"

int tests_run = 0;

static char* message_s(const char *expected, const char *actual) {
	static char msg[256];
	snprintf(msg, sizeof(msg), "Error: expected: <%s> but was: <%s>", expected, actual);
	return msg;
}

static char* message_d(int expected, int actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%d> but was: <%d>", expected, actual);
	return msg;
}

static char* test_parse(const char *str, const char *expected) {
	errno = 0;
	char actual[101] = { };
	mu_assert("Error: expected: <true> but was: <false>", parse(str, actual));
	mu_assert(message_s(expected, actual), !strcmp(actual, expected));
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_invalid(const char *str) {
	errno = 0;
	char actual[101] = { };
	mu_assert("Error: expected: <false> but was: <true>", !parse(str, actual));
	mu_assert(message_s("", actual), !strcmp(actual, ""));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_0() { return test_parse_invalid(NULL); }
static char* test_parse_1() { return test_parse_invalid(""); }
static char* test_parse_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse("0", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_3() { return test_parse_invalid("\n"); }
static char* test_parse_4() { return test_parse("5\n", "5"); }
static char* test_parse_5() { return test_parse("M\n", "M"); }
static char* test_parse_6() { return test_parse("n\n", "n"); }
static char* test_parse_7() { return test_parse_invalid("_\n"); }
static char* test_parse_8() { return test_parse("00\n", "00"); }
static char* test_parse_9() { return test_parse_invalid("_0\n"); }
static char* test_parse_10() { return test_parse_invalid("0_\n"); }
static char* test_parse_11() { return test_parse("000\n", "000"); }
static char* test_parse_12() { return test_parse_invalid("_00\n"); }
static char* test_parse_13() { return test_parse_invalid("0_0\n"); }
static char* test_parse_14() { return test_parse_invalid("00_\n"); }
static char* test_parse_15() {
	return test_parse(
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZab\n",
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZab"
	);
}
static char* test_parse_16() {
	return test_parse_invalid(
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabc\n"
	);
}

static char* all_tests() {
    mu_run_test(test_parse_0);
    mu_run_test(test_parse_1);
    mu_run_test(test_parse_2);
    mu_run_test(test_parse_3);
    mu_run_test(test_parse_4);
    mu_run_test(test_parse_5);
    mu_run_test(test_parse_6);
    mu_run_test(test_parse_7);
    mu_run_test(test_parse_8);
    mu_run_test(test_parse_9);
    mu_run_test(test_parse_10);
    mu_run_test(test_parse_11);
    mu_run_test(test_parse_12);
    mu_run_test(test_parse_13);
    mu_run_test(test_parse_14);
    mu_run_test(test_parse_15);
    mu_run_test(test_parse_16);
    return 0;
}

int main() {
	char *result = all_tests();
	if (result != 0) {
		fprintf(stderr, "%s\n", result);
		fprintf(stderr, "Tests run: %d\n", tests_run);
	} else {
		fprintf(stdout, "ALL TESTS PASSED\n");
		fprintf(stdout, "Tests run: %d\n", tests_run);
	}
	return result != 0;
}

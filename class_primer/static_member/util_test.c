#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "minunit.h"
#include "util.h"

int tests_run = 0;

static char* message_s(const char *expected, const char *actual) {
	static char msg[256];
	snprintf(msg, sizeof(msg), "Error: expected: <%s> but was: <%s>", expected, actual);
	return msg;
}

static char* message_zu(size_t expected, size_t actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%zu> but was: <%zu>", expected, actual);
	return msg;
}

static char* message_d(int expected, int actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%d> but was: <%d>", expected, actual);
	return msg;
}

static char* test_chomp(const char *str, const char *expected) {
	errno = 0;
	char actual[strlen(str) + 1];
	const char *ptr = chomp(strcpy(actual, str));
	mu_assert(message_s(expected, actual), !strcmp(actual, expected));
    mu_assert(message_s(expected, ptr), !strcmp(ptr, expected));
	return 0;
}

static char* test_chomp_0() {
	errno = 0;
    mu_assert("Error: expected: <null> but was: <not null>", !chomp(NULL));
	return 0;
}

static char* test_chomp_1() { return test_chomp("", ""); }
static char* test_chomp_2() { return test_chomp("\n", ""); }
static char* test_chomp_3() { return test_chomp("0", "0"); }
static char* test_chomp_4() { return test_chomp(" \n", ""); }
static char* test_chomp_5() { return test_chomp("0\n", "0"); }
static char* test_chomp_6() { return test_chomp(" 0", " 0"); }
static char* test_chomp_7() { return test_chomp("00", "00"); }
static char* test_chomp_8() { return test_chomp("  \n", ""); }
static char* test_chomp_9() { return test_chomp("0 \n", "0"); }
static char* test_chomp_10() { return test_chomp(" 0\n", " 0"); }
static char* test_chomp_11() { return test_chomp("00\n", "00"); }
static char* test_chomp_12() { return test_chomp("  0", "  0"); }
static char* test_chomp_13() { return test_chomp("0 0", "0 0"); }
static char* test_chomp_14() { return test_chomp(" 00", " 00"); }
static char* test_chomp_15() { return test_chomp("000", "000"); }

static char* test_parse_ulong(const char *str, size_t expected) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_ulong(str, &actual));
	mu_assert(message_zu(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_ulong_invalid(const char *str) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_ulong(str, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_ulong_out_of_range(const char *str) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_ulong(str, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_ulong_0() { return test_parse_ulong_invalid(NULL); }
static char* test_parse_ulong_1() { return test_parse_ulong_invalid(""); }
static char* test_parse_ulong_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_ulong("0", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_ulong_3() { return test_parse_ulong("0", 0); }
static char* test_parse_ulong_4() { return test_parse_ulong_invalid("00"); }
static char* test_parse_ulong_5() { return test_parse_ulong("9", 9); }
static char* test_parse_ulong_6() { return test_parse_ulong_invalid("\n"); }
static char* test_parse_ulong_7() { return test_parse_ulong("90", 90); }
static char* test_parse_ulong_8() { return test_parse_ulong_invalid("-1"); }
static char* test_parse_ulong_9() { return test_parse_ulong_invalid("1\n"); }
static char* test_parse_ulong_10() { return test_parse_ulong("900", 900); }
static char* test_parse_ulong_11() { return test_parse_ulong_invalid("-11"); }
static char* test_parse_ulong_12() { return test_parse_ulong_invalid("1.1"); }
static char* test_parse_ulong_13() { return test_parse_ulong_invalid("11\n"); }
static char* test_parse_ulong_14() { return test_parse_ulong("18446744073709551615", SIZE_MAX); }
static char* test_parse_ulong_15() { return test_parse_ulong_out_of_range("18446744073709551616"); }

static char* test_parse_int(const char *str, int expected) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_int(str, &actual));
	mu_assert(message_d(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_int_invalid(const char *str) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_int(str, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_int_out_of_range(const char *str) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_int(str, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_int_0() { return test_parse_int_invalid(NULL); }
static char* test_parse_int_1() { return test_parse_int_invalid(""); }
static char* test_parse_int_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_int("-1", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_int_3() { return test_parse_int_invalid("-0"); }
static char* test_parse_int_4() { return test_parse_int_invalid("-"); }
static char* test_parse_int_5() { return test_parse_int("-1", -1); }
static char* test_parse_int_6() { return test_parse_int_invalid("--"); }
static char* test_parse_int_7() { return test_parse_int("0", 0); }
static char* test_parse_int_8() { return test_parse_int_invalid("00"); }
static char* test_parse_int_9() { return test_parse_int_invalid("\n"); }
static char* test_parse_int_10() { return test_parse_int_invalid("1-1"); }
static char* test_parse_int_11() { return test_parse_int_invalid("11-"); }
static char* test_parse_int_12() { return test_parse_int_out_of_range("-2147483649"); }
static char* test_parse_int_13() { return test_parse_int("-2147483648", INT_MIN); }
static char* test_parse_int_14() { return test_parse_int("2147483647", INT_MAX); }
static char* test_parse_int_15() { return test_parse_int_out_of_range("2147483648"); }

static char* test_parse_uint(const char *str, unsigned expected) {
	errno = 0;
	unsigned actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_uint(str, &actual));
	mu_assert(message_zu(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_uint_invalid(const char *str) {
	errno = 0;
	unsigned actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_uint(str, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_uint_out_of_range(const char *str) {
	errno = 0;
	unsigned actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_uint(str, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_uint_0() { return test_parse_uint_invalid(NULL); }
static char* test_parse_uint_1() { return test_parse_uint_invalid(""); }
static char* test_parse_uint_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_uint("0", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_uint_3() { return test_parse_uint("0", 0); }
static char* test_parse_uint_4() { return test_parse_uint_invalid("00"); }
static char* test_parse_uint_5() { return test_parse_uint("9", 9); }
static char* test_parse_uint_6() { return test_parse_uint_invalid("\n"); }
static char* test_parse_uint_7() { return test_parse_uint("90", 90); }
static char* test_parse_uint_8() { return test_parse_uint_invalid("-1"); }
static char* test_parse_uint_9() { return test_parse_uint_invalid("1\n"); }
static char* test_parse_uint_10() { return test_parse_uint("900", 900); }
static char* test_parse_uint_11() { return test_parse_uint_invalid("-11"); }
static char* test_parse_uint_12() { return test_parse_uint_invalid("1.1"); }
static char* test_parse_uint_13() { return test_parse_uint_invalid("11\n"); }
static char* test_parse_uint_14() { return test_parse_uint("4294967295", UINT_MAX); }
static char* test_parse_uint_15() { return test_parse_uint_out_of_range("4294967296"); }

static char* all_tests() {
    mu_run_test(test_chomp_0);
    mu_run_test(test_chomp_1);
    mu_run_test(test_chomp_2);
    mu_run_test(test_chomp_3);
    mu_run_test(test_chomp_4);
    mu_run_test(test_chomp_5);
    mu_run_test(test_chomp_6);
    mu_run_test(test_chomp_7);
    mu_run_test(test_chomp_8);
    mu_run_test(test_chomp_9);
    mu_run_test(test_chomp_10);
    mu_run_test(test_chomp_11);
    mu_run_test(test_chomp_12);
    mu_run_test(test_chomp_13);
    mu_run_test(test_chomp_14);
    mu_run_test(test_chomp_15);
    mu_run_test(test_parse_ulong_0);
    mu_run_test(test_parse_ulong_1);
    mu_run_test(test_parse_ulong_2);
    mu_run_test(test_parse_ulong_3);
    mu_run_test(test_parse_ulong_4);
    mu_run_test(test_parse_ulong_5);
    mu_run_test(test_parse_ulong_6);
    mu_run_test(test_parse_ulong_7);
    mu_run_test(test_parse_ulong_8);
    mu_run_test(test_parse_ulong_9);
    mu_run_test(test_parse_ulong_10);
    mu_run_test(test_parse_ulong_11);
    mu_run_test(test_parse_ulong_12);
    mu_run_test(test_parse_ulong_13);
    mu_run_test(test_parse_ulong_14);
    mu_run_test(test_parse_ulong_15);
    mu_run_test(test_parse_int_0);
    mu_run_test(test_parse_int_1);
    mu_run_test(test_parse_int_2);
    mu_run_test(test_parse_int_3);
    mu_run_test(test_parse_int_4);
    mu_run_test(test_parse_int_5);
    mu_run_test(test_parse_int_6);
    mu_run_test(test_parse_int_7);
    mu_run_test(test_parse_int_8);
    mu_run_test(test_parse_int_9);
    mu_run_test(test_parse_int_10);
    mu_run_test(test_parse_int_11);
    mu_run_test(test_parse_int_12);
    mu_run_test(test_parse_int_13);
    mu_run_test(test_parse_int_14);
    mu_run_test(test_parse_int_15);
    mu_run_test(test_parse_uint_0);
    mu_run_test(test_parse_uint_1);
    mu_run_test(test_parse_uint_2);
    mu_run_test(test_parse_uint_3);
    mu_run_test(test_parse_uint_4);
    mu_run_test(test_parse_uint_5);
    mu_run_test(test_parse_uint_6);
    mu_run_test(test_parse_uint_7);
    mu_run_test(test_parse_uint_8);
    mu_run_test(test_parse_uint_9);
    mu_run_test(test_parse_uint_10);
    mu_run_test(test_parse_uint_11);
    mu_run_test(test_parse_uint_12);
    mu_run_test(test_parse_uint_13);
    mu_run_test(test_parse_uint_14);
    mu_run_test(test_parse_uint_15);
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

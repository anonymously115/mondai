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

static char* message_zu(size_t expected, size_t actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%zu> but was: <%zu>", expected, actual);
	return msg;
}

static char* test_parse_size(const char *str, size_t en, size_t ek) {
	errno = 0;
	size_t n, k;
	mu_assert("Error: expected: <true> but was: <false>", parse_size(str, &n, &k));
	mu_assert(message_zu(en, n), n == en);
	mu_assert(message_zu(ek, k), k == ek);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_size_invalid(const char *str) {
	errno = 0;
	size_t n, k;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size(str, &n, &k));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_size_out_of_range(const char *str) {
	errno = 0;
	size_t n, k;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size(str, &n, &k));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_size_0() { return test_parse_size_invalid(NULL); }
static char* test_parse_size_1() { return test_parse_size_invalid(""); }
static char* test_parse_size_2() {
	errno = 0;
	size_t k;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size("1 1\n", NULL, &k));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_size_3() {
	errno = 0;
	size_t n;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size("1 1\n", &n, NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_size_4() { return test_parse_size_invalid("\n"); }
static char* test_parse_size_5() { return test_parse_size_invalid("1\n"); }
static char* test_parse_size_6() { return test_parse_size_invalid("1 1 1\n"); }
static char* test_parse_size_7() { return test_parse_size_out_of_range("0 0\n"); }
static char* test_parse_size_8() { return test_parse_size_out_of_range("1 0\n"); }
static char* test_parse_size_9() { return test_parse_size("1 1\n", 1, 1); }
static char* test_parse_size_10() { return test_parse_size_out_of_range("1 2\n"); }
static char* test_parse_size_11() { return test_parse_size("100000 100000\n", 100000, 100000); }
static char* test_parse_size_12() { return test_parse_size_out_of_range("100001 100000\n");}

static char* test_parse_element(const char *str, int expected) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_element(str, &actual));
	mu_assert(message_d(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_element_invalid(const char *str) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element(str, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_element_out_of_range(const char *str) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element(str, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_element_0() { return test_parse_element_invalid(NULL); }
static char* test_parse_element_1() { return test_parse_element_invalid(""); }
static char* test_parse_element_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element("1\n", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_element_3() { return test_parse_element_invalid("\n"); }
static char* test_parse_element_4() { return test_parse_element_out_of_range("-1\n"); }
static char* test_parse_element_5() { return test_parse_element("0\n", 0); }
static char* test_parse_element_6() { return test_parse_element("10000\n", 10000); }
static char* test_parse_element_7() { return test_parse_element_out_of_range("10001\n"); }

static char* test_parse_query(const char *str, const char *expected) {
	errno = 0;
	char actual[5] = { };
	mu_assert("Error: expected: <true> but was: <false>", parse_query(str, actual));
	mu_assert(message_s(expected, actual), !strcmp(actual, expected));
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_query_invalid(const char *str) {
	errno = 0;
	char actual[5] = { };
	mu_assert("Error: expected: <false> but was: <true>", !parse_query(str, actual));
	mu_assert(message_s("", actual), !strcmp(actual, ""));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_query_0() { return test_parse_query_invalid(NULL); }
static char* test_parse_query_1() { return test_parse_query_invalid(""); }
static char* test_parse_query_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_query("pop", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_query_3() { return test_parse_query_invalid("\n"); }
static char* test_parse_query_4() { return test_parse_query("pop\n", "pop"); }
static char* test_parse_query_5() { return test_parse_query("show\n", "show"); }
static char* test_parse_query_6() { return test_parse_query_invalid("Pop\n"); }
static char* test_parse_query_7() { return test_parse_query_invalid("Show\n"); }

static char* all_tests() {
	mu_run_test(test_parse_size_0);
	mu_run_test(test_parse_size_1);
	mu_run_test(test_parse_size_2);
	mu_run_test(test_parse_size_3);
	mu_run_test(test_parse_size_4);
	mu_run_test(test_parse_size_5);
	mu_run_test(test_parse_size_6);
	mu_run_test(test_parse_size_7);
	mu_run_test(test_parse_size_8);
	mu_run_test(test_parse_size_9);
	mu_run_test(test_parse_size_10);
	mu_run_test(test_parse_size_11);
	mu_run_test(test_parse_size_12);
	mu_run_test(test_parse_element_0);
	mu_run_test(test_parse_element_1);
	mu_run_test(test_parse_element_2);
	mu_run_test(test_parse_element_3);
	mu_run_test(test_parse_element_4);
	mu_run_test(test_parse_element_5);
	mu_run_test(test_parse_element_6);
	mu_run_test(test_parse_element_7);
    mu_run_test(test_parse_query_0);
    mu_run_test(test_parse_query_1);
    mu_run_test(test_parse_query_2);
    mu_run_test(test_parse_query_3);
    mu_run_test(test_parse_query_4);
    mu_run_test(test_parse_query_5);
    mu_run_test(test_parse_query_6);
    mu_run_test(test_parse_query_7);
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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "minunit.h"
#include "parser.h"

int tests_run = 0;

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

static char* test_parse_size(const char* str, size_t en, size_t eq) {
	errno = 0;
	size_t n, q;
	mu_assert("Error: expected: <true> but was: <false>", parse_size(str, &n, &q));
	mu_assert(message_zu(en, n), n == en);
	mu_assert(message_zu(eq, q), q == eq);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_size_invalid(const char* str) {
	errno = 0;
	size_t n, q;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size(str, &n, &q));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_size_out_of_range(const char* str) {
	errno = 0;
	size_t n, q;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size(str, &n, &q));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_size_0() { return test_parse_size_invalid(NULL); }
static char* test_parse_size_1() { return test_parse_size_invalid(""); }
static char* test_parse_size_2() {
	errno = 0;
	size_t q;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size("1 1", NULL, &q));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_size_3() {
	errno = 0;
	size_t n;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size("1 1", &n, NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_size_4() { return test_parse_size_invalid("\n"); }
static char* test_parse_size_5() { return test_parse_size_invalid("1\n"); }
static char* test_parse_size_6() { return test_parse_size_invalid("1 1 1\n"); }
static char* test_parse_size_7() { return test_parse_size_out_of_range("0 1\n"); }
static char* test_parse_size_8() { return test_parse_size_out_of_range("1 0\n"); }
static char* test_parse_size_9() { return test_parse_size("1 1\n", 1, 1); }
static char* test_parse_size_10() { return test_parse_size("100000 100000\n", 100000, 100000); }
static char* test_parse_size_11() { return test_parse_size_out_of_range("100001 100000\n"); }
static char* test_parse_size_12() { return test_parse_size_out_of_range("100000 100001\n"); }

static char* test_parse_element(const char* str, size_t expected) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_element(str, &actual));
	mu_assert(message_zu(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_element_invalid(const char* str) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element(str, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_element_out_of_range(const char* str) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element(str, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_element_0() { return test_parse_element_invalid(NULL); }
static char* test_parse_element_1() { return test_parse_element_invalid(""); }
static char* test_parse_element_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element("1", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_element_3() { return test_parse_element_invalid("\n"); }
static char* test_parse_element_4() { return test_parse_element("0\n", 0); }
static char* test_parse_element_5() { return test_parse_element("1000000\n", 1000000); }
static char* test_parse_element_6() { return test_parse_element_out_of_range("1000001\n"); }

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

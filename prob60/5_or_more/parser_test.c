#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
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

static char* test_parse_size(const char *str, size_t expected) {
	errno = 0;
	size_t actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_size(str, &actual));
	mu_assert(message_zu(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_size_invalid(const char *str) {
	errno = 0;
	size_t actual = SIZE_MAX;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size(str, &actual));
	mu_assert(message_zu(SIZE_MAX, actual), actual == SIZE_MAX);
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_size_out_of_range(const char *str) {
	errno = 0;
	size_t actual = SIZE_MAX;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size(str, &actual));
	mu_assert(message_zu(SIZE_MAX, actual), actual == SIZE_MAX);
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_size_0() { return test_parse_size_invalid(NULL); }
static char* test_parse_size_1() { return test_parse_size_invalid(""); }
static char* test_parse_size_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_size("1", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_size_3() { return test_parse_size_invalid("\n"); }
static char* test_parse_size_4() { return test_parse_size_out_of_range("0\n"); } 
static char* test_parse_size_5() { return test_parse_size("1\n", 1); }
static char* test_parse_size_6() { return test_parse_size("1000\n", 1000); }
static char* test_parse_size_7() { return test_parse_size_out_of_range("1001\n"); }

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
	int actual = INT_MIN;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element(str, &actual));
	mu_assert(message_d(INT_MIN, actual), actual == INT_MIN);
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_element_out_of_range(const char *str) {
	errno = 0;
	int actual = INT_MIN;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element(str, &actual));
	mu_assert(message_d(INT_MIN, actual), actual == INT_MIN);
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_element_0() { return test_parse_element_invalid(NULL); }
static char* test_parse_element_1() { return test_parse_element_invalid(""); }
static char* test_parse_element_2() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_element("0", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_element_3() { return test_parse_element_invalid("\n"); }
static char* test_parse_element_4() { return test_parse_element_out_of_range("-1000001\n"); } 
static char* test_parse_element_5() { return test_parse_element("-1000000\n", -1000000); }
static char* test_parse_element_6() { return test_parse_element("1000000\n", 1000000); }
static char* test_parse_element_7() { return test_parse_element_out_of_range("1000001\n"); }

static char* all_tests() {
    mu_run_test(test_parse_size_0);
    mu_run_test(test_parse_size_1);
    mu_run_test(test_parse_size_2);
    mu_run_test(test_parse_size_3);
    mu_run_test(test_parse_size_4);
    mu_run_test(test_parse_size_5);
    mu_run_test(test_parse_size_6);
    mu_run_test(test_parse_size_7);
    mu_run_test(test_parse_element_0);
    mu_run_test(test_parse_element_1);
    mu_run_test(test_parse_element_2);
    mu_run_test(test_parse_element_3);
    mu_run_test(test_parse_element_4);
    mu_run_test(test_parse_element_5);
    mu_run_test(test_parse_element_6);
    mu_run_test(test_parse_element_7);
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

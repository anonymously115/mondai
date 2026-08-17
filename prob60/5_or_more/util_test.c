#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "minunit.h"
#include "util.h"
#define SIZE(a) (sizeof(a) / sizeof(a[0]))

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
	char actual[strlen(str) + 1];
	const char *ptr = chomp(strcpy(actual, str));
	mu_assert(message_s(expected, actual), !strcmp(actual, expected));
	mu_assert(message_s(expected, ptr), !strcmp(ptr, expected));
	return 0;
}

static char* test_chomp_0() {
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

static char* test_sum(size_t n, const int A[], int expected) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <true> but was: <false>", sum(n, A, &actual));
	mu_assert(message_d(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_sum_invalid(size_t n, const int A[]) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <false> but was: <true>", !sum(n, A, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_sum_out_of_range(size_t n, const int A[]) {
	errno = 0;
	int actual;
	mu_assert("Error: expected: <false> but was: <true>", !sum(n, A, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_sum_0() { return test_sum(0, NULL, 0); }
static char* test_sum_1() { return test_sum_invalid(1, NULL); }
static char* test_sum_2() { 
	errno = 0;
	int A[] = { 0 };
	mu_assert("Error: expected: <false> but was: <true>", !sum(SIZE(A), A, NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_sum_3() { 
	int A[] = { 0 };
	return test_sum(SIZE(A), A, 0);
}
static char* test_sum_4() { 
	int A[] = { INT_MAX };
	return test_sum(SIZE(A), A, INT_MAX);
}
static char* test_sum_5() { 
	int A[] = { INT_MIN };
	return test_sum(SIZE(A), A, INT_MIN);
}
static char* test_sum_6() {
	int A[65536] = { };
	for (size_t i = 0; i < 32767; i++) {
		A[i] = i + 1;
	}
	for (size_t i = 32767; i < 65536; i++) {
		A[i] = i;
	}
	return test_sum(SIZE(A), A, INT_MAX);
}
static char* test_sum_7() {
	int A[65536] = { };
	for (size_t i = 0; i < 32768; i++) {
		A[i] = ~i;
	}
	for (size_t i = 32768; i < 65536; i++) {
		A[i] = -i;
	}
	return test_sum(SIZE(A), A, INT_MIN);
}
static char* test_sum_8() { 
	int A[] = { INT_MAX, 1 };
	return test_sum_out_of_range(SIZE(A), A);
}
static char* test_sum_9() { 
	int A[] = { 1, INT_MAX };
	return test_sum_out_of_range(SIZE(A), A);
}
static char* test_sum_10() { 
	int A[] = { INT_MIN, -1 };
	return test_sum_out_of_range(SIZE(A), A);
}
static char* test_sum_11() { 
	int A[] = { -1, INT_MIN };
	return test_sum_out_of_range(SIZE(A), A);
}
static char* test_sum_12() { 
	int A[] = { 1, INT_MAX, INT_MIN };
	return test_sum_out_of_range(SIZE(A), A);
}
static char* test_sum_13() { 
	int A[] = { INT_MAX, 1, INT_MIN };
	return test_sum_out_of_range(SIZE(A), A);
}
static char* test_sum_14() { 
	int A[] = { 1, INT_MIN, INT_MAX };
	return test_sum(SIZE(A), A, 0);
}
static char* test_sum_15() { 
	int A[] = { INT_MIN, 1, INT_MAX };
	return test_sum(SIZE(A), A, 0);
}
static char* test_sum_16() { 
	int A[] = { INT_MAX, INT_MIN, 1 };
	return test_sum(SIZE(A), A, 0);
}
static char* test_sum_17() { 
	int A[] = { INT_MIN, INT_MAX, 1 };
	return test_sum(SIZE(A), A, 0);
}

static char* test_is_5_or_more_0() {
	mu_assert("Error: expected: <false> but was: <true>", !is_5_or_more(4));
	return 0;
}

static char* test_is_5_or_more_1() {
	mu_assert("Error: expected: <true> but was: <false>", is_5_or_more(5));
	return 0;
}

static char* test_filter(bool (*fn)(int), size_t n, const int A[], size_t em, const int expected[]) {
	errno = 0;
	size_t m;
	int actual[n ? n : 1];
	mu_assert("Error: expected: <true> but was: <false>", filter(fn, n, A, &m, actual));
	mu_assert(message_zu(em, m), m == em);
	for (size_t i = 0; i < em; i++) {
		mu_assert(message_d(expected[i], actual[i]), actual[i] == expected[i]);
	}
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_filter_invalid(bool (*fn)(int), size_t n, const int A[]) {
	errno = 0;
	size_t m;
	int actual[n ? n : 1];
	mu_assert("Error: expected: <false> but was: <true>", !filter(fn, n, A, &m, actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static bool stub(int n) {
	return !!n;
}

static bool stub_false(int n) {
	(void) n;
	return false;
}

static bool stub_true(int n) {
	(void) n;
	return true;
}

static char* test_filter_0() { return test_filter(stub, 0, NULL, 0, NULL); }
static char* test_filter_1() { return test_filter_invalid(stub, 1, NULL); }
static char* test_filter_2() {
	int A[] = { 0 };
	return test_filter_invalid(NULL, SIZE(A), A);
}
static char* test_filter_3() {
	errno = 0;
	int A[] = { 0 };
	int actual[SIZE(A)];
	mu_assert("Error: expected: <false> but was: <true>", !filter(stub, SIZE(A), A, NULL, actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_filter_4() {
	errno = 0;
	int A[] = { 0 };
	size_t m;
	mu_assert("Error: expected: <false> but was: <true>", !filter(stub, SIZE(A), A, &m, NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_filter_5() {
	int A[] = { 0 };
	return test_filter(stub_false, SIZE(A), A, 0, NULL); 
}
static char* test_filter_6() {
	int A[] = { 0 };
	int B[] = { 0 };
	return test_filter(stub_true, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_7() {
	int A[] = { 0, 0 };
	return test_filter(stub, SIZE(A), A, 0, NULL); 
}
static char* test_filter_8() {
	int A[] = { 0, 1 };
	int B[] = { 1 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_9() {
	int A[] = { 1, 0 };
	int B[] = { 1 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_10() {
	int A[] = { 1, 2 };
	int B[] = { 1, 2 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_11() {
	int A[] = { 0, 0, 0 };
	return test_filter(stub, SIZE(A), A, 0, NULL); 
}
static char* test_filter_12() {
	int A[] = { 0, 0, 1 };
	int B[] = { 1 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_13() {
	int A[] = { 0, 1, 0 };
	int B[] = { 1 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_14() {
	int A[] = { 0, 1, 2 };
	int B[] = { 1, 2 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_15() {
	int A[] = { 1, 0, 0 };
	int B[] = { 1 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_16() {
	int A[] = { 1, 0, 2 };
	int B[] = { 1, 2 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_17() {
	int A[] = { 1, 2, 0 };
	int B[] = { 1, 2 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}
static char* test_filter_18() {
	int A[] = { 1, 2, 3 };
	int B[] = { 1, 2, 3 };
	return test_filter(stub, SIZE(A), A, SIZE(B), B); 
}

static char* all_tests() {
	mu_run_test(test_sum_0);
	mu_run_test(test_sum_1);
	mu_run_test(test_sum_2);
	mu_run_test(test_sum_3);
	mu_run_test(test_sum_4);
	mu_run_test(test_sum_5);
	mu_run_test(test_sum_6);
	mu_run_test(test_sum_7);
	mu_run_test(test_sum_8);
	mu_run_test(test_sum_9);
	mu_run_test(test_sum_10);
	mu_run_test(test_sum_11);
	mu_run_test(test_sum_12);
	mu_run_test(test_sum_13);
	mu_run_test(test_sum_14);
	mu_run_test(test_sum_15);
	mu_run_test(test_sum_16);
	mu_run_test(test_sum_17);
	mu_run_test(test_is_5_or_more_0);
	mu_run_test(test_is_5_or_more_1);
	mu_run_test(test_filter_0);
	mu_run_test(test_filter_1);
	mu_run_test(test_filter_2);
	mu_run_test(test_filter_3);
	mu_run_test(test_filter_4);
	mu_run_test(test_filter_5);
	mu_run_test(test_filter_6);
	mu_run_test(test_filter_7);
	mu_run_test(test_filter_8);
	mu_run_test(test_filter_9);
	mu_run_test(test_filter_10);
	mu_run_test(test_filter_11);
	mu_run_test(test_filter_12);
	mu_run_test(test_filter_13);
	mu_run_test(test_filter_14);
	mu_run_test(test_filter_15);
	mu_run_test(test_filter_16);
	mu_run_test(test_filter_17);
	mu_run_test(test_filter_18);
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

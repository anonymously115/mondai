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

static char* message_u(unsigned expected, unsigned actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%u> but was: <%u>", expected, actual);
	return msg;
}

static char* message_zu(size_t expected, size_t actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%zu> but was: <%zu>", expected, actual);
	return msg;
}

static char* test_parse_num(const char *str, size_t en, size_t ek) {
	errno = 0;
	size_t N, K;
	mu_assert("Error: expected: <true> but was: <false>", parse_num(str, &N, &K));
	mu_assert(message_zu(en, N), N == en);
	mu_assert(message_zu(ek, K), K == ek);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_num_invalid(const char *str) {
	errno = 0;
	size_t N, K;
	mu_assert("Error: expected: <false> but was: <true>", !parse_num(str, &N, &K));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_num_out_of_range(const char *str) {
	errno = 0;
	size_t N, K;
	mu_assert("Error: expected: <false> but was: <true>", !parse_num(str, &N, &K));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_num_0() { return test_parse_num_invalid(NULL); }
static char* test_parse_num_1() { return test_parse_num_invalid(""); }
static char* test_parse_num_2() { return test_parse_num_invalid("\n"); }
static char* test_parse_num_3() {
	errno = 0;
	size_t K;
	mu_assert("Error: expected: <false> but was: <true>", !parse_num("1 1\n", NULL, &K));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_num_4() {
	errno = 0;
	size_t N;
	mu_assert("Error: expected: <false> but was: <true>", !parse_num("1 1\n", &N, NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_num_5() { return test_parse_num_invalid("1\n"); }
static char* test_parse_num_6() { return test_parse_num_invalid("1 1 0\n"); }
static char* test_parse_num_7() { return test_parse_num_out_of_range("0 1\n"); }
static char* test_parse_num_8() { return test_parse_num_out_of_range("1 0\n"); }
static char* test_parse_num_9() { return test_parse_num("1 1\n", 1, 1); }
static char* test_parse_num_10() { return test_parse_num("1000 1000\n", 1000, 1000); }
static char* test_parse_num_11() { return test_parse_num_out_of_range("1001 1000\n"); }
static char* test_parse_num_12() { return test_parse_num_out_of_range("1000 1001\n"); }

static char* test_parse_age(const char *str, unsigned expected) {
	errno = 0;
	unsigned actual;
	mu_assert("Error: expected: <true> but was: <false>", parse_age(str, &actual));
	mu_assert(message_u(expected, actual), actual == expected);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_age_invalid(const char *str) {
	errno = 0;
	unsigned actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_age(str, &actual));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_age_out_of_range(const char *str) {
	errno = 0;
	unsigned actual;
	mu_assert("Error: expected: <false> but was: <true>", !parse_age(str, &actual));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_age_0() { return test_parse_age_invalid(NULL); }
static char* test_parse_age_1() { return test_parse_age_invalid(""); }
static char* test_parse_age_2() { return test_parse_age_invalid("\n"); }
static char* test_parse_age_3() {
	errno = 0;
	mu_assert("Error: expected: <false> but was: <true>", !parse_age("1\n", NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_age_4() { return test_parse_age_invalid("1 1\n"); }
static char* test_parse_age_5() { return test_parse_age_out_of_range("0\n"); }
static char* test_parse_age_6() { return test_parse_age("1\n", 1); }
static char* test_parse_age_7() { return test_parse_age("100\n", 100); }
static char* test_parse_age_8() { return test_parse_age_out_of_range("101\n"); }

static char* test_parse_order(const char *str, size_t N, size_t en, const char *es, unsigned em) {
	errno = 0;
	size_t n;
	char s[16] = { };
	unsigned m;
	mu_assert("Error: expected: <true> but was: <false>", parse_order(str, N, &n, s, &m));
	mu_assert(message_zu(en, n), n == en);
	mu_assert(message_s(es, s), !strcmp(s, es));
	if (300 <= em && em <= 5000) {
		mu_assert(message_u(em, m), m == em);
	}
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_parse_order_invalid(const char *str, size_t N) {
	errno = 0;
	size_t n;
	char s[16] = { };
	unsigned m;
	mu_assert("Error: expected: <false> but was: <true>", !parse_order(str, N, &n, s, &m));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}

static char* test_parse_order_out_of_range(const char *str, size_t N) {
	errno = 0;
	size_t n;
	char s[16] = { };
	unsigned m;
	mu_assert("Error: expected: <false> but was: <true>", !parse_order(str, N, &n, s, &m));
	mu_assert(message_d(ERANGE, errno), errno == ERANGE);
	return 0;
}

static char* test_parse_order_0() { return test_parse_order_invalid(NULL, 1); }
static char* test_parse_order_1() { return test_parse_order_invalid("", 1); }
static char* test_parse_order_2() { return test_parse_order_invalid("\n", 1); }
static char* test_parse_order_3() {
	errno = 0;
	char s[16] = { };
	unsigned m;
	mu_assert("Error: expected: <false> but was: <true>", !parse_order("1 0\n", 1, NULL, s, &m));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_order_4() {
	errno = 0;
	size_t n;
	unsigned m;
	mu_assert("Error: expected: <false> but was: <true>", !parse_order("1 0\n", 1, &n, NULL, &m));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_order_5() {
	errno = 0;
	size_t n;
	char s[16] = { };
	mu_assert("Error: expected: <false> but was: <true>", !parse_order("1 0\n", 1, &n, s, NULL));
	mu_assert(message_d(EINVAL, errno), errno == EINVAL);
	return 0;
}
static char* test_parse_order_6() { return test_parse_order_invalid("1\n", 1); }
static char* test_parse_order_7() { return test_parse_order_out_of_range("0 0\n", 1); }
static char* test_parse_order_8() { return test_parse_order_out_of_range("2 0\n", 1); }
static char* test_parse_order_9() { return test_parse_order_invalid("1 _\n", 1); }
static char* test_parse_order_10() { return test_parse_order_invalid("1 _ 500\n", 1); }
static char* test_parse_order_11() { return test_parse_order_invalid("1 food\n", 1); }
static char* test_parse_order_12() { return test_parse_order_out_of_range("1 food 299\n", 1); }
static char* test_parse_order_13() { return test_parse_order("1 food 300\n", 1, 1, "food", 300); }
static char* test_parse_order_14() { return test_parse_order("1 food 5000\n", 1, 1, "food", 5000); }
static char* test_parse_order_15() { return test_parse_order_out_of_range("1 food 5001\n", 1); }
static char* test_parse_order_16() { return test_parse_order_invalid("1 softdrink\n", 1); }
static char* test_parse_order_17() { return test_parse_order_out_of_range("1 softdrink 299\n", 1); }
static char* test_parse_order_18() { return test_parse_order("1 softdrink 300\n", 1, 1, "softdrink", 300); }
static char* test_parse_order_19() { return test_parse_order("1 softdrink 5000\n", 1, 1, "softdrink", 5000); }
static char* test_parse_order_20() { return test_parse_order_out_of_range("1 softdrink 5001\n", 1); }
static char* test_parse_order_21() { return test_parse_order_invalid("1 alcohol\n", 1); }
static char* test_parse_order_22() { return test_parse_order_out_of_range("1 alcohol 299\n", 1); }
static char* test_parse_order_23() { return test_parse_order("1 alcohol 300\n", 1, 1, "alcohol", 300); }
static char* test_parse_order_24() { return test_parse_order("1 alcohol 5000\n", 1, 1, "alcohol", 5000); }
static char* test_parse_order_25() { return test_parse_order_out_of_range("1 alcohol 5001\n", 1); }
static char* test_parse_order_26() { return test_parse_order("1 0\n", 1, 1, "0", 0); }
static char* test_parse_order_27() { return test_parse_order_invalid("1 0 500\n", 1); }
static char* test_parse_order_28() { return test_parse_order("1 A\n", 1, 1, "A", 0); }
static char* test_parse_order_29() { return test_parse_order_invalid("1 A 500\n", 1); }

static char* all_tests() {
    mu_run_test(test_parse_num_0);
    mu_run_test(test_parse_num_1);
    mu_run_test(test_parse_num_2);
    mu_run_test(test_parse_num_3);
    mu_run_test(test_parse_num_4);
    mu_run_test(test_parse_num_5);
    mu_run_test(test_parse_num_6);
    mu_run_test(test_parse_num_7);
    mu_run_test(test_parse_num_8);
    mu_run_test(test_parse_num_9);
    mu_run_test(test_parse_num_10);
    mu_run_test(test_parse_num_11);
    mu_run_test(test_parse_num_12);
    mu_run_test(test_parse_age_0);
    mu_run_test(test_parse_age_1);
    mu_run_test(test_parse_age_2);
    mu_run_test(test_parse_age_3);
    mu_run_test(test_parse_age_4);
    mu_run_test(test_parse_age_5);
    mu_run_test(test_parse_age_6);
    mu_run_test(test_parse_age_7);
    mu_run_test(test_parse_age_8);
	mu_run_test(test_parse_order_0);
	mu_run_test(test_parse_order_1);
	mu_run_test(test_parse_order_2);
	mu_run_test(test_parse_order_3);
	mu_run_test(test_parse_order_4);
	mu_run_test(test_parse_order_5);
	mu_run_test(test_parse_order_6);
	mu_run_test(test_parse_order_7);
	mu_run_test(test_parse_order_8);
	mu_run_test(test_parse_order_9);
	mu_run_test(test_parse_order_10);
	mu_run_test(test_parse_order_11);
	mu_run_test(test_parse_order_12);
	mu_run_test(test_parse_order_13);
	mu_run_test(test_parse_order_14);
	mu_run_test(test_parse_order_15);
	mu_run_test(test_parse_order_16);
	mu_run_test(test_parse_order_17);
	mu_run_test(test_parse_order_18);
	mu_run_test(test_parse_order_19);
	mu_run_test(test_parse_order_20);
	mu_run_test(test_parse_order_21);
	mu_run_test(test_parse_order_22);
	mu_run_test(test_parse_order_23);
	mu_run_test(test_parse_order_24);
	mu_run_test(test_parse_order_25);
	mu_run_test(test_parse_order_26);
	mu_run_test(test_parse_order_27);
	mu_run_test(test_parse_order_28);
	mu_run_test(test_parse_order_29);
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

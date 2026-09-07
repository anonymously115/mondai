#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "minunit.h"
#include "Array.h"

#define SIZE(A) (sizeof(A) / sizeof(*A))

int tests_run = 0;

#if 0
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

#endif

static char* message_d(int expected, int actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%d> but was: <%d>", expected, actual);
	return msg;
}

static char* message_p(const void *expected, const void *actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%p> but was: <%p>", expected, actual);
	return msg;
}

static char* test_constructor(size_t n, const size_t A[]) {
    Array *array = new_Array(n, A);
    mu_assert("Error: expected: <not null> but was: <null>", array);
    del_Array(&array);
    mu_assert("Error: expected: <null> but was: <not null>", !array);
    return 0;
}

static char* test_constructor_0() {
	return test_constructor(0, NULL);
}

static char* test_search(size_t n, const size_t A[], size_t k, const size_t *expected) {
	errno = 0;
    Array *array = new_Array(n, A);
    const size_t *actual = array->search(array, k);
    del_Array(&array);
    mu_assert(message_p(expected, actual), actual == expected);
    mu_assert(message_d(0, errno), errno == 0);
    return 0;
}

static char* test_search_0 () {
	return test_search(0, NULL, 0, NULL);
}

static char* test_search_1() {
	errno = 0;
	size_t A[] = { 0 };
    Array *array = new_Array(1, A);
    const size_t *actual = array->search(NULL, 0);
    del_Array(&array);
    mu_assert(message_p(NULL, actual), actual == NULL);
    mu_assert(message_d(EINVAL, errno), errno == EINVAL);
    return 0;
}

static char* test_search_2 () {
	size_t A[] = { 0 };
	return test_search(SIZE(A), A, 0, A);
}

static char* test_search_3 () {
	size_t A[] = { 1 };
	return test_search(SIZE(A), A, 0, NULL);
}

static char* test_search_4 () {
	size_t A[] = { 0 };
	return test_search(SIZE(A), A, 1, NULL);
}

static char* test_search_5 () {
	size_t A[] = { 1, 2 };
	return test_search(SIZE(A), A, 0, NULL);
}

static char* test_search_6 () {
	size_t A[] = { 1, 2 };
	return test_search(SIZE(A), A, 1, A);
}

static char* test_search_7 () {
	size_t A[] = { 1, 2 };
	return test_search(SIZE(A), A, 2, A + 1);
}

static char* test_search_8 () {
	size_t A[] = { 1, 2 };
	return test_search(SIZE(A), A, 3, NULL);
}

static char* test_search_9 () {
	size_t A[] = { 0, 0 };
	return test_search(SIZE(A), A, 0, A);
}

static char* test_search_10 () {
	size_t A[] = { 1, 2, 3 };
	return test_search(SIZE(A), A, 0, NULL);
}

static char* test_search_11 () {
	size_t A[] = { 1, 2, 3 };
	return test_search(SIZE(A), A, 1, A);
}

static char* test_search_12 () {
	size_t A[] = { 1, 2, 3 };
	return test_search(SIZE(A), A, 2, A + 1);
}

static char* test_search_13 () {
	size_t A[] = { 1, 2, 3 };
	return test_search(SIZE(A), A, 3, A + 2);
}

static char* test_search_14 () {
	size_t A[] = { 1, 2, 3 };
	return test_search(SIZE(A), A, 4, NULL);
}

static char* test_search_15 () {
	size_t A[] = { 0, 0, 1 };
	return test_search(SIZE(A), A, 0, A);
}

static char* test_search_16 () {
	size_t A[] = { 0, 1, 0 };
	return test_search(SIZE(A), A, 0, A);
}

static char* test_search_17 () {
	size_t A[] = { 1, 0, 0 };
	return test_search(SIZE(A), A, 0, A + 1);
}

static char* test_search_18 () {
	size_t A[] = { 0, 0, 0 };
	return test_search(SIZE(A), A, 0, A);
}

static char* all_tests() {
    mu_run_test(test_constructor_0);
    mu_run_test(test_search_0);
    mu_run_test(test_search_1);
    mu_run_test(test_search_2);
    mu_run_test(test_search_3);
    mu_run_test(test_search_4);
    mu_run_test(test_search_5);
    mu_run_test(test_search_6);
    mu_run_test(test_search_7);
    mu_run_test(test_search_8);
    mu_run_test(test_search_9);
    mu_run_test(test_search_10);
    mu_run_test(test_search_11);
    mu_run_test(test_search_12);
    mu_run_test(test_search_13);
    mu_run_test(test_search_14);
    mu_run_test(test_search_15);
    mu_run_test(test_search_16);
    mu_run_test(test_search_17);
    mu_run_test(test_search_18);
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

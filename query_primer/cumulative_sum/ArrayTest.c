#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include "minunit.h"
#include "util.h"
#include "Array.h"
#define OUT "out.txt"

#define TRY(fun, ptr) \
	do { \
		if (!(fun)) { \
			del_Array(ptr); \
			return "Unknown error"; \
		} \
	} while (0)

int tests_run = 0;

static char* message_d(int expected, int actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%d> but was: <%d>", expected, actual);
	return msg;
}

static char* test() {
    Array *array = new_Array(0, NULL);
    mu_assert("Error: expected: <not null> but was: <null>", array);
    del_Array(&array);
    mu_assert("Error: expected: <null> but was: <not null>", !array);
    return 0;
}

static char* test_cumulative_sum(size_t n, const int A[n], const int expected[n + 1]) {
    Array *array = NULL;
    TRY(array = new_Array(n, A), NULL);
    bool result[n + 2];
    int actual[n + 2];
    int errnum[n + 2];
    for (size_t i = 0; i <= n + 1; i++) {
        result[i] = array->cumulative_sum(array, i, &actual[i]);
        errnum[i] = errno;
    }
    errno = 0;
    del_Array(&array);
    for (size_t i = 0; i <= n; i++) {
        mu_assert("Error: expected: <true> but was: <false>", result[i]);
        mu_assert(message_d(expected[i], actual[i]), actual[i] == expected[i]);
        mu_assert(message_d(0, errnum[i]), errnum[i] == 0);
    }
    mu_assert("Error: expected: <false> but was: <true>", !result[n + 1]);
    mu_assert(message_d(ERANGE, errnum[n + 1]), errnum[n + 1] == ERANGE);
    return 0;
}

static char* test_cumulative_sum_0() { return test_cumulative_sum(0, NULL, (const int[]){0}); }

static char* test_cumulative_sum_1() {
    errno = 0;
    Array *array = NULL;
    TRY(array = new_Array(0, NULL), NULL);
    int actual;
    bool result = array->cumulative_sum(NULL, 0, &actual);
    del_Array(&array);
    mu_assert("Error: expected: <false> but was: <true>", !result);
    mu_assert(message_d(EINVAL, errno), errno == EINVAL);
    return 0;
}

static char* test_cumulative_sum_2() {
    errno = 0;
    Array *array = NULL;
    TRY(array = new_Array(0, NULL), NULL);
    bool result = array->cumulative_sum(array, 0, NULL);
    del_Array(&array);
    mu_assert("Error: expected: <false> but was: <true>", !result);
    mu_assert(message_d(EINVAL, errno), errno == EINVAL);
    return 0;
}

static char* test_cumulative_sum_3() {
    const size_t N = 1<<16;
    int A[N - 1];
    int S[N];
    S[0] = 0;
    for (size_t i = 1; i < N; i++) {
        A[i - 1] = (int) i;
        if (i % 2) {
            S[i] = (int) ((i + 1) / 2 * i);
        } else {
            S[i] = (int) (i / 2 * (i + 1));
        }
    }
    return test_cumulative_sum(N - 1, A, S);
}

static char* test_cumulative_sum_4() {
    const size_t N = 1<<16;
    int A[N - 1];
    int S[N];
    S[0] = 0;
    for (size_t i = 1; i < N; i++) {
        A[i - 1] = -(int)i;
        if (i % 2) {
            S[i] = -(int) ((i + 1) / 2 * i);
        } else {
            S[i] = -(int) (i / 2 * (i + 1));
        }
    }
    return test_cumulative_sum(N - 1, A, S);
}

static char* all_tests() {
    mu_run_test(test);
    mu_run_test(test_cumulative_sum_0);
    mu_run_test(test_cumulative_sum_1);
    mu_run_test(test_cumulative_sum_2);
    mu_run_test(test_cumulative_sum_3);
    mu_run_test(test_cumulative_sum_4);
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
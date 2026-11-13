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

static char* test_interval_sum(size_t n, const int A[n], const int expected[n + 1][n + 1]) {
    Array *array = NULL;
    TRY(array = new_Array(n, A), NULL);
    bool result[n + 2][n + 2];
    int actual[n + 2][n + 2];
    int errnum[n + 2][n + 2];
    for (size_t i = 0; i <= n + 1; i++) {
        for (size_t j = 0; j <= n + 1; j++) {
            result[i][j] = array->interval_sum(array, i, j, &actual[i][j]);
            errnum[i][j] = errno;
        }
    }
    errno = 0;
    del_Array(&array);
    for (size_t i = 0; i <= n; i++) {
        for (size_t j = 0; j < i; j++) {
            mu_assert("Error: expected: <false> but was: <true>", !result[i][j]);
            mu_assert(message_d(ERANGE, errnum[i][j]), errnum[i][j] == ERANGE);
        }
        for (size_t j = i; j <= n; j++) {
            mu_assert("Error: expected: <true> but was: <false>", result[i][j]);
            mu_assert(message_d(expected[i][j], actual[i][j]), actual[i][j] == expected[i][j]);
            mu_assert(message_d(0, errnum[i][j]), errnum[i][j] == 0);
        }
        mu_assert("Error: expected: <false> but was: <true>", !result[i][n + 1]);
        mu_assert(message_d(ERANGE, errnum[i][n + 1]), errnum[i][n + 1] == ERANGE);
    }
    for (size_t j = 0; j <= n + 1; j++) {
        mu_assert("Error: expected: <false> but was: <true>", !result[n + 1][j]);
        mu_assert(message_d(ERANGE, errnum[n + 1][j]), errnum[n + 1][j] == ERANGE);
    }
    return 0;
}

static char* test_interval_sum_0() {
	int S[1][1] = { { 0 } };
    return test_interval_sum(0, NULL, S);
}

static char* test_interval_sum_1() {
    errno = 0;
    Array *array = NULL;
    TRY(array = new_Array(0, NULL), NULL);
    int actual;
    bool result = array->interval_sum(NULL, 0, 0, &actual);
    del_Array(&array);
    mu_assert("Error: expected: <false> but was: <true>", !result);
    mu_assert(message_d(EINVAL, errno), errno == EINVAL);
    return 0;
}

static char* test_interval_sum_2() {
    errno = 0;
    Array *array = NULL;
    TRY(array = new_Array(0, NULL), NULL);
    bool result = array->interval_sum(array, 0, 0, NULL);
    del_Array(&array);
    mu_assert("Error: expected: <false> but was: <true>", !result);
    mu_assert(message_d(EINVAL, errno), errno == EINVAL);
    return 0;
}

static char* test_interval_sum_3() {
	const size_t N = 1 << 8;
	int A[N - 1];
	for (size_t i = 1; i < N; i++) {
		A[i - 1] = (int) (i * i * i);
	}
	int S[N][N];
	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < N; j++) {
			S[i][j] = (int) (j * (j + 1) + i * (i + 1)) / 2
					* ((int) (j * (j + 1)) - (int) (i * (i + 1))) / 2;
		}
	}
    return test_interval_sum(N - 1, A, S);
}

static char* test_interval_sum_4() {
	const size_t N = 1 << 8;
	int A[N - 1];
	for (size_t i = 1; i < N; i++) {
		A[i - 1] = -(int) (i * i * i);
	}
    int S[N][N];
	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < N; j++) {
			S[i][j] = (int) (i * (i + 1) + j * (j + 1)) / 2
					* ((int) (i * (i + 1)) - (int) (j * (j + 1))) / 2;
		}
	}
    return test_interval_sum(N - 1, A, S);
}

static char* all_tests() {
    mu_run_test(test);
    mu_run_test(test_interval_sum_0);
    mu_run_test(test_interval_sum_1);
    mu_run_test(test_interval_sum_2);
    mu_run_test(test_interval_sum_3);
    mu_run_test(test_interval_sum_4);
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
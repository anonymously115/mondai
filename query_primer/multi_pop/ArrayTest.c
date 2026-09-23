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

static char* test() {
    Array *array = new_Array(0, NULL);
    mu_assert("Error: expected: <not null> but was: <null>", array);
    del_Array(&array);
    mu_assert("Error: expected: <null> but was: <not null>", !array);
    return 0;
}

static char* test_pop(size_t n, int A[n]) {
    Array *array = NULL;
    TRY(array = new_Array(n, A), &array);
    int a[n + 1];
    bool b[n + 1];
    for (size_t i = 0; i < n; i++) {
        b[i] = array->pop(array, &a[i]);
    }
    b[n] = array->pop(array, &a[n]);
    del_Array(&array);
    for (size_t i = 0; i < n; i++) {
        mu_assert("Error: expected: <true> but was: <false>", b[i]);
        mu_assert(message_d(A[i], a[i]), a[i] == A[i]);
    }
    mu_assert("Error: expected: <false> but was: <true>", !b[n]);
    return 0;
}

static char* test_pop_0() { return test_pop(0, NULL); }
static char* test_pop_1() {
    int A[] = { 0 };
    size_t n = sizeof(A) / sizeof(*A);
    Array *array = NULL;
    TRY(array = new_Array(n, A), &array);
    int a;
    bool b = array->pop(NULL, &a);
    del_Array(&array);
    mu_assert("Error: expected: <false> but was: <true>", !b);
    return 0;
}
static char* test_pop_2() {
    int A[] = { 0 };
    size_t n = sizeof(A) / sizeof(*A);
    Array *array = NULL;
    TRY(array = new_Array(n, A), &array);
    bool b = array->pop(array, NULL);
    del_Array(&array);
    mu_assert("Error: expected: <true> but was: <false>", b);
    return 0;
}

static char* test_show(size_t n, int A[n]) {
    Array *array = NULL;
    TRY(array = new_Array(n, A), &array);
    FILE *file = NULL;
    TRY(file = freopen(OUT, "w", stdout), &array);
    bool result = array->show(array);
#if defined(_WIN32) || defined(_WIN64)
    TRY(file = freopen("con", "w", stdout), &array);
#else
    TRY(file = freopen("/dev/tty", "w", stdout), &array);
#endif
    del_Array(&array);
    mu_assert("Error: expected: <true> but was: <false>", result);
    if (!(file = fopen(OUT, "r"))) {
        return "Unknown error";
    }
    int a[n ? n : 1];
    char s[22];
    for (size_t i = 0; i < n; i++) {
        if (!parse_int(chomp(fgets(s, sizeof(s), file)), a + i)) {
            fclose(file);
            return "Unknown error";
        }
    }
    if (fgets(s, sizeof(s), file)) {
        fclose(file);
        return "Unknown error";
    }
    fclose(file);
    remove(OUT);
    for (size_t i = 0; i < n; i++) {
        mu_assert(message_d(A[i], a[i]), a[i] == A[i]);
    }
    return 0;
}

static char* test_show_0() { return test_show(0, NULL); }
static char* test_show_1() {
    Array *array = NULL;
    TRY(array = new_Array(0, NULL), &array);
    bool result = array->show(NULL);
    del_Array(&array);
    mu_assert("Error: expected: <false> but was: <true>", !result);
    return 0;
}
static char* test_show_2() {
    Array *array = NULL;
    TRY(array = new_Array(0, NULL), &array);
    FILE *file = NULL;
#if defined(_WIN32) || defined(_WIN64)
    TRY(file = freopen("nul", "w", stdout), &array);
#else
    TRY(file = freopen("/dev/null", "w", stdout), &array);
#endif
    bool b[11];
    for (size_t i = 0; i < 10; i++) {
        b[i] = array->show(array);
    }
    b[10] = array->show(array);
#if defined(_WIN32) || defined(_WIN64)
    TRY(file = freopen("con", "w", stdout), &array);
#else
    TRY(file = freopen("/dev/tty", "w", stdout), &array);
#endif
    del_Array(&array);
    for (size_t i = 0; i < 10; i++) {
        mu_assert("Error: expected: <true> but was: <false>", b[i]);
    }
    mu_assert("Error: expected: <false> but was: <true>", !b[10]);
    return 0;
}

static char* all_tests() {
    mu_run_test(test);
    mu_run_test(test_pop_0);
    mu_run_test(test_pop_1);
    mu_run_test(test_pop_2);
    mu_run_test(test_show_0);
    mu_run_test(test_show_1);
    mu_run_test(test_show_2);
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

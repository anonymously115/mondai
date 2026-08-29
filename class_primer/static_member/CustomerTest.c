#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include "minunit.h"
#include "Customer.h"
#define OUT "out.txt"

#define TRY(fun, ptr) \
	do { \
		if (!(fun)) { \
			del_Customer(ptr); \
			return "Unknown error"; \
		} \
	} while (0)

int tests_run = 0;

static bool get_amount(const Customer *customer, unsigned *amount) {
	errno = 0;
	if (!customer || !customer->_customer || !amount) {
		errno = EINVAL;
		return false;
	}
	_Customer *ptr = customer->_customer;
	*amount = *(unsigned*) ptr;
	return true;
}

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

static char* test_constructor() {
	errno = 0;
    Customer *customer = new_Customer();
    mu_assert("Error: expected: <not null> but was: <null>", customer);
    del_Customer(&customer);
    mu_assert("Error: expected: <null> but was: <not null>", !customer);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_food_0() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_food(customer, m);
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(m, amount), amount == m);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_food_1() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_food(NULL, m);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_food_2() {
	errno = 0;
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	TRY(customer->take_food(customer, UINT_MAX), &customer);
	bool result = customer->take_food(customer, 1);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(UINT_MAX, amount), amount == UINT_MAX);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_food_3() {
	errno = 0;
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	TRY(customer->take_food(customer, 1), &customer);
	bool result = customer->take_food(customer, UINT_MAX);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(1, amount), amount == 1);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_softdrink_0() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_softdrink(customer, m);
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(m, amount), amount == m);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_softdrink_1() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_softdrink(NULL, m);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_softdrink_2() {
	errno = 0;
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	TRY(customer->take_softdrink(customer, UINT_MAX), &customer);
	bool result = customer->take_softdrink(customer, 1);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(UINT_MAX, amount), amount == UINT_MAX);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_softdrink_3() {
	errno = 0;
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	TRY(customer->take_softdrink(customer, 1), &customer);
	bool result = customer->take_softdrink(customer, UINT_MAX);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(1, amount), amount == 1);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_alcohol_0() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_alcohol(customer, m);
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_alcohol_1() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_alcohol(NULL, m);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_beer_0() {
	errno = 0;
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_beer(customer);
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_beer_1() {
	errno = 0;
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	bool result = customer->take_beer(NULL);
	int e = errno;
	unsigned amount;
	TRY(get_amount(customer, &amount), &customer);
	del_Customer(&customer);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_accounting_0() {
	errno = 0;
	size_t before = get_num_of_left();
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	unsigned m = (unsigned) rand();
	char s[16] = { };
	snprintf(s, sizeof(s), "%u\n", m);
	TRY(customer->take_softdrink(customer, m), &customer);
	TRY(freopen(OUT, "w", stdout), &customer);
	bool result = customer->accounting(customer);
	#if defined(_WIN32) || defined(_WIN64)
		TRY(freopen("con", "w", stdout), &customer);
	#else
		TRY(freopen("/dev/tty", "w", stdout), &customer);
	#endif
	del_Customer(&customer);
	size_t after = get_num_of_left();
	FILE *file = NULL;
	TRY(file = fopen(OUT, "r"), NULL);
	char amount[16] = { };
	if (!fgets(amount, sizeof(amount), file)) {
		fclose(file);
		return "Unknown error";
	}
	if (fclose(file) != 0) {
		return "Unknown error";
	}
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_s(s, amount), !strcmp(amount, s));
	mu_assert(message_zu(before + 1, after), after == before + 1);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_accounting_1() {
	errno = 0;
	size_t before = get_num_of_left();
	Customer *customer = NULL;
	TRY(customer = new_Customer(), &customer);
	unsigned m = (unsigned) rand();
	TRY(customer->take_softdrink(customer, m), &customer);
	TRY(freopen(OUT, "w", stdout), &customer);
	bool result = customer->accounting(NULL);
	int e = errno;
	#if defined(_WIN32) || defined(_WIN64)
		TRY(freopen("con", "w", stdout), &customer);
	#else
		TRY(freopen("/dev/tty", "w", stdout), &customer);
	#endif
	del_Customer(&customer);
	size_t after = get_num_of_left();
	FILE *file = NULL;
	TRY(file = fopen(OUT, "r"), NULL);
	char amount[16] = { };
	if (fgets(amount, sizeof(amount), file)) {
		fclose(file);
		return message_s("", amount);
	}
	if (fclose(file) != 0) {
		return "Unknown error";
	}
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_zu(before, after), after == before);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* all_tests() {
    mu_run_test(test_constructor);
    mu_run_test(test_take_food_0);
    mu_run_test(test_take_food_1);
    mu_run_test(test_take_food_2);
    mu_run_test(test_take_food_3);
    mu_run_test(test_take_softdrink_0);
    mu_run_test(test_take_softdrink_1);
    mu_run_test(test_take_softdrink_2);
    mu_run_test(test_take_softdrink_3);
    mu_run_test(test_take_alcohol_0);
    mu_run_test(test_take_alcohol_1);
    mu_run_test(test_take_beer_0);
    mu_run_test(test_take_beer_1);
    mu_run_test(test_take_accounting_0);
    mu_run_test(test_take_accounting_1);
    return 0;
}

int main() {
	srand((unsigned) time(NULL));
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

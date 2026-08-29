#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "minunit.h"
#include "Adult.h"

#define OUT "out.txt"

#define TRY(fun, ptr) \
	do { \
		if (!(fun)) { \
			del_Adult(ptr); \
			return "Unknown error"; \
		} \
	} while (0)

int tests_run = 0;

static bool get_amount(const Adult *adult, unsigned *amount) {
	errno = 0;
	if (!adult || !adult->_adult || !amount) {
		errno = EINVAL;
		return false;
	}
	_Customer *customer = adult->_customer._customer;
	*amount = *(unsigned*) customer;
	return true;
}

static bool get_alcohol(const Adult *adult, bool *alcohol) {
	errno = 0;
	if (!adult || !adult->_adult || !alcohol) {
		errno = EINVAL;
		return false;
	}
	_Adult *ptr = adult->_adult;
	*alcohol = *(bool*) ptr;
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
    Adult *adult = new_Adult();
    mu_assert("Error: expected: <not null> but was: <null>", adult);
    del_Adult(&adult);
    mu_assert("Error: expected: <null> but was: <not null>", !adult);
	return 0;
}

static char* test_take_food_0() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_food(customer, 199);
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(199, amount), amount == 199);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_food_1() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_food(NULL, 200);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_food_2() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_beer(customer), &adult);
	bool result = customer->take_food(customer, 199);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(500, amount), amount == 500);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_food_3() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_beer(customer), &adult);
	bool result = customer->take_food(customer, 200);
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(500, amount), amount == 500);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_food_4() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_food(customer, UINT_MAX), &adult);
	bool result = customer->take_food(customer, 1);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(UINT_MAX, amount), amount == UINT_MAX);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_food_5() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_food(customer, 1), &adult);
	bool result = customer->take_food(customer, UINT_MAX);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(1, amount), amount == 1);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_softdrink_0() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_softdrink(customer, m);
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(m, amount), amount == m);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_softdrink_1() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_softdrink(NULL, m);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_softdrink_2() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_softdrink(customer, UINT_MAX), &adult);
	bool result = customer->take_softdrink(customer, 1);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(UINT_MAX, amount), amount == UINT_MAX);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_softdrink_3() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_softdrink(customer, 1), &adult);
	bool result = customer->take_softdrink(customer, UINT_MAX);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(1, amount), amount == 1);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_alcohol_0() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_alcohol(customer, m);
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(m, amount), amount == m);
	mu_assert("Error: expected: <true> but was: <false>", alcohol);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_alcohol_1() {
	errno = 0;
	unsigned m = (unsigned) (rand() % 4701 + 300);
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_alcohol(NULL, m);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert("Error: expected: <false> but was: <true>", !alcohol);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_alcohol_2() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_food(customer, UINT_MAX), &adult);
	bool result = customer->take_alcohol(customer, 1);
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(UINT_MAX, amount), amount == UINT_MAX);
	mu_assert("Error: expected: <false> but was: <true>", !alcohol);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_alcohol_3() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_food(customer, 1), &adult);
	bool result = customer->take_alcohol(customer, UINT_MAX);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(1, amount), amount == 1);
	mu_assert("Error: expected: <false> but was: <true>", !alcohol);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_beer_0() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_beer(customer);
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_u(500, amount), amount == 500);
	mu_assert("Error: expected: <true> but was: <false>", alcohol);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_take_beer_1() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	bool result = customer->take_beer(NULL);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(0, amount), amount == 0);
	mu_assert("Error: expected: <false> but was: <true>", !alcohol);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_take_beer_2() {
	errno = 0;
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	TRY(customer->take_food(customer, UINT_MAX - 499), &adult);
	bool result = customer->take_beer(customer);
	int e = errno;
	unsigned amount;
	TRY(get_amount(adult, &amount), &adult);
	bool alcohol;
	TRY(get_alcohol(adult, &alcohol), &adult);
	del_Adult(&adult);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_u(UINT_MAX - 499, amount), amount == UINT_MAX - 499);
	mu_assert("Error: expected: <false> but was: <true>", !alcohol);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_take_accounting_0() {
	errno = 0;
	size_t before = get_num_of_left();
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	unsigned m = (unsigned) rand();
	char s[16] = { };
	snprintf(s, sizeof(s), "%u\n", m);
	TRY(customer->take_softdrink(customer, m), &adult);
	TRY(freopen(OUT, "w", stdout), &adult);
	bool result = customer->accounting(customer);
	#if defined(_WIN32) || defined(_WIN64)
		TRY(freopen("con", "w", stdout), &adult);
	#else
		TRY(freopen("/dev/tty", "w", stdout), &adult);
	#endif
	del_Adult(&adult);
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
	Adult *adult = NULL;
	TRY(adult = new_Adult(), &adult);
	Customer *customer = (Customer*) adult;
	unsigned m = (unsigned) rand();
	TRY(customer->take_softdrink(customer, m), &adult);
	TRY(freopen(OUT, "w", stdout), &adult);
	bool result = customer->accounting(NULL);
	int e = errno;
	#if defined(_WIN32) || defined(_WIN64)
		TRY(freopen("con", "w", stdout), &adult);
	#else
		TRY(freopen("/dev/tty", "w", stdout), &adult);
	#endif
	del_Adult(&adult);
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
    mu_run_test(test_take_food_4);
    mu_run_test(test_take_food_5);
    mu_run_test(test_take_softdrink_0);
    mu_run_test(test_take_softdrink_1);
    mu_run_test(test_take_softdrink_2);
    mu_run_test(test_take_softdrink_3);
    mu_run_test(test_take_alcohol_0);
    mu_run_test(test_take_alcohol_1);
    mu_run_test(test_take_alcohol_2);
    mu_run_test(test_take_alcohol_3);
    mu_run_test(test_take_beer_0);
    mu_run_test(test_take_beer_1);
    mu_run_test(test_take_beer_2);
    mu_run_test(test_take_accounting_0);
    mu_run_test(test_take_accounting_1);
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "minunit.h"
#include "Pub.h"

#define TRY(fun, n, arr, ptr) \
	do { \
		if (!(fun)) { \
			size_t _ = (n); \
			while (_--) { \
				del_Customer((arr) + _); \
			} \
			del_Pub(ptr); \
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

#endif

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

static char* message_p(const void* expected, const void* actual) {
	static char msg[72];
	snprintf(msg, sizeof(msg), "Error: expected: <%p> but was: <%p>", expected, actual);
	return msg;
}

static char* test_constructor() {
	errno = 0;
    Pub *pub = new_Pub();
    mu_assert("Error: expected: <not null> but was: <null>", pub);
    del_Pub(&pub);
    mu_assert("Error: expected: <null> but was: <not null>", !pub);
	return 0;
}

static bool get_customers(const Pub *pub, size_t *count, Customer *customers[]) {
	if (!pub || !count || !customers) {
		errno = EINVAL;
		return false;
	}
	*count = *(size_t*) pub->_pub;
	memcpy(customers, (void*) pub->_pub + sizeof(size_t), *count * sizeof(Customer*));
	return true;
}

static char* test_add_customer_0() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	void *expected = (void*) customer;
	bool result = pub->add_customer(pub, customer);
	size_t count = 0;
	Customer *customers[1];
	TRY(get_customers(pub, &count, customers), 1, &customer, &pub);
	void *actual = (void*) customers[0];
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_zu(1, count), count == 1);
	mu_assert(message_p(expected, actual), expected == actual);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_add_customer_1() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	bool result = pub->add_customer(NULL, customer);
	int e = errno;
	size_t count = 0;
	Customer *customers[1];
	TRY(get_customers(pub, &count, customers), 1, &customer, &pub);
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_zu(0, count), count == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_add_customer_2() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	bool result = pub->add_customer(pub, NULL);
	int e = errno;
	size_t count = 0;
	Customer *customers[1];
	TRY(get_customers(pub, &count, customers), 1, &customer, &pub);
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_zu(0, count), count == 0);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_query_0() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(NULL, 0, "softdrink", rand() % 4701 + 300);
	int e = errno;
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_query_1() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	bool result = pub->query(pub, 0, "softdrink", rand() % 4701 + 300);
	int e = errno;
	del_Pub(&pub);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_d(ERANGE, e), e == ERANGE);
	return 0;
}

static char* test_query_2() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, NULL, rand() % 4701 + 300);
	int e = errno;
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_query_3() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, "", rand() % 4701 + 300);
	int e = errno;
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <false> but was: <true>", !result);
	mu_assert(message_d(EINVAL, e), e == EINVAL);
	return 0;
}

static char* test_query_4() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, "food", rand() % 4701 + 300);
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_query_5() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, "softdrink", rand() % 4701 + 300);
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_query_6() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, "alcohol", rand() % 4701 + 300);
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_query_7() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, "0", rand());
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* test_query_8() {
	Pub *pub = NULL;
	TRY(pub = new_Pub(), 0, NULL, NULL);
	Customer *customer = NULL;
	TRY(customer = new_Customer(), 0, NULL, &pub);
	TRY(pub->add_customer(pub, customer), 1, &customer, &pub);
	bool result = pub->query(pub, 0, "A", rand());
	del_Customer(&customer);
	del_Pub(&pub);
	mu_assert("Error: expected: <true> but was: <false>", result);
	mu_assert(message_d(0, errno), errno == 0);
	return 0;
}

static char* all_tests() {
    mu_run_test(test_constructor);
    mu_run_test(test_add_customer_0);
    mu_run_test(test_add_customer_1);
    mu_run_test(test_add_customer_2);
    mu_run_test(test_query_0);
    mu_run_test(test_query_1);
    mu_run_test(test_query_2);
    mu_run_test(test_query_3);
    mu_run_test(test_query_4);
    mu_run_test(test_query_5);
    mu_run_test(test_query_6);
    mu_run_test(test_query_7);
    mu_run_test(test_query_8);
    return 0;
}

int main() {
	srand((unsigned)time(NULL));
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

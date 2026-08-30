#include <stdlib.h>
#include "Customer.h"

struct __Customer {
    unsigned amount;
};

static bool stub1(Customer* self, unsigned amount) {
    (void) amount;
    return !!self;
}

static bool take_beer(Customer* self) {
    return !!self;
}

static bool accounting(const Customer* self) {
    return !!self;
}

Customer* new_Customer(void) {
    Customer *customer = (Customer*) malloc(sizeof(Customer));
    if (!customer) {
        return NULL;
    }
    customer->_customer = (_Customer*) malloc(sizeof(_Customer));
    if (!(customer->_customer)) {
        FREE(customer);
        return NULL;
    }
    customer->take_food = stub1;
	customer->take_softdrink = stub1;
	customer->take_alcohol = stub1;
	customer->take_beer = take_beer;
	customer->accounting = accounting;
    return customer;
}

void del_Customer(Customer** customer) {
    if (customer && *customer) {
        FREE((*customer)->_customer);
        FREE(*customer);
    }
}

size_t get_num_of_left(void) {
    return 0;
}


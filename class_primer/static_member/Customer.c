#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "Customer.h"

static size_t num_of_left = 0;

struct __Customer {
    unsigned amount;
    void (*delete)(Customer*);
};

static bool take_food(Customer* self, unsigned m) {
    errno = 0;
    if (!self || !self->_customer) {
        errno = EINVAL;
        return false;
    }
    if (m > UINT_MAX - self->_customer->amount) {
        errno = ERANGE;
        return false;
    }
    self->_customer->amount += m;
    return true;
}

static bool take_softdrink(Customer* self, unsigned m) {
    errno = 0;
    if (!self || !self->_customer) {
        errno = EINVAL;
        return false;
    }
    if (m > UINT_MAX - self->_customer->amount) {
        errno = ERANGE;
        return false;
    }
    self->_customer->amount += m;
    return true;
}

static bool take_alcohol(Customer* self, unsigned m) {
    (void) m;
    errno = 0;
    if (!self || !self->_customer) {
        errno = EINVAL;
        return false;
    }
    return true;
}

static bool take_beer(Customer* self) {
    errno = 0;
    if (!self || !self->_customer) {
        errno = EINVAL;
        return false;
    }
    return self->take_alcohol(self, 500);
}

static bool accounting(const Customer* self) {
    errno = 0;
    if (!self || !self->_customer) {
        errno = EINVAL;
        return false;
    }
    if (printf("%u\n", self->_customer->amount) == EOF) {
        return false;
    }
    num_of_left++;
    return true;
}

void delete(Customer *self) {
    FREE(self->_customer);
}

bool init_Customer(Customer* customer, void (*del)(Customer*), unsigned **amount) {
    errno = 0;
    if (!customer) {
        errno = EINVAL;
        return false;
    }
    customer->_customer = (_Customer*)malloc(sizeof(struct __Customer));
    if (!customer->_customer) {
        errno = ENOMEM;
        return false;
    }
    customer->_customer->amount = 0;
    customer->_customer->delete = del;
    customer->take_food = take_food;
    customer->take_softdrink = take_softdrink;
    customer->take_alcohol = take_alcohol;
    customer->take_beer = take_beer;
    customer->accounting = accounting;
    if (amount) {
        *amount = &(customer->_customer->amount);
    }
    return true;
}

Customer* new_Customer(void) {
    errno = 0;
    Customer* customer = (Customer*)malloc(sizeof(Customer));
    if (!customer) {
        errno = ENOMEM;
        return NULL;
    }
    if (!init_Customer(customer, delete, NULL)) {
        FREE(customer);
        errno = ENOMEM;
        return NULL;
    }
    return customer;
}

size_t get_num_of_left(void) {
    return num_of_left;
}

void del_Customer(Customer** customer) {
    if (customer && *customer) {
        (*customer)->_customer->delete(*customer);
        FREE(*customer);
    }
}


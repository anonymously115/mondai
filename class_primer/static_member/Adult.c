#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "Adult.h"

extern bool init_Customer(Customer*, void (*)(Customer*), unsigned**);

extern void delete(Customer*);

struct __Adult {
    bool alcohol;
    unsigned *amount;
    bool (*take_food)(Customer*, unsigned);
    bool (*delete)(Customer*);
};

static bool take_food(Customer* super, unsigned m) {
    errno = 0;
    if (!super) {
        errno = EINVAL;
        return false;
    }
    Adult *self = super ? (Adult*) super : NULL;
    if (self->_adult->alcohol) {
        return self->_adult->take_food(super, m - 200);
    } else {
        return self->_adult->take_food(super, m);
    }
}

static bool take_alcohol(Customer* super, unsigned m) {
    errno = 0;
    if (!super) {
        errno = EINVAL;
        return false;
    }
    Adult *self = (Adult*) super;
    unsigned *amount = self->_adult->amount;
    if (m > UINT_MAX - *amount) {
        errno = ERANGE;
        return false;
    }
    *amount += m;
    self->_adult->alcohol = true;
    return true;
}

static void delete_a(Customer *super) {
    Adult *self = (Adult*) super;
    FREE(self->_adult);
    delete(super);
}

Adult* new_Adult(void) {
    errno = 0;
    Adult *adult = (Adult*) malloc(sizeof(Adult));
    if (!adult) {   
        errno = ENOMEM;
        return NULL;
    }
    adult->_adult = (_Adult*) malloc(sizeof(_Adult));
    if (!(adult->_adult)) {
        FREE(adult);
        errno = ENOMEM;
        return NULL;
    }
    if (!init_Customer(&(adult->_customer), delete_a, &(adult->_adult->amount))) {
        errno = ENOMEM;
        FREE(adult);
        return NULL;
    }
    adult->_adult->alcohol = false;
    adult->_adult->take_food = adult->_customer.take_food;
    adult->_customer.take_food = take_food;
    adult->_customer.take_alcohol = take_alcohol;
    return adult;
}

void del_Adult(Adult** adult) {
    if (adult && *adult) {
        FREE((*adult)->_customer._customer);
        FREE((*adult)->_adult);
        FREE(*adult);
    }
}

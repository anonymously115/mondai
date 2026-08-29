#ifndef ADULT_H_
#define ADULT_H_

#include "Customer.h"

typedef struct __Adult _Adult;

typedef struct Adult {
    Customer _customer;
    _Adult *_adult;
} Adult;

Adult* new_Adult(void);

void del_Adult(Adult**);

#endif /* ADULT_H_ */

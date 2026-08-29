#ifndef PUB_H_
#define PUB_H_

#include <stdbool.h>
#include <stddef.h>
#include "Customer.h"

typedef struct __Pub _Pub;

typedef struct Pub {
	_Pub *_pub;
	bool (*add_customer)(struct Pub*, Customer*);
	bool (*query)(struct Pub*, size_t, const char*, unsigned);
} Pub;

Pub* new_Pub(void);

void del_Pub(Pub**);

#endif /* PUB_H_ */

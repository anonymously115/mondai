#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <stdbool.h>

#define FREE(ptr) \
	do { \
		free(ptr); \
		ptr = NULL; \
	} while (0)

typedef struct __Customer _Customer;

typedef struct Customer {
	_Customer *_customer; 
	bool (*take_food)(struct Customer*, unsigned);
	bool (*take_softdrink)(struct Customer*, unsigned);
	bool (*take_alcohol)(struct Customer*, unsigned);
	bool (*take_beer)(struct Customer*);
	bool (*accounting)(const struct Customer*);
} Customer;

Customer* new_Customer(void);

size_t get_num_of_left(void);

void del_Customer(Customer**);

#endif /* CUSTOMER_H_ */

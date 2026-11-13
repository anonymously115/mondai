#ifndef ARRAY_H_
#define ARRAY_H_

#include <stddef.h>

typedef struct __Array _Array;

typedef struct Array {
	_Array *_array;
	bool (*cumulative_sum)(const struct Array*, size_t, int*);
	bool (*interval_sum)(const struct Array*, size_t, size_t, int*);
} Array;

Array* new_Array(size_t, const int[]);

void del_Array(Array**);

#endif /* ARRAY_H_ */

#ifndef ARRAY_H_
#define ARRAY_H_
#include <stddef.h>

typedef struct __Array _Array;

typedef struct Array {
	_Array *_array;
	bool (*pop)(struct Array*, int*);
	bool (*show)(struct Array*);
} Array;

Array* new_Array(size_t, const int[]);

void del_Array(Array**);

#endif /* ARRAY_H_ */

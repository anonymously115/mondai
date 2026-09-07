#ifndef ARRAY_H_
#define ARRAY_H_
#include <stdbool.h>

typedef struct __Array _Array;

typedef struct Array {
    _Array *_array;
    size_t* (*search)(const struct Array*, size_t);
} Array;

Array* new_Array(size_t, const size_t[]);

void del_Array(Array **array);

#endif /* ARRAY_H_ */

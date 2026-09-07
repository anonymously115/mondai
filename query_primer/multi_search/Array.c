#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "Array.h"

// #include <stdio.h>
// #define PRINT(format,...) do{fprintf(stderr, format, ##__VA_ARGS__);fflush(stderr);}while(0);


struct __Array {
    size_t n;
    const size_t *A;
    size_t m;
    size_t *B[];
};

size_t* search(const struct Array *array, size_t k) {
    errno = 0;
    if (!array || !array->_array) {
        errno = EINVAL;
        return NULL;
    }
    size_t m = array->_array->m;
    if (k > m) {
        return NULL;
    }
    return array->_array->B[k];
}

Array* new_Array(size_t n, const size_t A[]) {
    size_t m = 0;
    for (size_t i = 0; i < n; i++) {
        if (A[i] > m) {
            m = A[i];
        }
    }
    Array *array = (Array*) malloc(sizeof(Array));
    if (!array) {
        return NULL;
    }
    array->_array = (_Array*) malloc(sizeof(_Array) + sizeof(array->_array->B[0]) * (m + 1));
    if (!array->_array) {
        free(array);
        array = NULL;
        return NULL;
    }
    array->_array->n = n;
    array->_array->A = A;
    array->_array->m = m;
    memset(array->_array->B, 0, sizeof(array->_array->B[0]) * (m + 1));
    for (size_t i = 0; i < n; i++) {
        if (!(array->_array->B[A[i]])) {
            array->_array->B[A[i]] = (size_t*) (A + i);
        }
    }
    array->search = search;
    return array;
}

void del_Array(Array **array) {
    free((*array)->_array);
    (*array)->_array = NULL;
    free(*array);
    *array = NULL;
}

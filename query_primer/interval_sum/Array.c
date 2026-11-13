#include <stdlib.h>
#include <errno.h>
#include "Array.h"

#define FREE(ptr) do { free(ptr); ptr = NULL; } while (0)

struct __Array {
    size_t n;
    const int *A;
    int S[];
};

static bool cumulative_sum(const Array *self, size_t k, int *s) {
    errno = 0;
    if (!self || !s) {
        errno = EINVAL;
        return false;
    }
    const _Array *a = self->_array;
    size_t n = a->n;
    if (k > n) {
        errno = ERANGE;
        return false;
    }
    *s = a->S[k];
    return true;
}

static bool interval_sum(const Array *self, size_t l, size_t r, int *s) {
    errno = 0;
    if (!self || !s) {
        errno = EINVAL;
        return false;
    } else if (l > r) {
        errno = ERANGE;
        return false;
    }
    const _Array *a = self->_array;
    size_t n = a->n;
    if (r > n) {
        errno = ERANGE;
        return false;
    }
    *s = a->S[r] - a->S[l];
    return true;
}

Array* new_Array(size_t n, const int A[]) {
    Array *array = (Array*) malloc(sizeof(Array));
    if (!array) {
        return NULL;
    }
    array->_array = (_Array*) malloc(sizeof(_Array) + sizeof(array->_array->S[0]) * (n + 1));
    if (!(array->_array)) {
        FREE(array);
        return NULL;
    }
    array->_array->n = n;
    array->_array->A = A;
    array->_array->S[0] = 0;
    for (size_t i = 0; i < n; i++) {
        array->_array->S[i + 1] = array->_array->S[i] + A[i];
    }
    array->cumulative_sum = cumulative_sum;
    array->interval_sum = interval_sum;
    return array;
}

void del_Array(Array** array) {
    FREE((*array)->_array);
    FREE(*array);
}
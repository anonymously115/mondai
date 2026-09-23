#include "Array.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct __Array {
	size_t n;
	const int *A;
	size_t k;
};

bool pop(Array* self, int* value) {
	errno = 0;
	if (!self) {
		errno = EINVAL;
		return false;
	}
	size_t *n = &(self->_array->n);
	if (!*n) {
		return false;
	}
	if (value) {
		*value = *(self->_array->A);
	}
	(self->_array->A)++;
	(*n)--;
	return true;
}

bool show(Array* self) {
	errno = 0;
	if (!self) {
		errno = EINVAL;
		return false;
	}
	size_t *k = &(self->_array->k);
	if (++(*k) > 10) {
		return false;
	}
	size_t n = self->_array->n;
	const int *A = self->_array->A;
	for (size_t i = 0; i < n; i++) {
		printf("%d\n", A[i]);
	}
	return true;
}

Array* new_Array(size_t n, const int A[]) {
	Array *array = (Array*) calloc(1, sizeof(Array));
	if (!array) {
		return NULL;
	}
	array->_array = (_Array*) calloc(1, sizeof(_Array));
	if (!(array->_array)) {
		free(array);
		array = NULL;
		return NULL;
	}
	array->_array->n = n;
	array->_array->A = A;
	array->_array->k = 0;
	array->pop = pop;
	array->show = show;
	return array;
}

void del_Array(Array **array) {
	free((*array)->_array);
	(*array)->_array = NULL;
	free(*array);
	*array = NULL;
}
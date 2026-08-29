#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "Pub.h"

struct __Pub {
	size_t n;
	Customer* customers[];
};

static bool add_customer(Pub *self, Customer *customer) {
	errno = 0;
	if (!self || !self->_pub || !customer || !customer->_customer) {
		errno = EINVAL;
		return false;
	}
	size_t n = self->_pub->n;
	_Pub *pub = (_Pub*) realloc(self->_pub, sizeof(_Pub) + sizeof(self->_pub->customers[0]) * -~n);
	if (!pub) {
		return false;
	}
	pub->n = n + 1;
	pub->customers[n] = customer;
	self->_pub = pub;
	return true;
}

bool query(Pub* self, size_t n, const char *s, unsigned m) {
	errno = 0;
	if (!self || !self->_pub || !s) {
		errno = EINVAL;
		return false;
	} else if (n >= self->_pub->n) {
		errno = ERANGE;
		return false;
	}
	Customer *customer = self->_pub->customers[n];
	if (!strcmp(s, "food")) {
		customer->take_food(customer, m);
	} else if (!strcmp(s, "softdrink")) {
		customer->take_softdrink(customer, m);
	} else if (!strcmp(s, "alcohol")) {
		customer->take_alcohol(customer, m);
	} else if (!strcmp(s, "0")) {
		customer->take_beer(customer);
	} else if (!strcmp(s, "A")) {
		customer->accounting(customer);
	} else {
		errno = EINVAL;
		return false;
	}
	return true;
}

Pub* new_Pub(void) {
	errno = 0;
	Pub *pub = (Pub*) malloc(sizeof(Pub));
	if (!pub) {
		return NULL;
	}
	pub->_pub = (_Pub*) malloc(sizeof(_Pub));
	if (!(pub->_pub)) {
		FREE(pub);
		return NULL;
	}
	pub->_pub->n = 0;
	pub->add_customer = add_customer;
	pub->query = query;
	return pub;
}

void del_Pub(Pub** pub) {
	if (pub && *pub) {
		FREE((*pub)->_pub);
		FREE(*pub);
	}
}

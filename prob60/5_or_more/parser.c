#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "util.h"

bool parse_size(const char *str, size_t *n) {
	errno = 0;
	if (!(str && *str) || !n) {
		errno = EINVAL;
		return false;
	}
	char t[strlen(str) + 1];
	size_t m;
	if (!parse_ulong(chomp(strcpy(t, str)), &m)) {
		return false;
	}
	if (m < 1 || 1000 < m) {
		errno = ERANGE;
		return false;
	}
	*n = m;
	return true;
}

bool parse_element(const char *str, int *a) {
	errno = 0;
	if (!(str && *str) || !a) {
		errno = EINVAL;
		return false;
	}
	char t[strlen(str) + 1];
	int b;
	if (!parse_int(chomp(strcpy(t, str)), &b)) {
		return false;
	}
	if (b < -1000000 || 1000000 < b) {
		errno = ERANGE;
		return false;
	}
	*a = b;
	return true;
}
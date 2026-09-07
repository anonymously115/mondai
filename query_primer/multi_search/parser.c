#include <string.h>
#include <errno.h>
#include "parser.h"
#include "util.h"

bool parse_size(const char *str, size_t *n, size_t *q) {
	errno = 0;
	if (!(str && *str && n && q)) {
		errno = EINVAL;
		return false;
	}
	char s[strlen(str) + 1];
	if (!parse_ulong(strtok(chomp(strcpy(s, str)), " "), n)) {
		errno = EINVAL;
		return false;
	} else if (*n < 1 || 100000 < *n) {
		errno = ERANGE;
		return false;
	}
	if (!parse_ulong(strtok(NULL, " "), q)) {
		errno = EINVAL;
		return false;
	} else if (*q < 1 || 100000 < *q) {
		errno = ERANGE;
		return false;
	}
	if (strtok(NULL, " ")) {
		errno = EINVAL;
		return false;
	}
	return true;
}

bool parse_element(const char *str, size_t *k) {
	errno = 0;
	if (!(str && *str && k)) {
		errno = EINVAL;
		return false;
	}
	char s[strlen(str) + 1];
	if (!parse_ulong(chomp(strcpy(s, str)), k)) {
		errno = EINVAL;
		return false;
	} else if (1000000 < *k) {
		errno = ERANGE;
		return false;
	}
	return true;
}
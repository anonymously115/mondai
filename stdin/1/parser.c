#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "util.h"

bool parse(const char *str, char *s) {
	errno = 0;
	if (!(str && *str) || !s) {
		errno = EINVAL;
		return false;
	}
	char t[strlen(str) + 1];
	size_t len = strlen(chomp(strcpy(t, str)));
	if (len < 1 || 100 < len) {
		errno = EINVAL;
		return false;
	}
	for (char *c = t; *c; c++) {
		if (!isalnum(*c)) {
			errno = EINVAL;
			return false;
		}
	}
	strcpy(s, t);
	return true;
}
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "util.h"

bool parse_num(const char *str, size_t *N, size_t *K) {
	errno = 0;
	if (!(str && *str && N && K)) {
		errno = EINVAL;
		return false;
	}
	char t[strlen(str) + 1];
	if (!parse_ulong(strtok(chomp(strcpy(t, str)), " "), N)) {
		return false;
	}
	if (*N < 1 || 1000 < *N) {
		errno = ERANGE;
		return false;
	}
	if (!parse_ulong(strtok(NULL, " "), K)) {
		return false;
	}
	if (*K < 1 || 1000 < *K) {
		errno = ERANGE;
		return false;
	}
	if (strtok(NULL, " ")) {
		errno = EINVAL;
		return false;
	}
	return true;
}

bool parse_age(const char *str, unsigned *a) {
	errno = 0;
	if (!(str && *str && a)) {
		errno = EINVAL;
		return false;
	}
	char t[strlen(str) + 1];
	if (!parse_uint(chomp(strcpy(t, str)), a)) {
		return false;
	}
	if (*a < 1 || 100 < *a) {
		errno = ERANGE;
		return false;
	}
	return true;

}

bool parse_order(const char *str, size_t N, size_t *n, char *s, unsigned *m) {
	errno = 0;
	if (!(str && *str && n && s && m)) {
		errno = EINVAL;
		return false;
	}
	char t[strlen(str) + 1];
	if (!parse_ulong(strtok(chomp(strcpy(t, str)), " "), n)) {
		return false;
	}
	if (*n < 1 || N < *n) {
		errno = ERANGE;
		return false;
	}
	char *p = strtok(NULL, " ");
	if (!p) {
		errno = EINVAL;
		return false;
	}
	if (!strcmp(p, "food") || !strcmp(p, "softdrink") || !strcmp(p, "alcohol")) {
		if (!parse_uint(strtok(NULL, " "), m)) {
			return false;
		}
		if (*m < 300 || 5000 < *m) {
			errno = ERANGE;
			return false;
		}
	} else if (strcmp(p, "0") && strcmp(p, "A")) {
		errno = EINVAL;
		return false;
	}
	strcpy(s, p);
	if (strtok(NULL, " ")) {
		errno = EINVAL;
		return false;
	}
	return true;
}

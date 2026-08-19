#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "util.h"

char* chomp(char *str) {
	if (!str) {
		return NULL;
	}
	size_t n = strlen(str);
	while (n--) {
		if (isspace(str[n])) {
			str[n] = '\0';
		} else {
			break;
		}
	}
	return str;
}

static bool is_positive_number(const char *str) {
	if (!(str && *str)) {
		return false;
	}
	if (str[0] == '0') {
		return str[1] == '\0';
	}
	for (const char *c = str; *c; c++) {
		if (!isdigit(*c)) {
			return false;
		}
	}
	return true;
}

static bool is_number(const char *str) {
	if (!(str && *str)) {
		return false;
	}
	if (str[0] == '-') {
		if (str[1] == '0') {
			return false;
		} else {
			return is_positive_number(str + 1);
		}
	} else {
		return is_positive_number(str);
	}
}

static bool parse_unsigned(const char *str, uintmax_t *n, uintmax_t max) {
	errno = 0;
	if (!is_positive_number(str)) {
		errno = EINVAL;
		return false;
	}
	char *s = NULL;
	uintmax_t m = strtoumax(str, &s, 10);
	if (errno) {
		return false;
	} else if (s && *s) {
		errno = EINVAL;
		return false;
	} else if (m > max) {
		errno = ERANGE;
		return false;
	}
	*n = m;
	return true;
}

static bool parse_signed(const char *str, intmax_t *n, intmax_t max) {
	errno = 0;
	if (!is_number(str)) {
		errno = EINVAL;
		return false;
	}
	char *s = NULL;
	intmax_t m = strtoimax(str, &s, 10);
	if (errno) {
		return false;
	} else if (s && *s) {
		errno = EINVAL;
		return false;
	} else if (m < ~max || max < m) {
		errno = ERANGE;
		return false;
	}
	*n = m;
	return true;
}

bool parse_ulong(const char *str, size_t *n) {
	if (!(str && *str) || !n) {
		errno = EINVAL;
		return false;
	}
	uintmax_t m;
	if (!parse_unsigned(str, &m, SIZE_MAX)) {
		return false;
	}
	*n = (size_t) m;
	return true;
}

bool parse_int(const char *str, int *n) {
	if (!(str && *str) || !n) {
		errno = EINVAL;
		return false;
	}
	intmax_t m;
	if (!parse_signed(str, &m, INT_MAX)) {
		return false;
	}
	*n = (int) m;
	return true;
}

bool sum(size_t n, const int A[], int* s) {
	errno = 0;
	if ((n && !A) || !s) {
		errno = EINVAL;
		return false;
	}
	*s = 0;
	for (size_t i = 0; i < n; i++) {
		if (*s > 0 && A[i] > 0 && A[i] > INT_MAX - *s) {
			errno = ERANGE;
			return false;
		}
		if (*s < 0 && A[i] < 0 && A[i] < INT_MIN - *s) {
			errno = ERANGE;
			return false;
		}
		*s += A[i];
	}
	return true;
}

bool is_5_or_more(int n) {
	return n >= 5;
}

bool filter(bool (*fn)(int), size_t n, const int A[], size_t *m, int B[]) {
	errno = 0;
	if (!fn || (n && !A) || !m || !B) {
		errno = EINVAL;
		return false;
	}
	*m = 0;
	for (size_t i = 0; i < n; i++) {
		if (fn(A[i])) {
			B[(*m)++] = A[i];
		}
	}
	return true;
}
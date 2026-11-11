#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "util.h"

bool parse_size(const char* str, size_t* n, size_t* k) {
    errno = 0;
    if (!(str && *str)) {
        errno = EINVAL;
        return false;
    }
    char s[strlen(str) + 1];
    if (!parse_ulong(strtok(chomp(strcpy(s, str)), " "), n)) {
        return false;
    }
    if (*n < 1 || 100000 < *n) {
        errno = ERANGE;
        return false;
    }
    if (!parse_ulong(strtok(NULL, " "), k)) {
        return false;
    }
    if (*k < 1 || 100000 < *k) {
        errno = ERANGE;
        return false;
    }
    if (strtok(NULL, " ")) {
        errno = EINVAL;
        return false;
    }
    return true;
}

bool parse_element(const char* str, int* a) {
    errno = 0;
    if (!(str && *str)) {
        errno = EINVAL;
        return false;
    }
    char s[strlen(str) + 1];
    if (!parse_int(chomp(strcpy(s, str)), a)) {
        return false;
    }
    if (*a < -100 || 100 < *a) {
        errno = ERANGE;
        return false;
    }
    return true;
}

bool parse_query(const char* str, size_t n, size_t* q) {
    errno = 0;
    if (!(str && *str)) {
        errno = EINVAL;
        return false;
    }
    char s[strlen(str) + 1];
    if (!parse_ulong(chomp(strcpy(s, str)), q)) {
        return false;
    }
    if (*q < 1 || n < *q) {
        errno = ERANGE;
        return false;
    }
    return true;
}

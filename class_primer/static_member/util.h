#ifndef UTIL_H_
#define UTIL_H_
#include <stddef.h>
#include <stdbool.h>

char* chomp(char*);

bool parse_ulong(const char*, size_t*);

bool parse_int(const char*, int*);

bool parse_uint(const char*, unsigned*);

#endif /* UTIL_H_ */

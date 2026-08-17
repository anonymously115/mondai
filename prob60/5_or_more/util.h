#ifndef UTIL_H_
#define UTIL_H_
#include <stddef.h>
#include <stdbool.h>

char* chomp(char*);

bool parse_ulong(const char*, size_t*);

bool parse_int(const char*, int*);

bool sum(size_t, const int[], int*);

bool is_5_or_more(int);

bool filter(bool (*)(int), size_t, const int[], size_t*, int[]);

#endif /* UTIL_H_ */

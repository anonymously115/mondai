#ifndef PARSER_H_
#define PARSER_H_
#include <stdbool.h>
#include <stddef.h>

bool parse_num(const char*, size_t*, size_t*);

bool parse_age(const char*, unsigned*);

bool parse_order(const char*, size_t, size_t*, char*, unsigned*);

#endif /* PARSER_H_ */

#ifndef PARSER_H_
#define PARSER_H_
#include <stdbool.h>

bool parse(const char*, char*);

bool parse_size(const char*, size_t*, size_t*);

bool parse_element(const char*, size_t*);

#endif /* PARSER_H_ */

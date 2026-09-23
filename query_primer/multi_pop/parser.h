#ifndef PARSER_H_
#define PARSER_H_
#include <stdbool.h>

bool parse_size(const char*, size_t*, size_t*);

bool parse_element(const char*, int*);

bool parse_query(const char*, char*);

#endif /* PARSER_H_ */

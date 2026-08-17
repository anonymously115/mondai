#ifndef PARSER_H_
#define PARSER_H_
#include <stdbool.h>
#include <stddef.h>

bool parse_size(const char*, size_t*);

bool parse_element(const char*, int*);

#endif /* PARSER_H_ */

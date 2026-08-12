// #define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#ifdef NDEBUG
#include <string.h>
#include <errno.h>
#include "parser.h"
const char END[] = { EOF, '\n', 0 };
#define PARSE(buf, siz, fun, msg)                                   \
	do {                                                            \
		errno = 0;                                                  \
		char buf[siz];                                              \
		if (!fgets(buf, sizeof(buf), stdin))                        \
			return EXIT_FAILURE;                                    \
		if (!strchr(buf, '\n'))                                     \
			for (int _ = getchar(); !strchr(END, _); _ = getchar());\
		else if (fun)                                               \
			break;                                                  \
		perror(msg);                                                \
	} while (1)
#else
#include <time.h>
#define PRINT(format, ...)                      \
	do {                                        \
		fprintf(stderr, format, ##__VA_ARGS__); \
		fflush(stderr);                         \
	} while (0)
#endif

int main(void) {
#ifndef NDEBUG
	clock_t clockt = clock();
#endif
	char str[101] = { };
#ifdef NDEBUG
	PARSE(s, 102, parse(s, str), NULL);
#endif
	int result = puts(str);
	if (result == EOF) {
		return EXIT_FAILURE;
	}
#ifndef NDEBUG
	PRINT("took %f sec.\n", (float) (clock() - clockt) / CLOCKS_PER_SEC);
#endif
	return EXIT_SUCCESS;
}

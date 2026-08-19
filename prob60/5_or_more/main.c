// #define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
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
	size_t N;
#ifdef NDEBUG
	PARSE(s, 6, parse_size(s, &N), NULL);
#else
	N = 1000;
#endif
	int A[N];
	for (size_t i = 0; i < N; i++) {
#ifdef NDEBUG
		PARSE(s, 9, parse_element(s, &A[i]), NULL);
#else
		A[i] = 1000000;
#endif
	}
	size_t n;
	int a[N];
	if (!filter(is_5_or_more, N, A, &n, a)) {
		return EXIT_FAILURE;
	}
	int s;
	if (!sum(n, a, &s)) {
		return EXIT_FAILURE;
	}
	int result = printf("%d\n", s);
	if (result == EOF) {
		return EXIT_FAILURE;
	}
#ifndef NDEBUG
	PRINT("took %f sec.\n", (float) (clock() - clockt) / CLOCKS_PER_SEC);
#endif
	return EXIT_SUCCESS;
}

// #define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include "Array.h"
#ifdef NDEBUG
#include <string.h>
#include <errno.h>
#include "parser.h"
const char END[] = { EOF, '\n', 0 };
#define PARSE(buf, siz, fun, msg) \
	do { \
		errno = 0; \
		char buf[siz]; \
		if (!fgets(buf, sizeof(buf), stdin)) \
			return EXIT_FAILURE; \
		if (!strchr(buf, '\n')) \
			for (int _ = getchar(); !strchr(END, _); _ = getchar()); \
		else if (fun) \
			break; \
		perror(msg); \
	} while (1)
#else
#include <time.h>
#define N (100000)
#define Q (100000)
#define PRINT(format, ...) \
	do { \
		fprintf(stderr, format, ##__VA_ARGS__); \
		fflush(stderr); \
	} while (0)
#endif

int main(void) {
#ifndef NDEBUG
	clock_t clockt = clock();
#endif
#ifdef NDEBUG
	size_t N, Q;
	PARSE(s, 15, parse_size(s, &N, &Q), NULL);
#endif
	size_t A[N];
	for (size_t i = 0; i < N; i++) {
#ifdef NDEBUG
		PARSE(s, 9, parse_element(s, &A[i]), NULL);
#else
		A[i] = 10 * (i + 1);		
#endif
	}
	Array *array = new_Array(N, A);
	for (size_t j = 0; j < Q; j++) {
		size_t k;
#ifdef NDEBUG
		PARSE(s, 9, parse_element(s, &k), NULL);
#else
		k = 900001 + j;
#endif
		if (puts(array->search(array, k) ? "YES" : "NO") == EOF) {
			del_Array(&array);
			return EXIT_FAILURE;
		}
	}
	del_Array(&array);
#ifndef NDEBUG
	PRINT("took %f sec.\n", (float) (clock() - clockt) / CLOCKS_PER_SEC);
#endif
	return EXIT_SUCCESS;
}

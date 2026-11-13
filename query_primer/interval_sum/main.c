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
#define K (100000)
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
	size_t N, K;
	PARSE(s, 15, parse_size(s, &N, &K), NULL);
#endif
	int A[N];
	for (size_t i = 0; i < N; i++) {
#ifdef NDEBUG
		PARSE(s, 6, parse_element(s, &A[i]), NULL);
#else
		A[i] = (i + 100) % 201 - 100;
#endif
	}
	Array *array = new_Array(N, A);
	for (size_t i = 0; i < K; i++) {
		size_t l, r;
#ifdef NDEBUG
		PARSE(s, 15, parse_query(s, N, &l, &r), NULL);
#else
		l = i + 1;
		r = 100000;
#endif
		l -= 1;
		int s;
		if (!array->interval_sum(array, l, r, &s)) {
			del_Array(&array);
			return EXIT_FAILURE;
		}
		if (printf("%d\n", s) == EOF) {
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

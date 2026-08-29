// #define NDEBUG
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Pub.h"
#include "Adult.h"
#ifdef NDEBUG
#include <errno.h>
#include "parser.h"
const char END[] = { EOF, '\n', 0 };

#define PARSE(buf, siz, fun, num, arr, ptr, msg) \
	do { \
		errno = 0; \
		char buf[siz]; \
		if (!fgets(buf, sizeof(buf), stdin)) { \
			DELETE_ALL(num, arr, ptr); \
			return EXIT_FAILURE; \
		} \
		if (!strchr(buf, '\n')) \
			for (int _ = getchar(); !strchr(END, _); _ = getchar());\
		else if (fun) \
			break; \
		perror(msg); \
	} while (1)
#else
#include <time.h>
#define PRINT(format, ...) \
	do { \
		fprintf(stderr, format, ##__VA_ARGS__); \
		fflush(stderr); \
	} while (0)
#endif

#define DELETE_ALL(num, arr, ptr) \
	do { \
		size_t _ = (num); \
		while (_--) { \
			del_Customer((arr) + _); \
		} \
		del_Pub(ptr); \
	} while (0)

int main(void) {
#ifndef NDEBUG
	clock_t clockt = clock();
#endif
	size_t N, K;
#ifdef NDEBUG
	PARSE(t, 11, parse_num(t, &N, &K), 0, NULL, NULL, NULL);
#else
	N = 1000;
	K = 1000;
#endif
	Pub *pub = new_Pub();
	if (!pub) {
		return EXIT_FAILURE;
	}
	Customer *customers[N];
	for (size_t i = 0; i < N; i++) {
		unsigned a;
#ifdef NDEBUG
		PARSE(t, 5, parse_age(t, &a), i, customers, &pub, NULL);
#else 
		a = (unsigned) (i % 100 + 1);
#endif
		customers[i] = a < 20 ? new_Customer() : (Customer*) new_Adult();
		if (!customers[i]) {
			DELETE_ALL(i, customers, &pub);
			return EXIT_FAILURE;
		}
		if (!pub->add_customer(pub, customers[i])) {
			DELETE_ALL(i + 1, customers, &pub);
			return EXIT_FAILURE;
		}
	}
	while (K--) {
		size_t n;
		char s[10];
		unsigned m;
#ifdef NDEBUG
		PARSE(t, 22, parse_order(t, N, &n, s, &m), N, customers, &pub, NULL);
#else
		n = (K % N + 1);
		strcpy(s, "A");
		m = 0;
#endif
		if (!pub->query(pub, ~-n, s, m)) {
			DELETE_ALL(N, customers, &pub);
			return EXIT_FAILURE;
		}
	}
	DELETE_ALL(N, customers, &pub);
	int result = printf("%zu\n", get_num_of_left());
	if (result < 0) {
		return EXIT_FAILURE;
	}
#ifndef NDEBUG
	PRINT("took %f sec.\n", (float) (clock() - clockt) / CLOCKS_PER_SEC);
#endif
	return EXIT_SUCCESS;
}

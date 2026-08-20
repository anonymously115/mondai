#include <stdio.h>
#include <stdlib.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#endif
#include <string.h>
#include <time.h>
#include <errno.h>
#include "minunit.h"
#include "util.h"
#define IN "in.txt"
#define OUT "out.txt"
#define ERR "err.txt"
#define MAIN "./main"
#define SIZE(a) (sizeof(a) / sizeof(*a))
#define THROW(proc) \
	do { \
		int e = errno; \
		do { proc } while (0); \
		static char _[256]; \
		snprintf(_, sizeof(_), "%s:%d: %s\n", __FUNCTION__, __LINE__, strerror(e)); \
		return _; \
	} while (0)

int tests_run = 0;

static char* message_s(const char *expected, const char *actual) {
	static char msg[256];
	snprintf(msg, sizeof(msg), "Error: expected: <%s> but was: <%s>", expected, actual);
	return msg;
}

int exec() {
#if defined(_WIN32) || defined(_WIN64)
	return system(".\\main <" IN " 1>" OUT " 2>" ERR);
#else
 	pid_t pid = fork();
 	if (pid < 0) {
 	 	perror(NULL);
 	 	return -1;
 	}
	if (pid == 0) {
		int in = open(IN, O_RDONLY);
		if (in < 0) {
			perror(NULL);
			_exit(-1);
		}
		if (dup2(in, STDIN_FILENO) < 0) {
			perror(NULL);
			_exit(-1);
		}
		close(in);
		
		int out = open(OUT, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out < 0) {
			perror(NULL);
			_exit(-1);
		}
		if (dup2(out, STDOUT_FILENO) < 0) {
			perror(NULL);
			_exit(-1);
		}
		close(out);
		
		int err = open(ERR, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (err < 0) {
			perror(NULL);
			_exit(-1);
		}
		if (dup2(err, STDERR_FILENO) < 0) {
			perror(NULL);
			_exit(-1);
		}
		close(err);
		
		execl(MAIN, MAIN, NULL);
		perror(NULL);
		_exit(-1);
	} else {
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			return WEXITSTATUS(status);
		} else {
			return -1;
		}
	}
#endif
}

static char* test(size_t n, const char *input[], size_t m, const char *expected[]) {
	errno = 0;
	FILE *in = fopen(IN, "w");
	if (!in) THROW();
	for (size_t i = 0; i < n; i++) {
		if (fprintf(in, "%s\n", input[i] ? input[i] : "") == EOF) {
			THROW(fclose(in););
		}
	}
	if (fclose(in) != 0) THROW();
	in = NULL;

	clock_t clockt = clock();
	int result = exec();
	fprintf(stderr, "#%d: %f sec.\n", tests_run, (float) (clock() - clockt) / CLOCKS_PER_SEC);
	fflush(stderr);
	if (expected != NULL && result != EXIT_SUCCESS) {
		return "Error: expected: <EXIT_SUCCESS> but was: <EXIT_FAILURE>";
	} else if (expected == NULL && result == EXIT_SUCCESS) {
		return "Error: expected: <EXIT_FAILURE> but was: <EXIT_SUCCESS>";
	}

	if (expected) {
		size_t maxlen = 0;
		for (size_t i = 0; i < m; i++) {
			if (expected[i]) {
				size_t len = strlen(expected[i]);
				if (len > maxlen) {
					maxlen = len;
				}
			}
		}
		char actual[m + 1][maxlen + 2];
		memset(actual, 0, sizeof(actual));

		FILE *out = fopen(OUT, "r");
		if (!out) THROW();
		for (size_t i = 0; i < m; i++) {
			if (!chomp(fgets(actual[i], sizeof(actual[i]), out))) {
				fclose(out);
				return message_s(expected[i], actual[i]);
			}
		}
		if (chomp(fgets(actual[m], sizeof(actual[m]), out))) {
			fclose(out);
			return message_s("", actual[m]);
		}
		for (size_t i = 0; i < m; i++) {
			if (expected[i]) {
				mu_assert(message_s(expected[i], actual[i]), !strcmp(actual[i], expected[i]));
			} else {
				mu_assert(message_s("", actual[i]), !strcmp(actual[i], ""));
			}
		}

		if (fclose(out) != 0) THROW();
		out = NULL;
	}
	if (remove(ERR) != 0) THROW();
	if (remove(OUT) != 0) THROW();
	if (remove(IN) != 0) THROW();
	return 0;
}

static char* test_0() {
	return test(0, NULL, 0, NULL);
}

static char* test_1() {
	const char *in[] = { "1", "4" };
	const char *out[] = { "0" };
	return test(SIZE(in), in, SIZE(out), out);
}

static char* test_2() {
	const char *in[] = { "1", "5" };
	const char *out[] = { "5" };
	return test(SIZE(in), in, SIZE(out), out);
}

static char* test_3() {
	const char *in[1001] = { "1000" };
	for (size_t i = 1; i <= 1000; i++) {
		in[i] = "1000000";
	}
	const char *out[] = { "1000000000" };
	return test(SIZE(in), in, SIZE(out), out);
}

static char* all_tests() {
	mu_run_test(test_0);
	mu_run_test(test_1);
	mu_run_test(test_2);
	mu_run_test(test_3);
	return 0;
}

int main() {
	char *result = all_tests();
	if (result != 0) {
		fprintf(stderr, "%s\n", result);
		fprintf(stderr, "Tests run: %d\n", tests_run);
		fflush(stderr);
	} else {
		fprintf(stdout, "ALL TESTS PASSED\n");
		fprintf(stdout, "Tests run: %d\n", tests_run);
		fflush(stdout);
	}
	return result != 0;
}

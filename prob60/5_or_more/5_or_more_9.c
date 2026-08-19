#include <stdio.h>

int main(void) {
	size_t N;
	scanf("%zu", &N);
	int A[N];
	for (size_t i = 0; i < N; i++) {
		scanf("%d", &A[i]);
	}
	int s = 0;
	for (size_t i = 0; i < N; i++) {
		if (A[i] >= 5) {
			s += A[i];
		}
	}
	printf("%d\n", s);
	return 0;
}

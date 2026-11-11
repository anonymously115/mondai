#include <stdio.h>

int main(void) {
	size_t N, K;
	scanf("%zu %zu", &N, &K);
	int A[N];
	int S[N + 1];
	S[0] = 0;
	for (size_t i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		S[i + 1] = S[i] + A[i];
	}
	for (size_t i = 0; i < K; i++) {
		size_t q;
		scanf("%zu", &q);
		printf("%d\n", S[q]);
	}
	return 0;
}

#include <stdio.h>
#include <stdbool.h>

int main(void) {
	int N, Q;
	scanf("%d %d", &N, &Q);
	int A[N];
	bool B[1000001] = { };
	for (int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		B[A[i]] = true;
	}
	for (int i = 0; i < Q; i++) {
		int k;
		scanf("%d", &k);
		puts(B[k] ? "YES" : "NO");
	}
	return 0;
}

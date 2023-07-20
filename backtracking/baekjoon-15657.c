#include <stdio.h>

int A[8], N, M;
int visited[8];

void visit(int depth) {
    int i, j;
    for (i = (depth == 0 ? 0 : visited[depth - 1]); i < N; ++i) {
        visited[depth] = i;
        if (depth == M - 1) {
            for (j = 0; j < M; ++j)
                printf("%d ", A[visited[j]]);
            puts("");
        } else {
            visit(depth + 1);
        }
    }
}

int main(void) {
    scanf("%d %d", &N, &M);
    int i, j;
    for (i = 0; i < N; ++i)
        scanf("%d", &A[i]);
    int tmp;
    // 먼저 입력을 정렬
    for (i = 0; i < N; ++i) {
        for (j = i + 1; j < N; ++j) {
            if (A[j] < A[i]) {
                tmp = A[i];
                A[i] = A[j];
                A[j] = tmp;
            }
        }
    }

    visit(0);
}
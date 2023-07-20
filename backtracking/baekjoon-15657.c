#include <stdio.h>

int A[8], N, M;
int visited[8];

void visit(int depth) {
    int i, j;
    // 보통의 N과 M 문제로 변환해서 푼다.
    // 즉, A[0]~A[N-1]을 0~N-1로 변환해서 다른 N과 M 풀듯이 풀고,
    // 다 풀고 나서 다시 A[0]~A[N-1]로 변환해서 출력한다.

    // 다음 손패로 이전에 썼던 손패 이상의 수를 뽑는다.
    for (i = (depth == 0 ? 0 : visited[depth - 1]); i < N; ++i) {
        // visited의 의미가 다른 것과 다름 (depth단계의 손패는 i라는 뜻)
        visited[depth] = i;
        if (depth == M - 1) {
            // 0~N-1을 A[0]~A[N-1]로 변환해서 출력.
            for (j = 0; j < M; ++j)
                printf("%d ", A[visited[j]]);
            puts("");
        } else {
            // 손패를 더 뽑음.
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
    // 먼저 입력을 정렬해야 A[0]~A[N-1]이 0~N-1에 대응되어 오름차순으로 풀기 쉽다.
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
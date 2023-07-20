#include <stdio.h>

#define M 6

int S[13];
int trace[13];
unsigned char visited[13];
int N;

void continue_trace(int depth) {
    if (depth == M-1) {
        for (int i = 0; i < M; ++i) {
            printf("%d ", S[trace[i]]);
        }
        printf("\n");
        return;
    } else {
        if (N - trace[depth] < M - depth) return;

        visited[trace[depth]] = 1;
        for (int i = trace[depth] + 1; i < N; ++i) {
            if (!visited[i]) {
                trace[depth+1] = i;
                continue_trace(depth + 1);
            }
        }
        visited[trace[depth]] = 0;
    }
}

int main() {
    while (1) {
        scanf("%d", &N);
        if (N <= 0) {
            return 0;
        }
        for (int i = 0; i < N; ++i) {
            scanf("%d", &S[i]);
        }

        for (int i = 0; i < N; ++i) {
            visited[i] = 0;
        }

        for (int i = 0; i < N; ++i) {
            trace[0] = i;
            continue_trace(0);
        }

        puts("");
    }

    // 실제로는 닿지 않음
    return 0;
}
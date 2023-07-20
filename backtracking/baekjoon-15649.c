#include <stdio.h>

int trace[8];
unsigned char visited[8];
int N, M;

void continue_trace(int depth) {
    if (depth == M-1) {
        for (int i = 0; i < M; ++i) {
            printf("%d ", trace[i] + 1);
        }
        printf("\n");
        return;
    } else {
        visited[trace[depth]] = 1;
        for (int i = 0; i < N; ++i) {
            if (!visited[i]) {
                trace[depth+1] = i;
                continue_trace(depth + 1);
            }
        }
        visited[trace[depth]] = 0;
    }
}

int main() {
    scanf("%d %d", &N, &M);

    for (int i = 0; i < M; ++i) {
        visited[i] = 0;
    }

    for (int i = 0; i < N; ++i) {
        trace[0] = i;
        continue_trace(0);
    }

    return 0;
}
#include <stdio.h>

int trace[8];
unsigned char visited[8];
int N, M;

void continue_trace(int depth) {
    // depth번째까지의, 즉 depth개의 수를 이미 뽑았을 때.
    if (depth == M-1) {
        // M개를 뽑았다면, 수열이 완성된 것.
        for (int i = 0; i < M; ++i) {
            // 0~N-1로 저장된 것을 1~N으로 변환해 출력.
            printf("%d ", trace[i] + 1);
        }
        printf("\n");
        return;
    } else {
        // 현재 뽑은 숫자를 뽑았다고 마킹
        visited[trace[depth]] = 1;
        for (int i = 0; i < N; ++i) {
            if (!visited[i]) {
                // 뽑지 않은 숫자를 넣고, 계속 수열을 구해 본다.
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
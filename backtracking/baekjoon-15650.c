#include <stdio.h>

int trace[8];
unsigned char visited[8];
int N, M;

void continue_trace(int depth) {
    // M개 뽑았을 때 리턴하는 루틴은 15649와 동일.
    if (depth == M-1) {
        for (int i = 0; i < M; ++i) {
            printf("%d ", trace[i] + 1);
        }
        printf("\n");
        return;
    } else {
        // 쓸 수 있는 숫자가 소진되어 있으면 조기 리턴.
        // 예로, 7까지 썼는데 2개를 더 채워야 하는 경우.
        if (N - trace[depth] < M - depth) return;

        // 현재 뽑힌 숫자를 마킹.
        visited[trace[depth]] = 1;
        for (int i = trace[depth] + 1; i < N; ++i) {
            // 그 다음 숫자를 하나씩 뽑으면서, 계속 손패를 뽑아 본다.
            if (!visited[i]) {
                trace[depth+1] = i;
                continue_trace(depth + 1);
            }
        }
        // 마킹 해제
        visited[trace[depth]] = 0;
    }
}

int main() {
    scanf("%d %d", &N, &M);

    for (int i = 0; i < N; ++i) {
        visited[i] = 0;
    }

    for (int i = 0; i < N; ++i) {
        trace[0] = i;
        continue_trace(0);
    }

    return 0;
}
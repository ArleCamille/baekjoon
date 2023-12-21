#include <stdio.h>

#define M 6 // N과 M 코드를 응용하기 위해 M을 6으로 고정

int S[13];
int trace[13];
unsigned char visited[13];
int N;

// depth번째의 숫자까지 뽑은 뒤 다음으로 이행하거나 출력.
void continue_trace(int depth) {
    if (depth == M-1) {
        // 0~5번째 숫자, 그러니까 6개의 숫자를 다 뽑았다면
        for (int i = 0; i < M; ++i) {
            // 숫자는 숫자의 번호 (0번째 숫자, 1번째 숫자, ...)로 인코딩되어 있음
            // 실제 숫자는 S[0], S[1] 등...
            printf("%d ", S[trace[i]]);
        }
        printf("\n");
        return;
    } else {
        // 이 이상 쓸 수 있는 숫자가 없다면 (숫자 3개를 뽑아야 하는데 남은 숫자가 2개라든지) 절단
        if (N - trace[depth] < M - depth) return;

        // 그렇지 않다면, 지금 쓰고 있는 숫자를 마킹
        visited[trace[depth]] = 1;
        for (int i = trace[depth] + 1; i < N; ++i) {
            // 지금 쓰고 있는 숫자 다음부터 마킹해가며 다음 단계를 돌려 본다.
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
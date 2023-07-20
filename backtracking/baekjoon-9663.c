#include <stdio.h>

int N;

int queen_y[15];
unsigned char visited[15];

int count = 0;

void iterate(int depth) {
    // 이 자리에 queen을 놓아 본다
    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            // queen이 놓인 적이 없는 y 좌표.
            queen_y[depth] = i;
            for (int j = 0; j <= depth; ++j) {
                for (int k = j + 1; k <= depth; ++k) {
                    if (queen_y[k] == (queen_y[j] + (k - j)) || queen_y[k] == (queen_y[j] - (k - j))) {
                        // 대각선으로 공격 가능
#ifdef DEBUG
                        printf("not satisfied: ");
#endif
                        goto exit_loop;
                    }
                }
            }
            if (depth == N - 1) {
                ++count;
#ifdef DEBUG
                printf("satisfied: ");
#endif
            } else {
                visited[i] = 1;
                iterate(depth + 1);
                visited[i] = 0;
            }
            exit_loop:
#ifdef DEBUG
            printf("y = ");
            for (int i = 0; i <= depth; ++i) {
                printf("%d, ", queen_y[i]);
            }
            printf("\n");
#endif
            continue;
        }
    }
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i)
        visited[i] = 0;
    iterate(0);

    printf("%d\n", count);
}
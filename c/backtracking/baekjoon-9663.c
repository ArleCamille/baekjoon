#include <stdio.h>

int N;

int queen_y[15]; // 각 x좌표에 대한여왕의 y좌표
unsigned char visited[15]; // 이 y좌표에 넣은 적이 있는가?

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
                        goto exit_loop; // 다중 루프 탈출
                    }
                }
            }
            if (depth == N - 1) {
                // depth == N - 1 상황에서 말을 놓아 보았다, 즉 N개의 말을 다 놓았는데 여기까지 왔다는 것은 가능한 경우가 하나 늘었다는 것
                ++count;
#ifdef DEBUG
                printf("satisfied: ");
#endif
            } else {
                // 아직 말을 다 놓아 보지는 않았으니, y좌표 마킹하고 다음 x좌표에 놓아 본다.
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
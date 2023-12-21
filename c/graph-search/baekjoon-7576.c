#include <stdio.h>

int tomato_state[1000][1000];
int max_depth, M, N;

#define QUEUE_SIZE 2000000

struct QUEUE_ITEM {
    int depth;
    short x;
    short y;
} queue[QUEUE_SIZE];
int head, tail;

void push_queue(short x, short y, int depth) {
    if (y < 0 || y >= N || x < 0 || x >= M || tomato_state[y][x] != 0)
        return; // 유효하지 않은 좌표나 유효하지 않은 토마토 상태에 대해서는 그냥 넘어감
    queue[tail].depth = depth;
    queue[tail].x = x;
    queue[tail++].y = y;
    if (tail >= QUEUE_SIZE) {
        // 뒤로 넘어갔으면 다시 0으로 돌아옴
        tail = 0;
    }
}

struct QUEUE_ITEM pop_queue() {
    if (head == tail) {
        return (struct QUEUE_ITEM) { .depth = -1, .x = -1, .y = -1 };
    }
    struct QUEUE_ITEM retval = queue[head++];
    if (head >= QUEUE_SIZE) {
        // 뒤로 넘어갔으면 다시 0으로 돌아옴
        head = 0;
    }
    return retval;
}

int main(void) {
    scanf("%d %d", &M, &N);
    int i, j;
    int head = 0, tail = 0;
    for (i = 0; i < N; ++i)
        for (j = 0; j < M; ++j) {
            scanf("%d", &tomato_state[i][j]);
            // 토마토가 이미 익어 있다면 큐에도 추가
            if (tomato_state[i][j] == 1) {
                // 전후좌우를 하루 뒤 익은 상태로
                push_queue(j - 1, i, 1);
                push_queue(j + 1, i, 1);
                push_queue(j, i - 1, 1);
                push_queue(j, i + 1, 1);
            }
        }

    // 큐로부터 탐색
    for (struct QUEUE_ITEM popped = pop_queue(); popped.depth >= 0; popped = pop_queue()) {
        if (tomato_state[popped.y][popped.x] == 0) {
            // 토마토가 익지 않은 경우만 탐색
            tomato_state[popped.y][popped.x] = 1;
            if (popped.depth > max_depth)
                max_depth = popped.depth;
            // 익음을 전염
            push_queue(popped.x - 1, popped.y, popped.depth + 1);
            push_queue(popped.x + 1, popped.y, popped.depth + 1);
            push_queue(popped.x, popped.y - 1, popped.depth + 1);
            push_queue(popped.x, popped.y + 1, popped.depth + 1);
        }
    }

    // 전수조사
    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            if (tomato_state[i][j] == 0) {
                // 익지 않은 토마토가 있다면 -1 반환 후 끝냄
                max_depth = -1;
            }
        }
    }

    printf("%d\n", max_depth);

    return 0;
}

#include <stdio.h>

int N, K;
unsigned char visited[100001];

#define VALID(node) (node >= 0 && node <= 100000)

struct QUEUE_ITEM {
    int node;
    int depth;
} queue[200000];

int main(void) {
    scanf("%d %d", &N, &K);

    int head = 0, tail = 1;
    // 출발점을 큐에 저장
    queue[0].node = N;
    queue[0].depth = 0;

    struct QUEUE_ITEM popped;
    int i, dest;
    while (head != tail) {
        popped = queue[head++];
        if (!visited[popped.node]) {
            visited[popped.node] = 1;
            if (popped.node == K) {
                // 답을 찾았으니 바로 종료
                printf("%d\n", popped.depth);
                return 0;
            }
            // 3경우에 대해 queue에 넣기
            dest = popped.node - 1;
            if (VALID(dest) && !visited[dest]) {
                queue[tail].node = dest;
                queue[tail++].depth = popped.depth + 1;
            }
            dest = popped.node + 1;
            if (VALID(dest) && !visited[dest]) {
                queue[tail].node = dest;
                queue[tail++].depth = popped.depth + 1;
            }
            dest = popped.node * 2;
            if (VALID(dest) && !visited[dest]) {
                queue[tail].node = dest;
                queue[tail++].depth = popped.depth + 1;
            }
        }
    }
    // FIXME: 이 경우까지 올 가능성이 있음?

    return 0;
}
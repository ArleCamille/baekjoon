#include <stdio.h>

int N, M;
unsigned short edges[100][100];
struct QUEUE_ITEM {
    unsigned char depth;
    unsigned char node;
} queue[5000];
unsigned char visited[100];
unsigned short relationship_number[100];

int main(void) {
    scanf("%d %d", &N, &M);
    int i, start, end;
    for (i = 0; i < M; ++i) {
        scanf("%d %d", &start, &end);
        ++edges[start - 1][end - 1];
        ++edges[end - 1][start - 1];
    }
    // 각 유저로부터 케빈베이컨 수를 더한다.
    struct QUEUE_ITEM popped;
    for (i = 0; i < N; ++i) {
        // 자기 자신에게는 케빈베이컨 수가 0
        start = 0, end = 1;
        queue[0].depth = 0;
        queue[0].node = i;
        while (start != end) {
            // start == end이면 큐가 빈 것
            popped = queue[start++];
            if (visited[popped.node] <= i) {
                visited[popped.node] = i + 1;
                // i번째 노드로부터 여기에 대해서는 아직 케빈 베이컨 수를 안 구했음.
                relationship_number[popped.node] += popped.depth;
                for (int j = 0; j < N; ++j) {
                    if (edges[popped.node][j] && visited[j] <= i) {
                        queue[end].depth = popped.depth + 1;
                        queue[end++].node = j;
                    }
                }
            }
        }
    }

    // ... 그 중에서 케빈 베이컨 수가 가장 작은 사람을 출력한다.
    int min = relationship_number[0];
    int min_node = 0;
    for (i = 1; i < N; ++i) {
        if (relationship_number[i] < min) {
            min = relationship_number[i];
            min_node = i;
        }
    }
    printf("%d\n", min_node + 1);

    return 0;
}
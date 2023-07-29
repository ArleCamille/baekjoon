#include <stdio.h>

int N, M, V;
unsigned short connected[1000][1000];
unsigned char visited[1000];

void dfs(int node) {
    printf("%d ", node + 1);
    visited[node] = 1;
    for (int i = 0; i < N; ++i) {
        if (connected[node][i] && !visited[i])
            dfs(i);
    }
}

int main(void) {
    scanf("%d %d %d", &N, &M, &V);
    // 간선이 중복될 수도 있는데, 간선 갯수를 받는 식으로 대처.
    int start, end, i;
    for (i = 0; i < M; ++i) {
        scanf("%d %d", &start, &end);
        ++connected[start - 1][end - 1];
        ++connected[end - 1][start - 1];
    }
    // 먼저 dfs. 스택을 사용하는게 원칙이나, N이 1000으로 작아 재귀호출로 가능.
    dfs(V - 1);
    puts("");

    // bfs. queue를 사용해서 한 단계씩 전진한다.
    // 간선 갯수가 10000개이고 한 번 들렀던 노드를 다시 탐색할 일은 없으니 고정 10000 크기의 queue를 만든다.
    int queue[10000], node;
    queue[0] = V - 1; // 처음으로 들를 노드
    int head = 0, tail = 1; // tail: 새 원소를 넣어야 하는 곳, head: 첫 원소를 뺄 곳. head == tail이면 큐가 빈 것.
    while (head != tail) {
        // 큐가 비었으면 더 이상 탐사가 불가능하다는 뜻이므로 종료해도 무방.
        node = queue[head++];
        // 아까 DFS에서 visited=1이 들른 상태이므로 이걸 재활용해서 반대로 visited=0을 들른 것으로 처리.
        if (visited[node]) {
            printf("%d ", node + 1);
            visited[node] = 0;
            for (i = 0; i < N; ++i) {
                if (connected[node][i] && visited[i])
                    // 큐에 노드 투입
                    queue[tail++] = i;
            }
        }
    }
    puts("");

    return 0;
}
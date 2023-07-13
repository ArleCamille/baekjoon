#include <stdio.h>

int W[16][16]; // i -> j 이동 비용
int minimal_cost[65536][16]; // 0번은 무조건 기점지로서 지나고, 종점지가 어디였냐에 따라서 minimal_cost값을 따로 보관함.
// (이렇게 안하면 greedy가 돼버리는데, 이건 단순 최단거리 문제에는 적합하지만 TSP에서는 예상치 못한 일을 일으킬 수 있다)
unsigned char visited[65536]; // 이 경로 조합을 방문했는가?
int N; // 그래프 노드 수
int global_bitmask = 0; // 모두 주회를 나타내는 비트마스크

void traversal(int bitmask) {
    if (bitmask == 0)
        return; // 차후에 0을 불러오게 될 것.
    else if (visited[bitmask])
        return;
    int i, j, difference, minimal, result;
    for (i = 0; i < N; ++i) {
        if (bitmask & (1 << i)) {
            // 여기가 패스에서 지나야 할 점이었나?
            difference = bitmask ^ (1 << i);
            traversal(difference);
            minimal = 16000001; // 불가능 상황
            for (j = 0; j < N; ++j) {
                if ((difference & (1 << j)) && W[j][i] > 0) {
                    // j에서 i로 오는 길이 있었고, j가 나머지 패스에 있다.
                    result = minimal_cost[difference][j] + W[j][i];
                    if (result < minimal) {
                        minimal = result;
                    }
                }
            }
            minimal_cost[bitmask][i] = minimal;
        }
    }
    visited[bitmask] = 1;
}

int main(void) {
    scanf("%d", &N);
    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            scanf("%d", &W[i][j]);
    
    // 모든 점을 다 지난 것을 bitmask로 표현한다.
    for (i = 1; i < N; ++i) {
        global_bitmask |= (1 << i); // 낮은 자리부터 이진법으로 나타낼 때 n개의 1을 채움.
        // 0을 제외하고, minimal_cost[단일 노드][i]는 0 -> i를 표현하기로 한다.
        visited[1 << i] = 1;
        if (W[0][i] > 0)
            minimal_cost[1 << i][i] = W[0][i];
        else
            minimal_cost[1 << i][i] = 16000001;
    }

    // 0에 대한 더미 데이터도 채운다.
    visited[1] = 1;
    minimal_cost[1][0] = 16000001;

    // 0번 노드 제외, 모든 노드를 주회한다.
    traversal(global_bitmask);

    // 그 중에 최소 패스를 찾는다.
    int minimum = 16000001;
    for (i = 1; i < N; ++i) {
        if (W[i][0] > 0 && minimal_cost[global_bitmask][i] + W[i][0] < minimum)
            minimum = minimal_cost[global_bitmask][i] + W[i][0];
    }

    printf("%d\n", minimum);

    return 0;
}
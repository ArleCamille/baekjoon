#include <stdio.h>

typedef struct CHILD {
    int child;
    struct CHILD *next;
} child_t;

child_t memory[50];
child_t *children[50];
int next_memory;
int N;

int discover(int node) {
    if (!children[node]) {
        // 부하가 없다면, 소식을 전하지 않으므로 시간이 소요되지 않는다.
        return 0;
    }
    int child_count = 0;
    int subgraph_time[50]; // 각 부하가 자기 부하에게 전달하는 데 필요한 시간.
    for (child_t *iter = children[node]; iter; iter = iter->next) {
        subgraph_time[child_count++] = discover(iter->child);
    }
    // 자신의 직속 부하가 그의 부하들에게 소식을 전하는 데 걸린 시간 역순으로 정렬.
    int i, j, temp;
    for (i = 0; i < child_count; ++i) {
        for (j = i + 1; j < child_count; ++j) {
            if (subgraph_time[i] < subgraph_time[j]) {
                temp = subgraph_time[j];
                subgraph_time[j] = subgraph_time[i];
                subgraph_time[i] = temp;
            }
        }
    }
    // 자신의 모든 부하들에게 소식이 전파되는 데 걸리는 시간은
    // max(1분 + discover(1번째 부하), 2분 + discover(2번째 부하), ...)
    int max_time = subgraph_time[0] + 1;
    for (i = 1; i < child_count; ++i) {
        if (i + 1 + subgraph_time[i] > max_time) {
            max_time = i + 1 + subgraph_time[i];
        }
    }
    return max_time;
}

int main(void) {
    scanf("%d", &N);
    int parent;
    for (int i = 0; i < N; ++i) {
        scanf("%d", &parent);
        if (parent >= 0) {
            memory[next_memory] = (child_t) { .child = i, .next = children[parent] };
            children[parent] = &memory[next_memory++];
        }
    }
    printf("%d\n", discover(0));
}
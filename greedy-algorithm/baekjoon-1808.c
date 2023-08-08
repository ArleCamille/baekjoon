#include <stdio.h>

typedef struct NODE {
    int child;
    struct NODE *prev;
    struct NODE *next;
} node_t;

int depth_of[100];
int parent_of[100];
node_t *edges[100];
node_t actual_list[100];
int max_depth_node, max_depth;
int N, next_node_number, H;
int total_cost;

void dfs_depth(int node, int depth) {
    if (depth > max_depth) {
        max_depth = depth;
        max_depth_node = node;
    }
    depth_of[node] = depth;
    for (node_t *iter = edges[node]; iter != NULL; iter = iter->next) {
        dfs_depth(iter->child, depth + 1);
    }
}

int main(void) {
    scanf("%d", &N);
    int i, j, k;
    for (i = 0; i < N - 1; ++i) {
        scanf("%d %d", &j, &k);
        parent_of[k] = j;
        actual_list[next_node_number] = (node_t) { .child = k, .next = edges[j] };
        if (edges[j])
            edges[j]->prev = &actual_list[next_node_number];
        edges[j] = &actual_list[next_node_number++];
    }
    scanf("%d", &H);
    // 높이가 H 이하가 될 때까지 계속 숌작업을 수행.
    int current_node;
    node_t *iter;
    while(1) {
        // 일단 현 상태의 높이를 구한다
        max_depth_node = 0;
        max_depth = 0;
        dfs_depth(0, 0);
        if (max_depth <= H) {
            printf("%d\n", total_cost);
            return 0;
        }
        // 숌작업을 수행하기 위해, 제일 깊은 노드로부터 깊이 2인 조상을 찾는다.
        for (current_node = max_depth_node; depth_of[current_node] != 2; current_node = parent_of[current_node]) {
        }
        // 숌작업을 수행.
        // 1. 부모로부터 노드 연결을 끊는다.
        for (iter = edges[parent_of[current_node]]; iter != NULL; iter = iter->next)
            if (iter->child == current_node)
                break;
        // 부모에서 간선 제거
        if (iter->prev) {
            iter->prev->next = iter->next;
        } else {
            // 이 원소가 리스트 헤드
            edges[parent_of[current_node]] = iter->next;
        }
        if (iter->next)
            iter->next->prev = iter->prev;
        // 링크가 모두 끊어졌음
        // 2. 루트 노드에 이 노드를 자식으로 하는 새 연결을 수립한다.
        // 메모리 절약을 위해 링크 끊어진 그 노드를 다시 재활용
        iter->prev = NULL;
        iter->next = edges[0];
        edges[0]->prev = iter;
        edges[0] = iter;
        // 3. 부모 관계를 갱신
        parent_of[current_node] = 0;
        // 4. 숌작업 코스트 추가. 높이차가 2이므로 항상 코스트는 1
        ++total_cost;
    }

    // should not reach here
    return 0;
}
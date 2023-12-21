#include <stdio.h>

typedef struct EDGE {
    short parent;
    short child;
    int radius;
    struct EDGE *next;
} edge_t;

edge_t *edge_lists[10000];
edge_t edges[10000];
int next_edge;

unsigned short visited[10000];

int N;

int longest_path_from(int start) {
    int max_length = 0, length;
    for (edge_t *iter = edge_lists[start]; iter != NULL; iter = iter->next) {
        length = iter->radius + longest_path_from(iter->child);
        if (length > max_length)
            max_length = length;
    }
    return max_length;
}

int discover(int node) {
    // 각 노드에 대한 합은 다음 두 가지 중 하나
    // - 내 서브그래프의 최적값
    // - 양쪽 최대거리 서로 더한 것
    int max_length = 0, length;
    // 일단 모든 서브그래프들에 대해 시도
    for (edge_t *iter = edge_lists[node]; iter != NULL; iter = iter->next) {
        length = discover(iter->child);
        if (length > max_length)
            max_length = length;
    }
    // 그 다음, 직접 시도
    int radius1 = 0, radius2 = 0;
    for (edge_t *iter = edge_lists[node]; iter != NULL; iter = iter->next) {
        length = iter->radius + longest_path_from(iter->child);
        if (length > radius1) {
            radius2 = radius1;
            radius1 = length;
        } else if (length > radius2) {
            radius2 = length;
        }
    }
    if (radius1 + radius2 > max_length)
        max_length = radius1 + radius2;
    
    return max_length;
}

int main(void) {
    scanf("%d", &N);
    int parent, child, radius;
    int i, limit = N - 1;
    for (i = 0; i < limit; ++i) {
        scanf("%d %d %d", &parent, &child, &radius);
        --parent, --child;
        edges[next_edge] = (edge_t) { .parent = parent, .child = child, .radius = radius, .next = edge_lists[parent] };
        edge_lists[parent] = &edges[next_edge++];
    }

    printf("%d\n", discover(0));

    return 0;
}
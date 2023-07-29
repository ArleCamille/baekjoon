#include <stdio.h>

struct LINE {
    int to;
    struct LINE *next;
} lines[200000]; // 그래프 간선 구조체

int next_new_line; // 다음으로 할당해야 하는 linked list 원소는 몇 번째?
struct LINE *line_list[100000]; // 각 노드가 가지고 있는 간선 수의 연결 리스트

int N;

int parent_of[100000];
unsigned char visited[100000];

void dfs(int node, int parent) {
    if (visited[node])
        return;
    visited[node] = 1;
    parent_of[node] = parent;
    for (struct LINE *iter = line_list[node]; iter != NULL; iter = iter->next) {
        // 간선을 나타내는 각 연결리스트에 대해 DFS
        dfs(iter->to, node);
    }
}

int main(void) {
    scanf("%d", &N);
    int max_lines = N - 1;
    int i, start, end;
    for (i = 0; i < max_lines; ++i) {
        scanf("%d %d", &start, &end);
        // 간선을 linked list 형태로 저장
        --start;
        --end;
        lines[next_new_line] = (struct LINE) { .to = end, .next = line_list[start] };
        line_list[start] = &lines[next_new_line++];
        lines[next_new_line] = (struct LINE) { .to = start, .next = line_list[end] };
        line_list[end] = &lines[next_new_line++];
    }

    // DFS로 트리를 탐색해서 각 노드의 부모를 찾는다.
    dfs(0, -2);

    // 각 노드에 대해 부모를 출력
    for (i = 1; i < N; ++i)
        printf("%d\n", parent_of[i] + 1);

    return 0;
}
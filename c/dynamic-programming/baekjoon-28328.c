#include <stdio.h>

#define MAXN 250000

typedef struct EDGE {
    int child;
    struct EDGE* prev;
    struct EDGE* next;
} edge_t;
edge_t tree[MAXN * 2];
int next_edge;
edge_t *edge_list[MAXN];
int parent[MAXN];
int visit_order[MAXN], visit_order_max;
int N;

typedef struct PAIR {
    int count;
    unsigned char truth;
} pair_t;
pair_t dp[MAXN], pdp[MAXN];
int onboy[MAXN];

void prune(int start) {
    edge_t *iter, *iter2;
    // 방문 순서 기록
    visit_order[visit_order_max++] = start;
    int child;
    for (iter = edge_list[start]; iter != NULL; iter = iter->next) {
        // 자식으로부터는 부모로 가는 간선을 제거
        child = iter->child;
        // 부모 정보를 대신 입력
        parent[iter->child] = start;
        for (iter2 = edge_list[child]; iter2 != NULL; iter2 = iter2->next) {
            if (iter2->child == start) {
                // 리스트에서 원소 제거
                if (!iter2->prev)
                    edge_list[child] = iter2->next; // 첫 원소였음
                else
                    iter2->prev->next = iter2->next;
                if (iter2->next)
                    iter2->next->prev = iter2->prev;
            }
        }
        // 자식에 대해서도 계속 반복
        prune(child);
    }
}

int main(void) {
    scanf("%d", &N);
    int i, x, y;
    for (i = 1; i < N; ++i) {
        scanf ("%d %d", &x, &y);
        --x;
        --y;
        tree[next_edge] = (edge_t) { .child = y, .next = edge_list[x] };
        if (edge_list[x])
            edge_list[x]->prev = &tree[next_edge];
        edge_list[x] = &tree[next_edge++];
        tree[next_edge] = (edge_t) { .child = x, .next = edge_list[y] };
        if (edge_list[y])
            edge_list[y]->prev = &tree[next_edge];
        edge_list[y] = &tree[next_edge++];
    }

    // 0에서부터 자식들 가지치기
    prune(0);

    // 방문 순서 역순으로 DP를 기록
    edge_t *iter;
    int count;
    unsigned char truth;
    for (i = visit_order_max - 1; i >= 0; --i) {
        x = visit_order[i];
        count = 0;
        truth = 1;
        for (iter = edge_list[x]; iter; iter = iter->next) {
            count += dp[iter->child].count;
            if (dp[iter->child].truth) {
                truth = 0;
                ++onboy[x];
            }
        }
        dp[x] = (pair_t) { .count = count + truth, .truth = truth };
    }
    // 방문 순서 정순대로 PDP를 기록
    for (i = 0; i < visit_order_max; ++i) {
        x = visit_order[i];
        if (!x) continue; // 루트 스킵
        truth = 1;
        count = dp[parent[x]].count - dp[x].count - dp[parent[x]].truth;
        if (onboy[parent[x]] - dp[x].truth > 0) // 나 없어도 자식 중 여전히 포함 있음
            truth = 0;
        count += pdp[parent[x]].count;
        if (pdp[parent[x]].truth)
            truth = 0;
        pdp[x] = (pair_t) { .count = count + truth, .truth = truth };
    }
    // 최대 대비 각 점에서의 값 구하기
    int base = dp[0].count;
    int must = 0;
    int cur;
    for (i = 0; i < N; ++i) {
        cur = pdp[i].count;
        for (iter = edge_list[i]; iter; iter = iter->next)
            cur += dp[iter->child].count;
        if (base > cur) // S에 포함
            ++must;
    }
    long long result = 1LL * N * (N - 1) / 2 - 1LL * must * (must - 1) / 2;
    printf("%lld\n", result);
}
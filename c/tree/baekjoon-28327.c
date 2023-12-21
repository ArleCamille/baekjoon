#include <stdio.h>

#define TREE_ELEMENTS 524288

typedef struct LIST {
    int value;
    int index;
    long long contrib;
    struct LIST *prev;
    struct LIST *next;
} list_t;
list_t actual_list[200000];
list_t *next_list = actual_list;

typedef struct TREE {
    int start;
    int end;
    list_t *left; // 맨 왼쪽의 부분수열
    list_t *right; // 맨 오른쪽의 부분수열
} tree_t;
tree_t tree[TREE_ELEMENTS + 1];

int A[200000], P[200001];
int N;

// 트리 노드 번호 구하기 관련
#define LEFT(node) 2 * (node)
#define RIGHT(node) 2 * (node) + 1
#define PARENT(node) node / 2

void build_tree(int node, int left, int right) {
    tree[node].start = left;
    tree[node].end = right;
    if (left != right) {
        // 자식 노드 있음
        int middle = (left + right) / 2;
        build_tree(LEFT(node), left, middle);
        build_tree(RIGHT(node), middle + 1, right);
    }
}

long long current_value;

// 구간 내의 제일 왼쪽 수열 원소 찾기
list_t *search_left(int node, int start, int end) {
    if ((tree[node].start == start && tree[node].end == end)) {
        return tree[node].left;
    }
    int left = LEFT(node);
    int right = RIGHT(node);
    list_t *retval = NULL;
    if (start <= tree[left].end) {
        retval = search_left(left, start, tree[left].end);
        if (retval) return retval;
        else return search_left(right, tree[right].start, end);
    } else return search_left(right, start, end);
}

// 구간 내의 제일 오른쪽 수열 원소 찾기
list_t *search_right(int node, int start, int end) {
    if ((tree[node].start == start && tree[node].end == end))
        return tree[node].right;
    int left = LEFT(node);
    int right = RIGHT(node);
    list_t *retval = NULL;
    if (end >= tree[right].start) {
        retval = search_right(right, tree[right].start, end);
        if (retval) return retval;
        else return search_right(left, start, tree[left].end);
    }
    return search_right(left, start, end);
}

void update_tree(list_t *new_element, int node) {
    if (tree[node].start == tree[node].end) { // 말단 노드
        tree[node].left = tree[node].right = new_element;
        return;
    }
    // 적절한 자식 업데이트
    int left_child = LEFT(node);
    if (new_element->index <= tree[left_child].end)
        update_tree(new_element, left_child);
    else
        update_tree(new_element, RIGHT(node));
    // 그 다음 나도 업데이트
    if (!tree[node].left || new_element->index < tree[node].left->index)
        tree[node].left = new_element;
    if (!tree[node].right || new_element->index > tree[node].right->index)
        tree[node].right = new_element;
}

int main(void) {
    scanf("%d", &N);
    int i;
    for (i = 0; i < N; ++i) {
        // A와 P를 동시에 받음
        scanf("%d", &A[i]);
        P[A[i]] = i;
    }

    // 세그먼트 트리 구축
    build_tree(1, 0, N - 1);

    // g(x)를 구함
    int x;
    long long new_contrib, contrib_diff, current_contrib;
    list_t *left, *right, *right2, *left2;
    int Pcurrent, Pleft, Xleft, Pleft2, Xleft2, Pright, Xright, Pright2, Xright2;
    long long Cleft, Cleft2;
    for (x = 1; x <= N; ++x) {
        Pcurrent = P[x];
        if (Pcurrent > 0)
            left = search_right(1, 0, Pcurrent - 1);
        else
            left = NULL;
        if (Pcurrent < N - 1)
            right = search_left(1, Pcurrent + 1, N - 1);
        else
            right = NULL;
#ifdef DEBUG
        printf("Adding new node for %d at %d\n", x, Pcurrent);
#endif
        *next_list = (list_t) { .index = Pcurrent, .value = x, .prev = left, .next = right };
        update_tree(next_list, 1);
        if (left) {
            left->next = next_list;
            left2 = left->prev;
            if (left2) {
                Pleft2 = left2->index;
                Xleft2 = left2->value;
                Cleft2 = left2->contrib;
            } else
                Pleft2 = -1;
            Pleft = left->index;
            Xleft = left->value;
            Cleft = left->contrib;
#ifdef DEBUG
            printf("Left: %d at %d\n", left->value, left->index);
            if (left2)
                printf("Left of left: %d at %d\n", left2->value, left2->index);
            else
                puts("No left of left");
#endif
        } else {
            left2 = NULL;
            Pleft = Pleft2 = -1;
#ifdef DEBUG
            puts("No left");
#endif
        }
        if (right) {
            right->prev = next_list;
            right2 = right->next;
            if (right2) {
                Pright2 = right2->index;
                Xright2 = right2->value;
            } else
                Pright2 = -1;
#ifdef DEBUG
            printf("Right: %d at %d\n", right->value, right->index);
            if (right2)
                printf("Right of right: %d at %d\n", right2->value, right2->index);
            else
                puts("No right of right");
#endif
            Pright = right->index;
            Xright = right->value;
        } else {
            right2 = NULL;
            Pright = Pright2 = -1;
#ifdef DEBUG
            puts("No right");
#endif
        }

        // 현재 값의 기여도 계산
        current_contrib = (long long) (Pcurrent + 1) * (N - Pcurrent);
        if (Pright2 >= 0 && ((x < Xright && Xright < Xright2) || (x > Xright && Xright > Xright2)))
            // 지그재그를 방해하는 수; 기여도에서 그만큼 감산
            current_contrib -= (long long) (Pcurrent + 1) * (N - Pright2);
        next_list->contrib = current_contrib;

        // 인접 값들의 기여도 업데이트
        contrib_diff = 0;
        if (Pleft >= 0) {
            new_contrib = (long long) (Pleft + 1) * (N - Pleft);
            if (Pright >= 0 && ((Xleft < x && x < Xright) || (Xleft > x && x > Xright)))
                new_contrib -= (long long) (Pleft + 1) * (N - Pright);
            contrib_diff = new_contrib - Cleft;
            left->contrib = new_contrib;
        }
        if (Pleft2 >= 0) {
            new_contrib = (long long) (Pleft2 + 1) * (N - Pleft2);
            if ((Xleft2 < Xleft && Xleft < x) || (Xleft2 > Xleft && Xleft > x))
                new_contrib -= (long long) (Pleft2 + 1) * (N - Pcurrent);
            contrib_diff += new_contrib - Cleft2;
            left2->contrib = new_contrib;
        }

        // g(x) = g(x-1) + 인접 값들 기여도 변화 + 현재 값 기여도
        current_value += contrib_diff + current_contrib;
        printf("%lld\n", current_value);
        
        ++next_list;
    }

    return 0;
}
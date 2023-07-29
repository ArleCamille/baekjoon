#include <stdio.h>
struct NODE {
    short left;
    short right;
    short parent;
    unsigned short coord;
    short depth;
} tree[10000];

struct LEVEL_INFO {
    short leftmost;
    short rightmost;
} depth_info[10001];

unsigned short current_x = 1;
unsigned short deepest_level = 0;
int N;

void tree_scan(short node, short depth) {
    if (node < 0)
        return; // 무효 좌표에 대해서는 처리 안 함
    // 깊이 정보 update
    tree[node].depth = depth;
    deepest_level = (depth > deepest_level) ? depth : deepest_level;
    // 왼쪽 자손 탐사
    tree_scan(tree[node].left, depth + 1);
    // 현재 노드에 대해 갖가지 값 업데이트
    tree[node].coord = current_x;
    if (depth_info[depth].leftmost == 0) {
        // 초기값 미세팅
        depth_info[depth] = (struct LEVEL_INFO) { current_x, current_x };
    } else {
        if (current_x < depth_info[depth].leftmost) // 왼쪽 갱신
            depth_info[depth].leftmost = current_x;
        if (current_x > depth_info[depth].rightmost) // 오른쪽 갱신
            depth_info[depth].rightmost = current_x;
    }
    ++current_x;
    // 오른쪽 자손 탐사
    tree_scan(tree[node].right, depth + 1);
}

int main(void) {
    // 노드 갯수 입력
    scanf("%d", &N);
    int id, left, right, i;
    for (i = 0; i < N; ++i)
        tree[i].parent = -1;
    for (i = 0; i < N; ++i) {
        scanf("%d %d %d", &id, &left, &right);
        tree[id-1].left = left-1;
        tree[id-1].right = right-1;
        tree[left-1].parent = tree[right-1].parent = id-1;
    }

    // 부모가 없는 노드를 찾는다.
    for (i = 0; i < N; ++i) {
        if (tree[i].parent < 0) {
            // 부모가 없는 노드가 루트이므로 거기서부터 탐색
            tree_scan(i, 1);
            break;
        }
    }

    // depth에 관한 정보가 찼으니, 여기에 입각해서 너비 데이터 출력
    short max_diff = depth_info[1].rightmost - depth_info[1].leftmost + 1;
    short max_depth = 1;
    short diff;
    for (i = 2; i <= deepest_level; ++i) {
        diff = depth_info[i].rightmost - depth_info[i].leftmost + 1;
        if (diff > max_diff) {
            max_diff = diff;
            max_depth = i;
        }
    }

    printf("%d %d\n", max_depth, max_diff);

    return 0;
}
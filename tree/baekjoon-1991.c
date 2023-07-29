#include <stdio.h>
struct NODE {
    char left;
    char right;
} tree[26];

void preorder(int node) {
    if (node < 0 || node >= 26)
        return;
    
    // 루트 출력
    char name = 'A' + node;
    printf("%c", name);

    // 왼쪽 출력
    preorder(tree[node].left);

    // 오른쪽 출력
    preorder(tree[node].right);
}

void inorder(int node) {
    if (node < 0 || node >= 26)
        return;

    // 왼쪽 출력
    inorder(tree[node].left);

    // 루트 출력
    char name = 'A' + node;
    printf("%c", name);

    // 오른쪽 출력
    inorder(tree[node].right);
}

void postorder(int node) {
    if (node < 0 || node >= 26)
        return;
    
    // 왼쪽 출력
    postorder(tree[node].left);

    // 오른쪽 출력
    postorder(tree[node].right);

    // 루트 출력
    char name = 'A' + node;
    printf("%c", name);
}

int main(void) {
    int N; // 입력 줄수
    scanf("%d", &N);
    char buffer_parent[10], buffer_left[10], buffer_right[10];
    int index;
    for (int i = 0; i < N; ++i) {
        scanf("%s %s %s", buffer_parent, buffer_left, buffer_right);
        index = buffer_parent[0] - 'A';
        tree[index].left = buffer_left[0] - 'A';
        tree[index].right = buffer_right[0] - 'A';
    }

    preorder(0);
    puts("");

    inorder(0);
    puts("");

    postorder(0);
    puts("");

    return 0;
}
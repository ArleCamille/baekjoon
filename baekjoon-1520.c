#include <stdio.h>

int M, N; // 행, 열 갯수.
int height[500][500]; // 높이들. [행][열] 순으로.
int paths[500][500]; // 결과값.
unsigned char visited[500][500]; // 노드 방문 여부. 결과값이 0인 경우가 있을 수 있기에 이걸로 대신 체크.

int get_paths(int y, int x) {
    if (x < 0 || y < 0 || x >= N || y >= M)
        return 0; // out of bounds; invalid
    else if (x == 0 && y == 0)
        return 1; // 원점에서 출발하는 경우니, 경우는 하나
    else if (visited[y][x]) // 이미 들른 적 있음
        return paths[y][x];
    
    visited[y][x] = 1;

    int cases = 0;
    if (height[y][x] < height[y - 1][x]) // 위에서 올 수 있음
        cases += get_paths(y - 1, x);
    if (height[y][x] < height[y + 1][x]) // 아래에서 올 수 있음
        cases += get_paths(y + 1, x);
    if (height[y][x] < height[y][x - 1]) // 왼쪽에서 올 수 있음
        cases += get_paths(y, x - 1);
    if (height[y][x] < height[y][x + 1]) // 오른쪽에서 올 수 있음
        cases += get_paths(y, x + 1);

    paths[y][x] = cases;
    
    return cases;
}

int main(void) {
    scanf("%d %d", &M, &N);
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            scanf("%d", &height[i][j]);
    
    printf("%d\n", get_paths(M - 1, N - 1));

    return 0;
}
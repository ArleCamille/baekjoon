#include <stdio.h>

unsigned char grids[100][100];
unsigned short visited[100][100];
unsigned short surface[10000];
int num_surfaces, M, N;

void dfs(int y, int x, int surface_code) {
    if (visited[y][x] || x < 0 || x >= N || y < 0 || y >= M)
        return; // 이미 들렀거나, 무효 좌표면 즉시 return
    
    visited[y][x] = 1;
    ++surface[surface_code];
    dfs(y + 1, x, surface_code);
    dfs(y - 1, x, surface_code);
    dfs(y, x + 1, surface_code);
    dfs(y, x - 1, surface_code);
}

int main(void) {
    int K;
    scanf("%d %d %d", &M, &N, &K);
    int left, bottom, top, right;
    int i, j, k;
    for (i = 0; i < K; ++i) {
        scanf("%d %d %d %d", &left, &bottom, &right, &top);
        for (j = bottom; j < top; ++j)
            for (k = left; k < right; ++k) {
                grids[j][k] = 1;
                visited[j][k] = 1;
            }
    }

    // 각 작은 정사각형에 대해서 인근을 DFS로 방문.
    for (j = 0; j < M; ++j) {
        for (k = 0; k < N; ++k) {
            if (!visited[j][k]) {
                dfs(j, k, num_surfaces++);
            }
        }
    }

    // 영역 갯수를 우선 출력.
    printf("%d\n", num_surfaces);

    // 영역 넓이를 소팅
    unsigned short temp;
    for (i = 0; i < num_surfaces; ++i) {
        for (j = i + 1; j < num_surfaces; ++j) {
            if (surface[j] < surface[i]) {
                temp = surface[j];
                surface[j] = surface[i];
                surface[i] = temp;
            }
        }
    }

    for (i = 0; i < num_surfaces; ++i)
        printf("%d ", surface[i]);
    
    puts("");

    return 0;
}
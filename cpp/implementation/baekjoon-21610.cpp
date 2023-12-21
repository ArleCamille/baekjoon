#include <iostream>
#include <cstring>

enum Directions {
    DIRECTION_WEST = 1,
    DIRECTION_NORTHWEST = 2,
    DIRECTION_NORTH = 3,
    DIRECTION_NORTHEAST = 4,
    DIRECTION_EAST = 5,
    DIRECTION_SOUTHEAST = 6,
    DIRECTION_SOUTH = 7,
    DIRECTION_SOUTHWEST = 8
};

int A[50][50]; // 원래 바구니에 있었던 물의 양
char clouds[50][50]; // 깔린 구름. 처음에는 (0, N - 2) ~ (1, N - 1)에 있음.
char previousClouds[50][50]; // 이전 구름. 새 구름을 만들 때 여기엔 생기면 안 돼서.
char newClouds[50][50]; // 새 구름. moveClouds에서 쓰는 임시 변수.
int N; // 격자 사이즈
int M; // iteration 횟수

inline int safeGet(int x, int y) {
    if (x < 0 || x >= N || y < 0 || y >= N)
        return 0;
    return A[y][x];
}

// 1번 절차에서 필요한 구름 옮기기 과정
void moveClouds(Directions direction, int distance) {
    // 이동 대상 구름을 초기화.
    memset(newClouds, 0, sizeof (newClouds));
    int i, j, newY, newX;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (clouds[i][j]) {
                // 가로 방향
                switch (direction) {
                case DIRECTION_WEST:
                case DIRECTION_NORTHWEST:
                case DIRECTION_SOUTHWEST:
                    newX = j - distance;
                    break;
                case DIRECTION_EAST:
                case DIRECTION_NORTHEAST:
                case DIRECTION_SOUTHEAST:
                    newX = j + distance;
                    break;
                default: // NORTH, SOUTH
                    newX = j;
                    break;
                }
                // 세로 방향
                switch (direction) {
                case DIRECTION_NORTHWEST:
                case DIRECTION_NORTH:
                case DIRECTION_NORTHEAST:
                    newY = i - distance;
                    break;
                case DIRECTION_SOUTHWEST:
                case DIRECTION_SOUTH:
                case DIRECTION_SOUTHEAST:
                    newY = i + distance;
                    break;
                default: // WEST, EAST
                    newY = i;
                    break;
                }
                // out of bounds를 안으로 끌어오기
                while (newX < 0)
                    newX += N;
                while (newX >= N)
                    newX -= N;
                while (newY < 0)
                    newY += N;
                while (newY >= N)
                    newY -= N;
                // 이동
                newClouds[newY][newX] = 1;
            }
        }
    }
    // clouds 쪽으로 다시 되돌린다.
    memcpy(clouds, newClouds, sizeof (clouds));
}

int main(void) {
    std::cin >> N >> M; // grid 크기와 iteration 횟수 구하기
    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            std::cin >> A[i][j]; // grid 입력받기. 위가 0행, 왼쪽이 0열.

    // 구름 초기 위치 셋팅
    clouds[N - 2][0] = 1;
    clouds[N - 2][1] = 1;
    clouds[N - 1][0] = 1;
    clouds[N - 1][1] = 1;
    
    int _dir, dist;
    Directions dir;
    for (int iterations = 0; iterations < M; ++iterations) {
        // 1. 구름을 방향과 거리를 받아 이동.
        std::cin >> _dir >> dist;
        dir = static_cast<Directions>(_dir);
        moveClouds(dir, dist);

        // 2. 구름이 있는 자리에는 비가 내려 물이 1 증가.
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                if (clouds[i][j])
                    ++A[i][j];
            }
        }

        // 3. 구름이 모두 사라진다. (하지만 5에서 필요하므로 previousClouds로 옮긴다.)
        memcpy(previousClouds, clouds, sizeof (clouds));
        memset(clouds, 0, sizeof (clouds));

        // 4. 2에서 비가 내렸던 곳 (previousClouds로 마킹)에 대각선 사방으로
        //    물 존재 여부에 따라 물을 1씩 추가한다.
        //    물이 0에서 1이 될 가능성은 없으니 실시간으로 해도 된다.
        //    (애초에 2번 과정에서 물은 최소 1이 되었어야 한다.)
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                if (previousClouds[i][j]) {
                    if (safeGet(j - 1, i - 1) > 0)
                        ++A[i][j];
                    if (safeGet(j + 1, i - 1) > 0)
                        ++A[i][j];
                    if (safeGet(j + 1, i + 1) > 0)
                        ++A[i][j];
                    if (safeGet(j - 1, i + 1) > 0)
                        ++A[i][j];
                }
            }
        }

        // 5. 2에서 구름이 있었지 아니한 자리 중 물이 2 이상인 곳에 물 2를 빼고 구름 추가.
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j) {
                if (!previousClouds[i][j] && A[i][j] >= 2) {
                    A[i][j] -= 2;
                    clouds[i][j] = 1;
                }
            }
        }
    }
    // 전부 처리한 뒤에는 모든 바구니의 물의 양의 합을 구해야 한다.
    int sum = 0;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            sum += A[i][j];
    
    // 출력하고 반환.
    std::cout << sum << std::endl;
    return 0;
}
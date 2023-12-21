#include <iostream>
#include <cstring>
#include <list>

struct Coordinates {
    int x;
    int y;
    Coordinates(int x, int y) {
        this->x = x;
        this->y = y;
    }
    Coordinates() {
        this->x = 0;
        this->y = 0;
    }
};

int R, C; // R: 행 수, C: 열 수
int K; // 기준 온도. 모든 온도가 측정값 이상이면 종료.
char wallInfo[20][20]; // 0b0000남북서동 순으로 벽 여부를 나타냄.

#define EAST_WALL 0b0001
#define WEST_WALL 0b0010
#define NORTH_WALL 0b0100
#define SOUTH_WALL 0b1000

int thermo[20][20], tempThermo[20][20];
char visitMap[20][20]; // 온풍기 방출 단계에서 사용. 여기로 이미 바람이 왔었는가?

struct HeaterInfo {
    Coordinates coords;
    char direction; // 온풍기 방향. 0: 동, 1: 서, 2: 북, 3: 남
    HeaterInfo(int x, int y, char dir) {
        this->coords = Coordinates(x, y);
        this->direction = dir;
    }
};

std::list<HeaterInfo> heaters; // 온풍기 정보
std::list<Coordinates> testGrids; // 온도를 조사해야 하는 칸

inline bool isValidCoordinates(int x, int y) {
    return x >= 0 && x < C && y >= 0 && y < R;
}

void spreadWind(int x, int y, int windPower, char direction) {
    if (windPower <= 0 || !isValidCoordinates(x, y) || visitMap[y][x])
        return;
    visitMap[y][x] = 1; // 이미 들른 곳은 추가로 들르지 않는다
    thermo[y][x] += windPower; // 바람으로 덥히기
    switch (direction) {
    case 0: // 동쪽
        // 대각선 이동은 거치는 길(ㄱ자)에 전부 벽이 없어야 한다.
        // NE 이동
        if (y > 0 && !(wallInfo[y][x] & NORTH_WALL) && !(wallInfo[y - 1][x] & EAST_WALL))
            spreadWind(x + 1, y - 1, windPower - 1, 0);
        // E 이동
        if (!(wallInfo[y][x] & EAST_WALL))
            spreadWind(x + 1, y, windPower - 1, 0);
        // SE 이동
        if (y < R - 1 && !(wallInfo[y][x] & SOUTH_WALL) && !(wallInfo[y + 1][x] & EAST_WALL))
            spreadWind(x + 1, y + 1, windPower - 1, 0);
        break;
    case 1: // 서쪽
        // NW 이동
        if (y > 0 && !(wallInfo[y][x] & NORTH_WALL) && !(wallInfo[y - 1][x] & WEST_WALL))
            spreadWind(x - 1, y - 1, windPower - 1, 1);
        // W 이동
        if (!(wallInfo[y][x] & WEST_WALL))
            spreadWind(x - 1, y, windPower - 1, 1);
        // SW 이동
        if (y < R - 1 && !(wallInfo[y][x] & SOUTH_WALL) && !(wallInfo[y + 1][x] & WEST_WALL))
            spreadWind(x - 1, y + 1, windPower - 1, 1);
        break;
    case 2: // 북쪽
        // NW 이동
        if (x > 0 && !(wallInfo[y][x] & WEST_WALL) && !(wallInfo[y][x - 1] & NORTH_WALL))
            spreadWind(x - 1, y - 1, windPower - 1, 2);
        // N 이동
        if (!(wallInfo[y][x] & NORTH_WALL))
            spreadWind(x, y - 1, windPower - 1, 2);
        // NE 이동
        if (x < C - 1 && !(wallInfo[y][x] & EAST_WALL) && !(wallInfo[y][x + 1] & NORTH_WALL))
            spreadWind(x + 1, y - 1, windPower - 1, 2);
        break;
    case 3: // 남쪽
        // SW 이동
        if (x > 0 && !(wallInfo[y][x] & WEST_WALL) && !(wallInfo[y][x - 1] & SOUTH_WALL))
            spreadWind(x - 1, y + 1, windPower - 1, 3);
        // S 이동
        if (!(wallInfo[y][x] & SOUTH_WALL))
            spreadWind(x, y + 1, windPower - 1, 3);
        // SE 이동
        if (x < C - 1 && !(wallInfo[y][x] & EAST_WALL) && !(wallInfo[y][x + 1] & SOUTH_WALL))
            spreadWind(x + 1, y + 1, windPower - 1, 3);
        break;
    }
}

int main(void) {
    // R, C, K를 먼저 받아옴.
    std::cin >> R >> C >> K;
    int i, j, tmp;
    for (i = 0; i < R; ++i) {
        // 각 행 각 칸의 정보를 받아온다.
        for (j = 0; j < C; ++j) {
            std::cin >> tmp;
            switch (tmp) {
            case 1: // 동쪽 온풍기
                heaters.push_back(HeaterInfo(j, i, 0));
                break;
            case 2: // 서쪽 온풍기
                heaters.push_back(HeaterInfo(j, i, 1));
                break;
            case 3: // 북쪽 온풍기
                heaters.push_back(HeaterInfo(j, i, 2));
                break;
            case 4: // 남쪽 온풍기
                heaters.push_back(HeaterInfo(j, i, 3));
                break;
            case 5: // 조사 대상 칸
                testGrids.push_back(Coordinates(j, i));
                break;
            }
        }
    }
    int numWalls; // 받아야 하는 벽의 수
    std::cin >> numWalls;
    int r, c;
    for (i = 0; i < numWalls; ++i) {
        std::cin >> r >> c >> tmp;
        switch (tmp) {
        case 0: // 가로벽: (r, c)와 (r-1, c) 사이에 벽이 있다는 것.
            wallInfo[r - 2][c - 1] |= SOUTH_WALL; // r-1, c 쪽에서는 남쪽 벽
            wallInfo[r - 1][c - 1] |= NORTH_WALL; // r, c 쪽에서는 북쪽 벽
            break;
        case 1: // 세로벽: (r, c)와 (r, c + 1) 사이에 벽이 있다는 것.
            wallInfo[r - 1][c - 1] |= EAST_WALL; // r, c 쪽에서는 동쪽 벽
            wallInfo[r - 1][c]     |= WEST_WALL; // r, c + 1 쪽에서는 서쪽 벽
            break;
        }
    }

    int iterations = 0; // "초콜릿의 갯수"
    while (iterations <= 100) {
        // 1. 집에 있는 모든 온풍기에서 바람이 한 번 나온다.
        for (auto heaterIter = heaters.cbegin(); heaterIter != heaters.cend(); heaterIter++) {
            // 온풍기 바로 앞은 막혀있지 않는게 assumption이므로, 안심하고 온풍기 바로 앞부터 검사한다.
            // 그 전에 visit map을 클리어.
            memset(visitMap, 0, sizeof (visitMap));
            switch (heaterIter->direction) {
            case 0:
                // 동, 시작 좌표 오른쪽
                spreadWind(heaterIter->coords.x + 1, heaterIter->coords.y, 5, 0);
                break;
            case 1:
                // 서, 시작 좌표 왼쪽
                spreadWind(heaterIter->coords.x - 1, heaterIter->coords.y, 5, 1);
                break;
            case 2:
                // 북, 시작 좌표 위쪽
                spreadWind(heaterIter->coords.x, heaterIter->coords.y - 1, 5, 2);
                break;
            case 3:
                // 남, 시작 좌표 아래쪽
                spreadWind(heaterIter->coords.x, heaterIter->coords.y + 1, 5, 3);
                break;
            }
        }

#ifdef DEBUG
        std::cout << "DEBUG: thermo map after heater spread" << std::endl;
        for (i = 0; i < R; ++i) {
            for (j = 0; j < C; ++j)
                std::cout << thermo[i][j] << '\t';
            std::cout << std::endl;
        }
        std::cout << std::endl;
#endif

        // 2. 온도가 조절된다.
        // 벽으로 막혀있지 않은 인접 셀에 대해 차이 / 4만큼 온도가 조절됨.
        // 모든 과정이 동시에 일어나므로 일단 tempThermo로 현재 온도 지도를 copy
        std::memcpy(tempThermo, thermo, sizeof (thermo));
        // 위/아래 처리
        for (i = 0; i < R - 1; ++i) {
            for (j = 0; j < C; ++j) {
                if (wallInfo[i][j] & SOUTH_WALL)
                    continue; // (j, i)와 (j, i + 1) 사이가 막혀 있으면 X
                if (thermo[i][j] > thermo[i + 1][j]) { // 위가 더 따뜻
                    tmp = (thermo[i][j] - thermo[i + 1][j]) / 4;
                    tempThermo[i][j] -= tmp;
                    tempThermo[i + 1][j] += tmp; // 아래가 더 추움
                } else {
                    // 아래가 더 따뜻
                    tmp = (thermo[i + 1][j] - thermo[i][j]) / 4;
                    tempThermo[i][j] += tmp;
                    tempThermo[i + 1][j] -= tmp;
                }
            }
        }
        // 왼쪽 / 오른쪽 처리
        for (i = 0; i < R; ++i) {
            for (j = 0; j < C - 1; ++j) {
                if (wallInfo[i][j] & EAST_WALL)
                    continue; // (j, i)와 (j + 1, i) 사이가 막혀 있으면 X
                if (thermo[i][j] > thermo[i][j + 1]) {
                    // 왼쪽이 더 따뜻
                    tmp = (thermo[i][j] - thermo[i][j + 1]) / 4;
                    tempThermo[i][j] -= tmp;
                    tempThermo[i][j + 1] += tmp;
                } else {
                    // 오른쪽이 더 따뜻
                    tmp = (thermo[i][j + 1] - thermo[i][j]) / 4;
                    tempThermo[i][j] += tmp;
                    tempThermo[i][j + 1] -= tmp;
                }
            }
        }
        // 처리한 값을 다시 thermo로 옮겨준다.
        std::memcpy(thermo, tempThermo, sizeof (thermo));

        // 3. 온도가 1 이상인 가장 바깥쪽 칸의 온도가 1씩 감소.
        for (i = 0; i < R; ++i) {
            if (thermo[i][0] > 0)
                --thermo[i][0];
            if (thermo[i][C - 1] > 0)
                --thermo[i][C - 1];
        }
        for (i = 1; i < C - 1; ++i) {
            // x = 0과 x = C - 1은 이미 위에서 했으므로 중복해서 안 하게 조심!
            if (thermo[0][i] > 0)
                --thermo[0][i];
            if (thermo[R - 1][i] > 0)
                --thermo[R - 1][i];
        }

        // 4. "초콜릿을 하나 먹는다." (iterations 증가 효과)
        ++iterations;

        // 5. 모든 칸의 온도가 K 이상인지 검사.
#ifdef DEBUG
        std::cout << "DEBUG: thermo map at iteration " << iterations << std::endl;
        for (i = 0; i < R; ++i) {
            for (j = 0; j < C; ++j)
                std::cout << thermo[i][j] << '\t';
            std::cout << std::endl;
        }
        std::cout << std::endl;
#endif

        bool testFailed = false;
        for (auto testIter = testGrids.cbegin(); testIter != testGrids.cend(); testIter++) {
            if (thermo[testIter->y][testIter->x] < K) {
                testFailed = true;
                break;
            }
        }
        if (!testFailed)
            break;
    }
    // 먹은 "초콜릿의 개수"를 출력하고 종료.
    std::cout << iterations << std::endl;
    return 0;
}
#include <iostream>
#include <vector>

struct Dice {
public:
    int top;
    int east;
    int west;
    int north;
    int south;
    int bottom;
    Dice() {
        this->top = 1;
        this->east = 3;
        this->north = 2;
        this->west = 4;
        this->south = 5;
        this->bottom = 6;
    }

    Dice rolledEast() {
        return Dice(this->west,
            this->top,
            this->bottom,
            this->north,
            this->south,
            this->east);
    }

    Dice rolledWest() {
        return Dice(this->east,
            this->bottom,
            this->top,
            this->north,
            this->south,
            this->west);
    }

    Dice rolledNorth() {
        return Dice(this->south,
            this->east,
            this->west,
            this->top,
            this->bottom,
            this->north);
    }

    Dice rolledSouth() {
        return Dice(this->north,
            this->east,
            this->west,
            this->bottom,
            this->top,
            this->south);
    }
private:
    Dice(int t, int e, int w, int n, int s, int b) {
        this->top = t;
        this->east = e;
        this->west = w;
        this->north = n;
        this->south = s;
        this->bottom = b;
    }
};

#define EAST 0
#define SOUTH 1
#define WEST 2
#define NORTH 3

int N, M; // N: 행 수, M: 열 수
int K; // 주사위의 총 이동 횟수
int map[20][20]; // 지도에 쓰여 있는 자연수들
int sectorScores[20][20]; // 현재 칸에서 동서남북으로 이동할 수 있는 모든 칸들의 수
int sectorIdentifiers[20][20]; // 중간 탐색 과정에서 섹터 번호별 마킹용
std::vector<int> sectorCount = {0}; // 0번 섹터는 "아직 마킹 안함"이라는 뜻이다.
int numberOfSectors; // 섹터 갯수

int currentX, currentY; // 현재 좌표. (0, 0)에서 시작.
int currentDirection = EAST; // 현재 방향. 동쪽에서 시작.
int currentScore;
Dice currentDice = Dice();

inline bool isValidCoordinates(int x, int y) {
    return x >= 0 && x < M && y >= 0 && y < N;
}

void markSector(int sector, int x, int y) {
    if (!isValidCoordinates(x, y) || sectorIdentifiers[y][x] != 0)
        return;
    sectorIdentifiers[y][x] = sector;
    ++sectorCount[sector];
    // 왼쪽 마킹
    if (x > 0 && map[y][x - 1] == map[y][x])
        markSector(sector, x - 1, y);
    // 오른쪽 마킹
    if (x < M - 1 && map[y][x + 1] == map[y][x])
        markSector(sector, x + 1, y);
    // 위쪽 마킹
    if (y > 0 && map[y - 1][x] == map[y][x])
        markSector(sector, x, y - 1);
    // 아래쪽 마킹
    if (y < N - 1 && map[y + 1][x] == map[y][x])
        markSector(sector, x, y + 1);
}

int main(void) {
    std::cin >> N >> M >> K;

    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < M; ++j)
            std::cin >> map[i][j];
    
    // 맵을 불러왔으니, 각 섹터를 돌면서 섹터마킹을 한다.
    for (i = 0; i < N; ++i) {
        for (j = 0; j < M; ++j) {
            if (sectorIdentifiers[i][j] == 0) {
                // 새로운 섹터 등장
                sectorCount.push_back(0);
                markSector(++numberOfSectors, j, i);
            }
        }
    }
    // 각 칸마다 섹터에 속한 칸 갯수에 따라 점수를 매긴다.
    for (i = 0; i < N; ++i)
        for (j = 0; j < M; ++j)
            sectorScores[i][j] = sectorCount[sectorIdentifiers[i][j]] * map[i][j];
    
    for (int I = 0; I < K; ++I) {
        // 각 이동 횟수마다, 주사위가 한 칸 굴러간다.
        switch (currentDirection) {
        case EAST:
            if (!isValidCoordinates(currentX + 1, currentY)) {
                // 대신 서쪽으로
                currentDirection = WEST;
                --currentX;
            } else ++currentX;
            break;
        case WEST:
            if (!isValidCoordinates(currentX - 1, currentY)) {
                // 대신 동쪽으로    
                currentDirection = EAST;
                ++currentX;
            } else --currentX;
            break;
        case NORTH:
            if (!isValidCoordinates(currentX, currentY - 1)) {
                // 대신 남쪽으로
                currentDirection = SOUTH;
                ++currentY;
            } else --currentY;
            break;
        case SOUTH:
            if (!isValidCoordinates(currentX, currentY + 1)) {
                // 대신 북쪽으로
                currentDirection = NORTH;
                --currentY;
            } else ++currentY;
            break;
        }
        // 진행 방향으로 주사위를 굴린다.
        // 이전 switch문에서 방향이 변했을 수 있으니 switch문을 다시 쓴다.
        switch (currentDirection) {
        case EAST:
            currentDice = currentDice.rolledEast();
            break;
        case WEST:
            currentDice = currentDice.rolledWest();
            break;
        case NORTH:
            currentDice = currentDice.rolledNorth();
            break;
        case SOUTH:
            currentDice = currentDice.rolledSouth();
            break;
        }
        // 도착점의 점수를 획득한다.
        currentScore += sectorScores[currentY][currentX];
        // 아랫면과 판의 정수를 비교해 이동 방향을 결정한다.
        if (currentDice.bottom > map[currentY][currentX]) {
            // A>B, 90도 시계방향
            switch (currentDirection) {
            case EAST:
                currentDirection = SOUTH;
                break;
            case SOUTH:
                currentDirection = WEST;
                break;
            case WEST:
                currentDirection = NORTH;
                break;
            case NORTH:
                currentDirection = EAST;
                break;
            }
        } else if (currentDice.bottom < map[currentY][currentX]) {
            // A<B, 90도 반시계방향
            switch (currentDirection) {
            case EAST:
                currentDirection = NORTH;
                break;
            case NORTH:
                currentDirection = WEST;
                break;
            case WEST:
                currentDirection = SOUTH;
                break;
            case SOUTH:
                currentDirection = EAST;
                break;
            }
        } // A=B인 경우 변화 없으므로 if문 필요 X
    }
    std::cout << currentScore << std::endl;
    return 0;
}
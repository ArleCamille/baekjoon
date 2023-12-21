#include <iostream>
#include <vector>
#include <cstring>


//#define DEBUG

struct Coordinates {
    int x;
    int y;
    Coordinates() {
        this->x = 0;
        this->y = 0;
    }
    Coordinates(int x, int y) {
        this->x = x;
        this->y = y;
    }
    bool operator==(const Coordinates &rhs) const {
        return this->x == rhs.x && this->y == rhs.y;
    }
};

int N; // 격자 크기. 무조건 홀수. N in 3~49
int M; // "마법" (= 주어진 방향의 구슬 일정수 제거) 시전 횟수
std::vector<Coordinates> spiral; // 나선 번호에서 좌표로 변환하기 위함.
int spiralIndex[49][49]; // spiral을 채우기 위해 번호 넣는 곳.
int balls[49][49]; // 구슬 번호들.
int coalescedBalls[49][49]; // 압축을 위해 임시로 쓰는 구슬 저장고.
int explosionScore;

int center; // N >> 1로 채워질 것. 가운데 자리를 나타냄.

inline bool isValidCoordinates(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

// 중간에 나오는 0들을 제거한다.
void compressBalls() {
    memset(coalescedBalls, 0, sizeof (coalescedBalls));
    int j = 1;
    int iLimit = N * N;
    for (int i = 1; i < iLimit; ++i) {
        if (balls[spiral[i].y][spiral[i].x] <= 0)
            continue;
        coalescedBalls[spiral[j].y][spiral[j].x] = balls[spiral[i].y][spiral[i].x];
        ++j;
    }
    memcpy(balls, coalescedBalls, sizeof (balls));
}

// 4개 이상의 연속된 구슬 무리가 있으면 폭발시켜 0으로 만들려고 시도한다.
bool explodeBalls() {
    // 작업을 하기 전 먼저 중간에 나오는 0들을 제거한다.
    compressBalls();
    int groupBegin = 1;
    int groupBall = balls[spiral[groupBegin].y][spiral[groupBegin].x];
    if (groupBall <= 0)
        return false; // compress 했음에도 첫 공이 0이면 이미 다 터진 것.
    int previousGroupCount;
    bool couldExplode = false;
    int iLimit = N * N;
    for (int i = 2; i < iLimit; ++i) {
        if (balls[spiral[i].y][spiral[i].x] == groupBall)
            continue; // 같은 group이면 계속 진행.
        // 여기서부터는 같은 group이 아닐 때
        previousGroupCount = i - groupBegin;
        if (previousGroupCount >= 4) {
            for (int j = groupBegin; j < i; ++j) {
                explosionScore += groupBall;
                balls[spiral[j].y][spiral[j].x] = 0; // 공들을 터뜨린다.
            }
            couldExplode = true;
        }
        if (balls[spiral[i].y][spiral[i].x] <= 0) {
            return couldExplode; // compress를 했으니 0 이후는 더 기대 불가
        }
        groupBegin = i;
        groupBall = balls[spiral[i].y][spiral[i].x];
    }
    return couldExplode;
    /*
    auto groupBegin = std::next(spiral.cbegin());
    int groupBall = balls[groupBegin->y][groupBegin->x];
    if (groupBall <= 0)
        return false; // compress 했음에도 첫 공이 0이면 이미 다 터진 것.
    int previousGroupCount;
    bool couldExplode = false;
    for (auto iter = std::next(groupBegin); iter != spiral.cend(); iter++) {
        if (balls[iter->y][iter->x] == groupBall)
            continue; // 같은 group이면 계속 진행.
        // 여기서부터는 같은 group이 아니라는 것.
        previousGroupCount = iter - groupBegin;
        if (previousGroupCount >= 4) {
            for (auto _iter = groupBegin; _iter != iter; _iter++) {
                explosionScore += balls[_iter->y][_iter->x];
                balls[_iter->y][_iter->x] = 0; // 공들을 터뜨린다.
            }
            couldExplode = true;
        }
        if (balls[iter->y][iter->x] <= 0)
            return couldExplode; // compress를 했으니 0 이후는 더 기대 불가
        groupBegin = iter; // 공이 있다면 새 그룹 시작
        groupBall = balls[iter->y][iter->x];
    }
    return couldExplode;
    */
}

// 연속된 구슬들을 룰에 따라 (개수-번호)의 그룹으로 바꾼다.
void transformBalls() {
#ifdef DEBUG
    std::cout << "DEBUG: begin transformBalls()..." << std::endl;
#endif
    // 작업을 하기 전 먼저 압축한다.
    compressBalls();
    auto groupBegin = std::next(spiral.cbegin());
    int groupBall = balls[groupBegin->y][groupBegin->x];
    int previousGroupCount;
    if (groupBall <= 0)
        return; // compress 했음에도 첫 공이 0이면 더 이상 할 게 없다.
    // coalescedBalls를 다시 쓸 것이다.
    memset(coalescedBalls, 0, sizeof (coalescedBalls));
    auto rewriteIter = std::next(spiral.cbegin());
    for (auto iter = std::next(groupBegin); iter != spiral.cend(); iter++) {
        if (balls[iter->y][iter->x] == groupBall)
            continue; // 같은 group이면 계속 진행.
        // 여기서부터는 같은 group이 아닐 때
        previousGroupCount = iter - groupBegin;
        // 개수를 먼저 기록
        coalescedBalls[rewriteIter->y][rewriteIter->x] = previousGroupCount;
        rewriteIter++;
        if (rewriteIter == spiral.cend())
            break; // 공이 터질 경우 그대로 stop
        // 그 다음 번호
        coalescedBalls[rewriteIter->y][rewriteIter->x] = groupBall;
        rewriteIter++;
        if (rewriteIter == spiral.cend())
            break; // 역시 공이 터질 경우 그대로 stop
        if (balls[iter->y][iter->x] <= 0)
            break; // 압축을 했는데도 0이 있으면 다 끝난 것.
        groupBegin = iter; // 공이 있다면 새 그룹 시작
        groupBall = balls[iter->y][iter->x];
    }
    memcpy(balls, coalescedBalls, sizeof (balls)); // 지금까지의 작업 결과 저장
}

int main(void) {
    // 일단 N, M을 받는다.
    std::cin >> N >> M;
    int i, j;
    // 언제나 그랬듯 0번 행이 위쪽, 0번 열이 왼쪽.
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            std::cin >> balls[i][j];
    center = N >> 1; // 3이면 1, 5면 2, 7이면 3, ... 해서 중앙을 가리킨다.
    // 더 알고리즘을 진행하기 전에, spiralIndex를 채우고 그걸 바탕으로 spiral을 만든다.
    int spiralDirection = EAST;
    int spiralX = 0;
    int spiralY = 0;
    for (i = N * N - 1; i > 0; --i) {
        spiralIndex[spiralY][spiralX] = i;
        switch (spiralDirection) {
        case EAST:
            if (!isValidCoordinates(spiralX + 1, spiralY) || spiralIndex[spiralY][spiralX + 1] > 0)
                spiralDirection = SOUTH;
            break;
        case SOUTH:
            if (!isValidCoordinates(spiralX, spiralY + 1) || spiralIndex[spiralY + 1][spiralX] > 0)
                spiralDirection = WEST;
            break;
        case WEST:
            if (!isValidCoordinates(spiralX - 1, spiralY) || spiralIndex[spiralY][spiralX - 1] > 0)
                spiralDirection = NORTH;
            break;
        case NORTH:
            if (!isValidCoordinates(spiralX, spiralY - 1) || spiralIndex[spiralY - 1][spiralX] > 0)
                spiralDirection = EAST;
            break;
        }
        switch (spiralDirection) {
        case EAST:
            ++spiralX;
            break;
        case SOUTH:
            ++spiralY;
            break;
        case WEST:
            --spiralX;
            break;
        case NORTH:
            --spiralY;
            break;
        }
    }

#ifdef DEBUG
    std::cout << "DEBUG: spiral indices: " << std::endl;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j)
            std::cout << spiralIndex[i][j] << '\t';
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    // 좌표를 순회하면서 spiral을 채운다.
    spiral.resize(N * N, Coordinates());
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            spiral[spiralIndex[i][j]] = Coordinates(j, i);

#ifdef DEBUG
    std::cout << "DEBUG: spiral vector: ";
    for (auto debugIter = spiral.cbegin(); debugIter != spiral.cend(); debugIter++)
        std::cout << "(" << debugIter->x << ", " << debugIter->y << "), ";
    std::cout << std::endl;
#endif
    
    int removeDirection, removeCount;
    for (int I = 0; I < M; ++I) {
        // d와 s를 받아온다.
        std::cin >> removeDirection >> removeCount;
        // 해당 행동에 따라 구슬들을 제거한다.
        for (i = 1; i <= removeCount; ++i) {
            switch (removeDirection) {
            case EAST:
                balls[center][center + i] = 0;
                break;
            case SOUTH:
                balls[center + i][center] = 0;
                break;
            case WEST:
                balls[center][center - i] = 0;
                break;
            case NORTH:
                balls[center - i][center] = 0;
                break;
            }
        }
#ifdef DEBUG
        std::cout << "DEBUG: initial removal success" << std::endl;
        std::cout << "DEBUG: structure after removal: " << std::endl;
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j)
                std::cout << balls[i][j] << '\t';
            std::cout << std::endl;
        }
#endif
        // 계속 구슬을 폭발시킨다.
        while (explodeBalls()) {
#ifdef DEBUG
            std::cout << "DEBUG: structure after explosion: " << std::endl;
            for (i = 0; i < N; ++i) {
                for (j = 0; j < N; ++j)
                    std::cout << balls[i][j] << '\t';
                std::cout << std::endl;
            }
#endif
        }
        // 더 이상 폭발하지 않는다면 구슬들을 변화시킨다.
        transformBalls();
#ifdef DEBUG
        std::cout << "DEBUG: structure after transform: " << std::endl;
        for (i = 0; i < N; ++i) {
            for (j = 0; j < N; ++j)
                std::cout << balls[i][j] << '\t';
            std::cout << std::endl;
        }
        std::cout << std::endl;
#endif
    }
    // 폭발한 공들의 갯수를 출력하고 끝낸다.
    std::cout << explosionScore << std::endl;
    return 0;
}
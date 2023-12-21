#include <iostream>
#include <cstring>

// y, x, 방향 순. 즉, 해당 좌표 해당 방향에 물고기가 얼마나 있는가? 를 봄.
int map[4][4][8], clonedMap[4][4][8], tempMap[4][4][8];
// 물고기 냄새가 완전히 빠지려면 몇 턴 남았나?
char scent[4][4];

int M; // 초기 물고기 마리수
int S; // iteration 수

int sharkX, sharkY; // 상어의 좌표

void tryMigratingFishFrom(int x, int y, int dir) {
    // 물고기는 이동할 수 있는 방향이 나올 때까지 "반시계로" 이동해본다.
    // 주어진 방향은 시계이므로, 이를 다음과 같이 가공.
    // 0: 좌, 1: 좌하, ..., 7: 좌상
    if (map[y][x][dir] <= 0)
        return; // 당연히, 옮길 수 있는 물고기가 없다면 시도도 안 한다.
    int newDir = dir;
    int newX, newY;
    for (int i = 0; i < 8; ++i) {
        switch (newDir) {
        case 0: // 좌
            newX = x - 1;
            newY = y;
            break;
        case 1: // 좌하
            newX = x - 1;
            newY = y + 1;
            break;
        case 2: // 하
            newX = x;
            newY = y + 1;
            break;
        case 3: // 우하
            newX = x + 1;
            newY = y + 1;
            break;
        case 4: // 우
            newX = x + 1;
            newY = y;
            break;
        case 5: // 우상
            newX = x + 1;
            newY = y - 1;
            break;
        case 6: // 상
            newX = x;
            newY = y - 1;
            break;
        case 7: // 좌상
            newX = x - 1;
            newY = y - 1;
            break;
        }
        if (newX < 0 || newX >= 4 || newY < 0 || newY >= 4) {
            newDir = (++newDir) % 8;
            continue; // 맵 밖으로 나갈 경우 다음 방향을 시도
        } else if (newX == sharkX && newY == sharkY) {
            newDir = (++newDir) % 8;
            continue; // 상어랑 겹칠 경우도 다음 방향을 시도
        } else if (scent[newY][newX] > 0) {
            newDir = (++newDir) % 8;
            continue; // 안 빠진 피냄새가 있을 경우에도 다음 방향을 시도
        }
#ifdef DEBUG
        std::cout << "DEBUG: moving " << map[y][x][dir] << " fish from (" << x << ", " << y << ") to (" << newX << ", " << newY << ")" << std::endl;
        std::cout << "DEBUG: (originally heading ";
        switch (dir) {
        case 0:
            std::cout << "W)" << std::endl;
            break;
        case 1:
            std::cout << "SW)" << std::endl;
            break;
        case 2:
            std::cout << "S)" << std::endl;
            break;
        case 3:
            std::cout << "SE)" << std::endl;
            break;
        case 4:
            std::cout << "E)" << std::endl;
            break;
        case 5:
            std::cout << "NE)" << std::endl;
            break;
        case 6:
            std::cout << "N)" << std::endl;
            break;
        case 7:
            std::cout << "NW)" << std::endl;
            break;
        }
#endif
        tempMap[newY][newX][newDir] += map[y][x][dir];
        return;
    }
    // 아무 곳에도 이동할 수 없었을 경우, 현 좌표에 고스란히 놔둔다.
    tempMap[y][x][dir] += map[y][x][dir];
}

// 상어의 이동은 0=상, 1=좌, 2=하, 3=우로 표현.
int optimalSharkMoveSequence[3]; // 지금까지 최대한 많은 고기를 잡아먹은 상어의 이동 순서
int currentSharkMoveSequence[3]; // 현재 시도하고 있는 이동 순서
int savedFishCount[3][8]; // 상어가 쓸고 간 자리에 있었던 각 방향별 물고기 마릿수
int savedFishMaximum; // 먹은 물고기 마릿수 맥시멈

void getOptimalSharkMoveSequence(int x, int y, int seq, int sum) {
    if (seq >= 3) {
        if (optimalSharkMoveSequence[0] < 0 || sum > savedFishMaximum) {
            // 현재의 sequence를 optimal로 저장해야 함.
            for (int i = 0; i < 3; ++i)
                optimalSharkMoveSequence[i] = currentSharkMoveSequence[i];
            savedFishMaximum = sum;
        }
        return;
    } else if (seq <= 0) {
        // 첫 시작에서는 savedFishMaximum과 optimalSharkMoveSequence를 초기화.
        savedFishMaximum = 0;
        // 세팅이 안 된 상태와 하나도 못 잡아먹은 상태를 구별하기 위해 -1로 세팅.
        for (int i = 0; i < 3; ++i)
            optimalSharkMoveSequence[i] = -1;
    }
    
    int i, j;
    int newX, newY;
    for (i = 0; i < 4; ++i) {
        switch (i) {
        case 0: // 상
            newY = y - 1;
            newX = x;
            break;
        case 1: // 좌
            newY = y;
            newX = x - 1;
            break;
        case 2: // 하
            newY = y + 1;
            newX = x;
            break;
        case 3: // 우
            newY = y;
            newX = x + 1;
            break;
        }
        if (newX < 0 || newX >= 4 || newY < 0 || newY >= 4)
            continue; // 밖으로 나갈 경우 불인정.
        // 이동할 경로에 있는 물고기 마릿수를 저장하고, 그 자리 물고기 없앰.
        int newSum = sum;
        for (j = 0; j < 8; ++j) {
            savedFishCount[seq][j] = map[newY][newX][j];
            newSum += map[newY][newX][j];
            map[newY][newX][j] = 0;
        }

        // 이동 순서를 저장하고 다음으로 이행.
        currentSharkMoveSequence[seq] = i;
        getOptimalSharkMoveSequence(newX, newY, seq + 1, newSum);

        // 원상복구.
        for (j = 0; j < 8; ++j)
            map[newY][newX][j] = savedFishCount[seq][j];
    }
}

int main(void) {
    std::cin >> M >> S;
    // 물고기 x는 위부터 1, 2, 3, 4이며 y는 왼쪽부터 1, 2, 3, 4.
    // 이를 위부터 0, 1, 2, 3과 왼쪽부터 0, 1, 2, 3으로 각각 바꿔줘야 함.
    // 또한, dir는 좌측부터 시계방향 45도가 각각 1, 2, 3, 4, 5, 6, 7, 8...로 되어 있는 것을
    // 좌측부터 시계방향 0, 7, 6, 5, 4, 3, 2, 1이 되어야 함.
    int i, x, y, dir;
    int I, j, k;
    for (i = 0; i < M; ++i) {
        std::cin >> x >> y >> dir;
#ifdef DEBUG
        std::cout << "DEBUG: fish coordinates (" << y - 1 << ", " << x - 1 << ", ";
        switch ((9 - dir) % 8) {
        case 0:
            std::cout << "W";
            break;
        case 1:
            std::cout << "SW";
            break;
        case 2:
            std::cout << "S";
            break;
        case 3:
            std::cout << "SE";
            break;
        case 4:
            std::cout << "E";
            break;
        case 5:
            std::cout << "NE";
            break;
        case 6:
            std::cout << "N";
            break;
        case 7:
            std::cout << "NW";
            break;
        }
        std::cout << ")" << std::endl;
#endif
        ++map[x - 1][y - 1][(9 - dir) % 8];
    }
    // 마지막으로 상어의 x, y도 같은 방식으로 지정.
    std::cin >> x >> y;
    sharkX = y - 1;
    sharkY = x - 1;

#ifdef DEBUG
        int tmp1;
        std::cout<<"DEBUG: Grid layout"<<std::endl;
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                tmp1 = 0;
                for (k = 0; k < 8; ++k)
                    tmp1 += map[i][j][k];
                std::cout << tmp1 << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << "DEBUG: Shark at (" << sharkX << ", " << sharkY << ")" << std::endl;
        std::cout << std::endl;
#endif

    for (I = 0; I < S; ++I) {
        // 각 iteration에 대해서...
        // 1. "상어가 모든 물고기에게 복제 마법을 시전한다."
        // 복제 마법의 결과물을 memcpy로.
        std::memcpy(clonedMap, map, sizeof (map));

        // 2. "모든 물고기가 한 칸 이동한다."
        // 일단 이동 후 물고기 위치를 가리키는 tempMap을 클린해준다.
        std::memset(tempMap, 0, sizeof (tempMap));
        // 모든 좌표 모든 방향에 대해 이동 시도.
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                for (k = 0; k < 8; ++k)
                    tryMigratingFishFrom(j, i, k);
        // 이동 결과를 다시 map으로 땡겨온다.
        std::memcpy(map, tempMap, sizeof (map));

#ifdef DEBUG
        int tmp1;
        std::cout<<"DEBUG: Grid layout after move"<<std::endl;
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                tmp1 = 0;
                for (k = 0; k < 8; ++k)
                    tmp1 += map[i][j][k];
                std::cout << tmp1 << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
#endif

        // 3. "상어가 연속해서 3칸 이동한다."
        // 일단 3번의 이동을 simulate해 본다.
        getOptimalSharkMoveSequence(sharkX, sharkY, 0, 0);

        // 이후, optimalSharkMoveSequence에 의거해 움직인다.
        // 만일 움직인 자리에 물고기가 있었다면, 전멸시키고 피냄새를 매긴다.
        for (i = 0; i < 3; ++i) {
            switch (optimalSharkMoveSequence[i]) {
            case 0: // 상
                --sharkY;
                break;
            case 1: // 좌
                --sharkX;
                break;
            case 2: // 하
                ++sharkY;
                break;
            case 3: // 우
                ++sharkX;
                break;
            }
            for (j = 0; j < 8; ++j) {
                if (map[sharkY][sharkX][j] > 0) {
                    // 피냄새를 매기고, 물고기를 없앤다.
                    map[sharkY][sharkX][j] = 0;
                    // 3으로 해야 4번 과정에서 2로 줄음
                    scent[sharkY][sharkX] = 3;
                }
            }
        }

#ifdef DEBUG
        std::cout<<"DEBUG: Grid layout after consumption"<<std::endl;
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                tmp1 = 0;
                for (k = 0; k < 8; ++k)
                    tmp1 += map[i][j][k];
                std::cout << tmp1 << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << "DEBUG: Shark at (" << sharkX << ", " << sharkY << ")" << std::endl;
        std::cout << std::endl;
#endif

        // 4. "두 번 전 연습에서 생긴 물고기의 냄새가 격자에서 사라진다."
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                if (scent[i][j] > 0)
                    --scent[i][j];
        
        // 5. "복제 마법이 완료된다."
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                for (k = 0; k < 8; ++k)
                    map[i][j][k] += clonedMap[i][j][k];

#ifdef DEBUG
        std::cout<<"DEBUG: Grid layout after clone"<<std::endl;
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 4; ++j) {
                tmp1 = 0;
                for (k = 0; k < 8; ++k)
                    tmp1 += map[i][j][k];
                std::cout << tmp1 << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << "DEBUG: Shark at (" << sharkX << ", " << sharkY << ")" << std::endl;
        std::cout << std::endl;
#endif
    }

    // 모든 연습이 종료된 후, 모든 격자의 모든 물고기 수를 구한다.
    int sum = 0;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            for (k = 0; k < 8; ++k)
                sum += map[i][j][k];
    
    std::cout<<sum<<std::endl;
    return 0;
}
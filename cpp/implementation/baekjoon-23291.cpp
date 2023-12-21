#include <iostream>
#include <vector>
#include <cstring>

int N, K;

// N<=100이므로 vector가 아닌 고정크기 배열로 표현해도 무방
// container[y][x] 꼴로 표현.
// 어항이 없는 빈 칸은 0으로 표현.
int container[100][100];
int containerNew[100][100];

int main(void) {
    std::cin >> N >> K;
    std::memset(container, 0, sizeof (container));
    std::memset(containerNew, 0, sizeof (containerNew));
    int i, j;
    for (i = 0; i < N; ++i)
        std::cin >> container[0][i];

    int stackedColumns, stackedHeight, leftStart;
    int tmp1, tmp2, tmp3;
    int minFish;
    int iterations = 0;
    std::vector<int> minIndices;

    while (1) {
        // 만일 물고기가 제일 많이 든 어항과 제일 적게 든 어항의 차이가 K 이하면 종료.
        minFish = container[0][0];
        tmp1 = container[0][0]; // maxFish 역할
        for (i = 1; i < N; ++i) {
            if (container[0][i] < minFish)
                minFish = container[0][i];
            else if (container[0][i] > tmp1)
                tmp1 = container[0][i];
        }
        if (tmp1 - minFish <= K)
            break;
        // 그렇지 않다면 일단 정리가 필요.
        ++iterations;
        // 0단계: 물고기 수가 제일 작은 어항에 물고기를 한 마리 넣는다.
        minIndices = std::vector<int>();
        minIndices.push_back(0);
        minFish = container[0][0];
        for (i = 1; i < N; ++i) {
            if (container[0][i] < minFish) {
                minFish = container[0][i];
                minIndices.clear();
                minIndices.push_back(i);
            } else if (container[0][i] == minFish) {
                minIndices.push_back(i);
            }
        }

        for (auto iter = minIndices.cbegin(); iter != minIndices.cend(); iter++)
            ++container[0][*iter];

        // 1단계: 돌출부를 90도 회전시키는 것을 반복한다.
        // 초기에 맨 왼쪽을 쌓는 것은 고정 operation이므로 먼저 해 둔다.
        container[1][1] = container[0][0];
        container[0][0] = 0;
        // 초기 상태에서 쌓인 열 수는 1, 열 높이는 2, 맨 왼쪽은 1.
        stackedColumns = 1;
        stackedHeight = 2;
        leftStart = 1;
        // 아래에 계속 쌓을 수 있는 동안 계속 돌린다.
        while (N - (leftStart + stackedColumns) >= stackedHeight) {
            for (i = 0; i < stackedHeight; ++i) {
                for (j = 0; j < stackedColumns; ++j) {
                    // 90도 돌리기.
                    container[stackedColumns - j][leftStart + stackedColumns + i] = container[i][leftStart + j];
                    container[i][leftStart + j] = 0;
                }
            }
            leftStart += stackedColumns; // 다음에는 지금 적층된 열들이 없어진다.
            tmp1 = 1 + stackedColumns; // new stackedHeight
            tmp2 = stackedHeight; // new stackedColumns
            stackedHeight = tmp1;
            stackedColumns = tmp2;
        }
        
        // 다음으로, 모든 인접한 셀에 대해서 물고기 수를 구한다.
        // 그러기 전, 현재의 물고기 수를 containerNew로 통째로 복사한다.
        for (i = 0; i < N; ++i)
            for (j = 0; j < N; ++j)
                containerNew[i][j] = 0; // 복사 전 초기화
        
        for (i = 0; i < stackedHeight; ++i)
            for (j = 0; j < stackedColumns; ++j)
                containerNew[i][leftStart + j] = container[i][leftStart + j];
        for (i = leftStart + stackedColumns; i < N; ++i)
            containerNew[0][i] = container[0][i];

        // container를 기준, 모든 인접한 cell마다 물고기 교류 처리를 한다.
        // 상-하, 적층된 열들에서만 발생함.
        for (i = 0; i < stackedHeight - 1; ++i) {
            for (j = 0; j < stackedColumns; ++j) {
                if (container[i][leftStart + j] < container[i + 1][leftStart + j]) {
                    tmp1 = (container[i + 1][leftStart + j] - container[i][leftStart + j]) / 5; // d값
                    containerNew[i][leftStart + j] += tmp1; // 적은 쪽으로는 들어오고
                    containerNew[i + 1][leftStart + j] -= tmp1; // 많은 쪽에서는 나가고
                } else {
                    // 아래가 더 많음.
                    tmp1 = (container[i][leftStart + j] - container[i + 1][leftStart + j]) / 5;
                    containerNew[i + 1][leftStart + j] += tmp1;
                    containerNew[i][leftStart + j] -= tmp1;
                }
            }
        }
        // 좌-우, 적층된 열들과 1행에서 모두 발생 가능.
        for (i = 1; i < stackedHeight; ++i) {
            for (j = 0; j < stackedColumns - 1; ++j) {
                if (container[i][leftStart + j] < container[i][leftStart + j + 1]) {
                    // 오른쪽이 더 많음.
                    tmp1 = (container[i][leftStart + j + 1] - container[i][leftStart + j]) / 5;
                    containerNew[i][leftStart + j] += tmp1;
                    containerNew[i][leftStart + j + 1] -= tmp1;
                } else {
                    // 왼쪽이 더 많음.
                    tmp1 = (container[i][leftStart + j] - container[i][leftStart + j + 1]) / 5;
                    containerNew[i][leftStart + j + 1] += tmp1;
                    containerNew[i][leftStart + j] -= tmp1;
                }
            }
        }
        // 1행 처리
        for (j = leftStart; j < N - 1; ++j) {
            if (container[0][j] < container[0][j + 1]) {
                // 오른쪽이 더 많음.
                tmp1 = (container[0][j + 1] - container[0][j]) / 5;
                containerNew[0][j] += tmp1;
                containerNew[0][j + 1] -= tmp1;
            } else {
                // 왼쪽이 더 많음.
                tmp1 = (container[0][j] - container[0][j + 1]) / 5;
                containerNew[0][j + 1] += tmp1;
                containerNew[0][j] -= tmp1;
            }
        }

        // container를 비우고, 여기에 다시 적층된 열들을 돌려놓는 작업을 한다.
        for (i = 0; i < N; ++i)
            for (j = 0; j < N; ++j)
                container[i][j] = 0;
        
        // tmp1을 container에서의 index로 사용.
        tmp1 = 0;
        for (j = 0; j < stackedColumns; ++j) {
            for (i = 0; i < stackedHeight; ++i) {
                // 쌓인 열은 오른쪽 방향으로 펼쳐진다.
                container[0][tmp1++] = containerNew[i][leftStart + j];
            }
        }
        for (i = leftStart + stackedColumns; i < N; ++i)
            // 나머지 복사
            container[0][tmp1++] = containerNew[0][i];
        
        // 2단계: 반씩 접는다.
        // N은 4의 배수이므로 N/2는 N >> 1로 간략히 표현 가능.
        tmp1 = N >> 1;
        for (i = 0; i < tmp1; ++i) {
            container[1][N - i - 1] = container[0][i];
            container[0][i] = 0;
        }
        // tmp1 ~ N - 1까지 1층이 쌓여 있음.
        // 이걸 다시 접는다.
        tmp2 = tmp1 >> 1;
        for (i = 0; i < tmp2; ++i) {
            container[2][N - i - 1] = container[1][tmp1 + i];
            container[3][N - i - 1] = container[0][tmp1 + i];
            container[1][tmp1 + i] = 0;
            container[0][tmp1 + i] = 0;
        }

        // 모든 인접한 셀에 대해서 물고기 이동을 또 한다.
        // 그 전, containerNew에 또 container를 복사.
        for (i = 0; i < N; ++i)
            for (j = 0; j < N; ++j)
                containerNew[i][j] = container[i][j];
        
        // 이 경우는 height는 4층으로 고정되어 있음.
        // 좌우
        for (i = 0; i < 4; ++i) {
            for (j = N - (N >> 2); j < N - 1; ++j) {
                if (container[i][j] < container[i][j + 1]) {
                    // 오른쪽이 더 많음.
                    tmp1 = (container[i][j + 1] - container[i][j]) / 5;
                    containerNew[i][j] += tmp1;
                    containerNew[i][j + 1] -= tmp1;
                } else {
                    // 왼쪽이 더 많음.
                    tmp1 = (container[i][j] - container[i][j + 1]) / 5;
                    containerNew[i][j + 1] += tmp1;
                    containerNew[i][j] -= tmp1;
                }
            }
        }

        // 상하
        for (i = 0; i < 3; ++i) {
            for (j = N - (N >> 2); j < N; ++j) {
                if (container[i][j] < container[i + 1][j]) {
                    // 위쪽이 더 많음.
                    tmp1 = (container[i + 1][j] - container[i][j]) / 5;
                    containerNew[i][j] += tmp1;
                    containerNew[i + 1][j] -= tmp1;
                } else {
                    // 아래쪽이 더 많음.
                    tmp1 = (container[i][j] - container[i + 1][j]) / 5;
                    containerNew[i + 1][j] += tmp1;
                    containerNew[i][j] -= tmp1;
                }
            }
        }

        // 쌓인 것을 다시 container 쪽으로 돌린다.
        // 그 전에 먼저 1층 제외 초기화.
        for (i = 1; i < N; ++i)
            for (j = 0; j < N; ++j)
                container[i][j] = 0;
        
        // height가 4로 고정이므로, 동적 레이아웃인 1단계에 비해 쉽게 펼치기 가능.
        tmp1 = N - (N >> 2);
        for (i = 0; i < N; ++i)
            container[0][i] = containerNew[i & 0x3][tmp1 + (i >> 2)];
#ifdef DEBUG
        // DEBUG: 현재 layout 출력
        std::cout << "DEBUG: current layout" << std::endl;
        for (i = N - 1; i >= 0; --i) {
            for (j = 0; j < N; ++j) {
                std::cout << container[i][j] << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << "Stacked columns: " << stackedColumns << std::endl;
        std::cout << "Stacked height:  " << stackedHeight << std::endl;
        std::cout << "Start of stack:  " << leftStart << std::endl;
#endif
    }

    std::cout << iterations << std::endl;

    return 0;
}
#include <stdio.h>

// nXm, mXl, lXo, ... 식으로 계속되면 최종 행렬 크기는 nXo
// 둘로 쪼개면, 각각 두 sub행렬을 구하는 데 사용했던 곱셈 수를 더한 뒤 (첫 행렬 행수) x (첫 클러스터 마지막 열수이자 둘째 클러스터 첫번째 행수) x (둘째 클러스터 마지막 열수)를 더한다.
// DP 단위는 memory[i][j]: i번째 행렬부터 j번째 행렬까지 곱한 것. i == j - 1이라면 두 인접 행렬을 곱한 값이 나오고, i == j라면 행렬 곱 자체가 일어나지 않으니 0이다.
// 궁극적으로 구해야 하는 것은 즉, memory[0][N-1]이다. 1번째 행렬부터 N번째 행렬까지 곱해야 하니.

int memory[500][500]; // 500 * 500 * 4 = 약 1MB 미만. 256MB보다 한참 못 미치니 이 정도는 써도 됨.
int rows[500], columns[500]; // 각 matrix별 행의 수, 열의 수.
int N; // 행렬 갯수

int calculate(int start, int end) {
    if (start >= end)
        return 0;
    else if (start == end - 1)
        return rows[start] * columns[start] * columns[end]; // mXn과 nXk를 곱하면 mXnXk이다.
    else if (memory[start][end] > 0) // 한 번 구한 적이 있는 값
        return memory[start][end];
    
    // 초기값은 A(BCDE...)의 형태로 한다.
    int min_result = calculate(start + 1, end) + (rows[start] * columns[start] * columns[end]); // calculate(start, start) == 0이므로 생략해도 무방.
    int result;
    for (int split = start + 1; split < end; ++split) {
        result = calculate(start, split) + calculate(split + 1, end) + (rows[start] * columns[split] * columns[end]);
        if (result < min_result)
            min_result = result;
    }

    // 메모리에 저장
    memory[start][end] = min_result;
    return min_result;
}

int main(void) {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i)
        scanf("%d %d", &rows[i], &columns[i]); // 행렬 사이즈 받아오기
    
    printf("%d\n", calculate(0, N - 1));
}
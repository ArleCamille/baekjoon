#include <stdio.h>

int permutation[8], A[8], N;
unsigned char visited[8];

inline int abs(int x) {
    return x < 0 ? -x : x;
}

// N개의 숫자의 permutation을 구한 뒤, 이를 A에 대한 수열로 변환, 합을 구한다.
int permute(int depth) {
    if (depth == N) {
        // N개의 숫자를 다 뽑았다면 차이의 합을 구해 본다.
        int sum = 0;
        for (int i = 0; i < N - 1; ++i) {
            sum += abs(A[permutation[i]] - A[permutation[i + 1]]);
        }
        return sum;
    }
    
    int i, result;
    int max = 0;
    for (i = 0; i < N; ++i) {
        if (!visited[i]) {
            // 사용한 적이 없는 숫자를 추가로 사용.
            permutation[depth] = i;
            visited[i] = 1;

            // 이 상태에서의 최댓값을 구해 본다.
            result = permute(depth + 1);
            // 최댓값이 이전 값보다 더 크다면 최댓값을 갱신
            if (result > max)
                max = result;

            // 이 숫자를 사용한 적 없다고 놓아줌
            visited[i] = 0;
        }
    }
    return max;
}

int main(void) {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &A[i]);
    }

    printf("%d\n", permute(0));
}
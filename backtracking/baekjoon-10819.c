#include <stdio.h>

int permutation[8], A[8], N;
unsigned char visited[8];

inline int abs(int x) {
    return x < 0 ? -x : x;
}

int permute(int depth) {
    if (depth == N) {
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
            permutation[depth] = i;
            visited[i] = 1;

            result = permute(depth + 1);
            if (result > max)
                max = result;

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
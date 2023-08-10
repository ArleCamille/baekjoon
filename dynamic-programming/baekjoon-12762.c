#include <stdio.h>

// 감소 부분
int length[1000]; // 최장 길이 증가 수열의 길이

// 증가 부분
int length_reversed[1000]; // 최장 길이 감소 수열의 길이

// 입력값
int N, A[1000];

int main(void) {
    scanf("%d", &N);

    int i, j, max_length;
    for (i = 0; i < N; ++i)
        scanf("%d", &A[i]);
    
    // 감소 수열 구하기
    length[0] = 1;
    for (i = 1; i < N; ++i) {
        max_length = 1; // 최악의 경우; 이게 제일 큰 숫자인 경우
        for (j = 0; j < i; ++j)
            if (A[i] < A[j] && length[j] + 1 > max_length)
                max_length = length[j] + 1;
        length[i] = max_length;
    }

    // 증가 수열 구하기
    length_reversed[N - 1] = 1;
    for (i = N - 2; i >= 0; --i) {
        max_length = 1; // 최악의 경우; 이게 제일 큰 숫자인 경우
        for (j = i + 1; j < N; ++j)
            if (A[i] < A[j] && length_reversed[j] + 1 > max_length)
                max_length = length_reversed[j] + 1;
        length_reversed[i] = max_length;
    }

    // 문제에서 원하는 부분 수열의 길이는 각 원소에 대해서 그 원소를 피벗으로 하는 증가 수열과 감소 수열의 길이를 합친 것이다.
    // 거기서 1을 빼야 한다. (피벗은 두 번 세지기 때문)
    int max_result = length[0] + length_reversed[0];
    for (i = 1; i < N; ++i)
        if (length[i] + length_reversed[i] > max_result)
            max_result = length[i] + length_reversed[i];
    
    printf("%d\n", max_result - 1);

    return 0;
}
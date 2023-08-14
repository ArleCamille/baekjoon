#include <stdio.h>

int N, A[50];

int main(void) {
    scanf("%d", &N);
    int i, j;
    for (i = 0; i < N; ++i)
        scanf("%d", &A[i]);
    
    // 정렬
    int tmp;
    for (i = 0; i < N; ++i) {
        for (j = i + 1; j < N; ++j) {
            if (A[i] > A[j]) {
                tmp = A[j];
                A[j] = A[i];
                A[i] = tmp;
            }
        }
    }
    // 양수, 음수가 어디부터 어디까지인지 세기
    int neg_max = -1; // 0에서부터 여기까지 음수
    int pos_min = N; // N-1에서부터 여기까지 양수
    for (i = 0; i < N; ++i) {
        if (A[i] < 0)
            neg_max = i;
    }
    for (i = N - 1; i >= 0; --i) {
        if (A[i] > 0)
            pos_min = i;
    }
    int sum = 0;
    // 음수 묶어서 더함
    for (i = 1; i <= neg_max; i += 2) {
        sum += A[i - 1] * A[i];
    }
    // 숫자 한 개 남은 경우 조건식
    // 만약에 0이 있을 경우, 0과 묶어서 버릴 수 있음.
    // 그렇지 않을 경우 (neg_max == pos_min - 1; 음수 뒤에 바로 양수) 음수 짤없이 더해야함
    if (i - neg_max == 1 && neg_max == pos_min - 1) {
        sum += A[neg_max];
    }
    // 양수 묶어서 더함
    int part_sum, part_product;
    for (i = N - 2; i >= pos_min; i -= 2) {
        // 양수는 곱이 더 작을 수도 있다
        part_product = A[i] * A[i + 1];
        part_sum = A[i] + A[i + 1];
        sum += part_sum > part_product ? part_sum : part_product;
    }
    if (pos_min - i == 1) {
        // 숫자 한 개 남음
        sum += A[pos_min];
    }

    printf("%d\n", sum);
    return 0;
}
#include <stdio.h>

int N, S;
unsigned char used[20]; // A[i]가 현재 추적하고 있는 수열에서 사용되었는지 여부
int A[20]; // 입력 정수
int count = 0;

int get_current_sum(int up_to) {
    // A[0]부터 A[up_to]까지의 숫자들을 써서 만든 수열에 대해 합을 구한다.
    int sum = 0;
    for (int i = 0; i <= up_to; ++i) {
        if (used[i]) { // A[i]가 수열에 포함되어 있으면 (used에 마킹되어 있으면)
            sum += A[i]; // 합에 더함
        }
    }
    return sum;
}

// 현재 A[next_index - 1]까지의 숫자가 수열에서 사용되었는지 여부가 결정되었을 때,
// A[next_index]가 사용된 경우와 사용되지 않은 경우로 분기해서 다음으로 넘어간다.
void iterate(int next_index) {
    if (next_index >= N) // 모든 숫자를 다 시도해본 경우
        return;
    // A[next_index]를 사용하는 경우부터 먼저 시도해 보자.
    used[next_index] = 1;
    // 다른 숫자를 썼기 때문에, 새로운 수열일 가능성이 있다.
    int sum = get_current_sum(next_index);
#ifdef DEBUG
    for (int i = 0; i < N; ++i) {
        printf("%d ", used[i]);
    }
    printf("-> %d, ", sum);
#endif
    // 합이 조건식(S)과 일치하는가?
    if (sum == S) {
        ++count; // 수열 하나를 카운트
#ifdef DEBUG
        puts("Match!");
#endif
    }
#ifdef DEBUG
    else {
        puts("No match");
    }
#endif
    // 다음 숫자도 시도해본다.
    iterate(next_index + 1);

    // 이 숫자를 사용하지 않는 경우도 시도해 본다.
    // 합이 변하지 않기 떄문에 새로 count하지는 않는다.
    used[next_index] = 0;
    // 다음 숫자만 시도해본다.
    iterate(next_index + 1);
}

int main() {
    scanf("%d %d", &N, &S);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &A[i]);
    }

    // 첫번째 숫자부터 계속 넣어 가면서 모든 경우를 시도해 본다.
    iterate(0);

    printf("%d\n", count);

    return 0;
}
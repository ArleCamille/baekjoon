#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct EQUATION {
    int a;
    int b;
} equation_t;

// qsort나 mergesort에 들어가는 비교 함수는 lhs < rhs이면 음수, lhs > rhs이면 양수, lhs == rhs이면 0을 출력해야 한다.
int comparator(const void *a, const void *b) {
    equation_t *lhs = (equation_t *)a, *rhs = (equation_t *)b;
    // edge case 1: 계수가 0인 측은 무조건 우측
    if (!lhs->a)
        return 1;
    else if (!rhs->a)
        return -1;
    // edge case 2: 양쪽의 절편이 모두 0이라면 계수가 큰 쪽을 따름
    if (!(lhs->b | rhs->b)) {
        return lhs->a - rhs->a; // 이러면 a<c이면 음수, a>c이면 양수가 나오게 된다.
    }
    // bc - ad를 반환한다.  bc - ad < 0이라면 at+b가 왼쪽이다.
    return (lhs->b * rhs->a) - (lhs->a * rhs->b);
}

int N;
equation_t equations[200000];

int main(void) {
    scanf("%d", &N);
    int i;
    for (i = 0; i < N; ++i)
        scanf("%d %d", &equations[i].a, &equations[i].b);

    int remainder = 0;

    qsort(equations, N, sizeof (equation_t), comparator);

    for (i = 0; i < N; ++i) {
        // at+b + t % 40000을 계속 저장
        remainder = (remainder * (equations[i].a + 1) + equations[i].b) % 40000;
    }

    printf("%d\n", remainder);
}
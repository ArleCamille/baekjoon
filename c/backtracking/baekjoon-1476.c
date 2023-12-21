#include <stdio.h>

int main(void) {
    int E, S, M;
    scanf("%d %d %d", &E, &S, &M);

    int passed = 0;
    // E == S && S == M이면, E년도를 더하면 된다.
    // 그렇지 않을 경우, 주먹구구식으로 다 하나씩 빼 보면서 법칙에 따라서 E가 1 미만으로 떨어지면 15로, S가 1 미만으로 떨어지면 28로, M이 1 미만으로 떨어지면 19로 복구한다.
    while (!(E == S && S == M)) {
        --E; --S; --M; ++passed;
        if (E <= 0)
            E += 15;
        if (S <= 0)
            S += 28;
        if (M <= 0)
            M += 19;
    }

    printf("%d\n", passed + E);

    return 0;
}
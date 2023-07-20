#include <stdio.h>

int main(void) {
    int E, S, M;
    scanf("%d %d %d", &E, &S, &M);

    int passed = 0;
    // E == S && S == M이면, E년도를 더하면 된다.
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
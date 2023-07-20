#include <stdio.h>

int N, S;
unsigned char used[20];
int A[20];
int count = 0;

int get_current_sum(int up_to) {
    int sum = 0;
    for (int i = 0; i <= up_to; ++i) {
        if (used[i]) {
            sum += A[i];
        }
    }
    return sum;
}

void iterate(int next_index) {
    if (next_index >= N)
        return;
    used[next_index] = 1;
    int sum = get_current_sum(next_index);
#ifdef DEBUG
    for (int i = 0; i < N; ++i) {
        printf("%d ", used[i]);
    }
    printf("-> %d\n", sum);
#endif
    if (sum == S) {
        ++count;
#ifdef DEBUG
        puts("Match!");
#endif
    }
    iterate(next_index + 1);

    used[next_index] = 0;
    iterate(next_index + 1);
}

int main() {
    scanf("%d %d", &N, &S);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &A[i]);
    }

    iterate(0);

    printf("%d\n", count);

    return 0;
}
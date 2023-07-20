#include <stdio.h>
#include <limits.h> // INT_MAX 사용 위해

int N;
int S[20][20];
int visit_order[10];
unsigned char visited[20];

int abs_difference() {
    int i, j, start_sum, link_sum;
    start_sum = 0;
    link_sum = 0;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (visited[i] && visited[j])
                start_sum += S[i][j];
            else if (!visited[i] && !visited[j])
                link_sum += S[i][j];
        }
    }
    int diff = start_sum - link_sum;
    return diff < 0 ? -diff : diff;
}

int visit(int depth) {
    int i, j;
    int previous = depth > 0 ? visit_order[depth - 1] : -1;
    int minimum = INT_MAX;
    int result;
    for (i = previous + 1; i < N; ++i) {
        visit_order[depth] = i;
        visited[i] = 1;
        if (depth == N / 2 - 1) {
            result = abs_difference();
#ifdef DEBUG
            printf("Start team: ");
            for (j = 0; j <= depth; ++j) {
                printf("%d ", visit_order[j]);
            }
            puts("");
            printf("Difference: %d\n", result);
#endif
        } else
            result = visit(depth + 1);
        if (result < minimum)
            minimum = result;
        visited[i] = 0;
    }
    return minimum;
}

int main(void) {
    scanf("%d", &N);
    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            scanf("%d", &S[i][j]);
    
    printf("%d\n", visit(0));

    return 0;
}
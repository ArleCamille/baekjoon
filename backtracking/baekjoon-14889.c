#include <stdio.h>
#include <limits.h> // INT_MAX 사용 위해

int N;
int S[20][20];
int visit_order[10];
unsigned char visited[20];

// 스타트 팀과 링크 팀이 완성되었을 때 (N/2명) 전력차를 구해 본다.
int abs_difference() {
    int i, j, start_sum, link_sum;
    start_sum = 0;
    link_sum = 0;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (visited[i] && visited[j]) // i와 j가 모두 스타트에 속했다면
                start_sum += S[i][j]; // 스타트 팀 전력 up
            else if (!visited[i] && !visited[j]) // i와 j가 모두 링크에 속했다면
                link_sum += S[i][j]; // 링크 팀 전력 up
        }
    }
    int diff = start_sum - link_sum;
    return diff < 0 ? -diff : diff; // 전력차 절댓값
}

int visit(int depth) {
    int i, j;
    // previous: 몇 번 선수까지 스타트 팀에 뽑았는가?
    // 이전 단계가 없다면 -1번 선수 (0번 선수부터 뽑게)
    int previous = depth > 0 ? visit_order[depth - 1] : -1;
    int minimum = INT_MAX;
    int result;
    for (i = previous + 1; i < N; ++i) {
        // 현 단계에서 뽑을 수 있는 각 선수에 대해,
        visit_order[depth] = i;
        visited[i] = 1;
        if (depth == N / 2 - 1) {
            // 만약에 N/2명을 구해서 팀을 다 짰다면, 팀 전력차를 구해 본다.
            result = abs_difference();
#ifdef DEBUG
            printf("Start team: ");
            for (j = 0; j <= depth; ++j) {
                printf("%d ", visit_order[j]);
            }
            puts("");
            printf("Difference: %d\n", result);
#endif
        } else // 그렇지 않다면, 인원을 더 채운 것의 최솟값.
            result = visit(depth + 1);
        // 그렇게 해서 격차 최솟값이 갱신되면 최솟값 갱신.
        if (result < minimum)
            minimum = result;
        // 이 선수를 다시 안 쓴 것으로 마킹.
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
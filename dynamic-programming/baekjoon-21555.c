#include <stdio.h>

int N, K;
long long A[200000], B[200000];

long long Alast[200001], Blast[200001];

// 둘 중 작은 것. 컴파일러 최적화 때문에 함수로 만들어도 상관X
long long min(long long x, long long y) {
    return x > y ? y : x;
}

int main(void) {
    scanf("%d %d", &N, &K);
    int i;
    for (i = 0; i < N; ++i)
        scanf("%lld", &A[i]);
    for (i = 0; i < N; ++i)
        scanf("%lld", &B[i]);
    
    Alast[1] = A[0]; // 1번까지 이동하되, 마지막은 끌어서 옮긴 경우 최소 비용은 A1
    Blast[1] = B[0]; // 2번까지 이동하되, 마지막은 들고 옮긴 경우 최소 비용은 A2
    for (i = 2; i <= N; ++i) {
        Alast[i] = A[i - 1] + min(Alast[i - 1], Blast[i - 1] + K);
        Blast[i] = B[i - 1] + min(Blast[i - 1], Alast[i - 1] + K);
    }
    printf("%lld\n", min(Alast[N], Blast[N]));
    return 0;
}
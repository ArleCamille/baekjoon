#include <stdio.h>

int N, M;
int cards[100];
int hand[3]; // hand[i]가 x라는 것은 i번째 카드가 cards[x]라는 것이다.

// depth장의 패가 완성되어 있을 때, 최댓값을 구하는 함수.
// 현재까지 완성된 패는 hand 배열에 저장되어 있음.
// ()
int visit(int depth) {
    if (depth == 3) {
        // 3장 모두 완성했다면 그 합을 그대로 돌려준다.
        int sum = cards[hand[0]] + cards[hand[1]] + cards[hand[2]];
        return sum;
    }
    int maximum = 0;
    int result;
    // ternary operator (condition ? truevalue : falsevalue) -> condition이 만족되면 truevalue를, 그렇지 않으면 falsevalue를.
    // 그러니까,
    // - 한 장의 카드도 뽑은 적 없다면 첫 카드부터,
    // - 그렇지 않다면 마지막으로 쓴 카드의 다음 카드부터
    // 마지막 카드까지 다음 패로 넣어 본다.
    for (int i = (depth == 0 ? 0 : hand[depth - 1] + 1); i < N; ++i) {
        hand[depth] = i; // 해당 카드를 다음 패에 넣는다
        result = visit(depth + 1); // 이 상태의 최대의 합을 구한다
        if (result > maximum && result <= M)
            maximum = result; // 최대값 갱신
    }
    return maximum;
}

int main(void) {
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; ++i)
        scanf("%d", &cards[i]);
    
    printf("%d\n", visit(0));

    return 0;
}
#include <stdio.h>

int N, M;
int cards[100];
int hand[3];

int visit(int depth) {
    if (depth == 3) {
        int sum = cards[hand[0]] + cards[hand[1]] + cards[hand[2]];
        return sum;
    }
    int maximum = 0;
    int result;
    // ternary operator (condition ? truevalue : falsevalue) -> condition이 만족되면 truevalue를, 그렇지 않으면 falsevalue를.
    for (int i = (depth == 0 ? 0 : hand[depth - 1] + 1); i < N; ++i) {
        hand[depth] = i;
        result = visit(depth + 1);
        if (result > maximum && result <= M)
            maximum = result;
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
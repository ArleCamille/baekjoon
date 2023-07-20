#include <stdio.h>

int L, C;
char corpus[15]; // 주어진 문자들, 정렬됨
int trace[15]; // trace[x]가 i라는 것은, 지금 완성된 암호의 x번째 글자에는 corpus의 i번째 글자가 들어간다는 것
unsigned char visited[15]; // corpus[i]를 사용했는가 여부

// (depth + 1)글자가 완성되어 있을 때 (0 ~ depth), 가능한 경우를 재귀적으로 추적한다.
// 여기서, 각 글자들(corpus)은 정렬되어 있다.
void continue_trace(int depth) {
    if (depth == L - 1) { // 0~L-1번째 글자, 즉 L개의 글자 완성 == 암호 완성
        int vowels = 0; // 전체 모음의 갯수를 count. 자음의 갯수는 L - count로 구할 수 있다. (모음이 아닌 건 다 자음이니)
        for (int i = 0; i < L; ++i) {
            switch (corpus[trace[i]]) {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                    ++vowels; // 모음: a, e, i, o, u이므로 해당 글자의 경우 모음 갯수 하나 카운트
                    break;
                default:
                    break;
            }
        }
        // 최소 한 개의 모음과 최소 두 개의 자음으로 구성되어 있어야 함
        // -> 모음이 없거나 자음이 2개 미만이면 out
        if (vowels <= 0 || (L - vowels) < 2) return;
#ifdef DEBUG
        for (int i = 0; i < L; ++i) {
            printf("%d ", trace[i]);
        }
        printf("-> ");
#endif
        // 그렇지 않을 경우, 지금의 암호를 출력.
        // 저장한 형태는 숫자 나열이므로, 이를 저장한 corpus를 통해 다시 문자열로 변환.
        for (int i = 0; i < L; ++i) {
            printf("%c", corpus[trace[i]]);
        }
        printf("\n");
    } else {
        // 암호가 아직 완성되지 않음.
        visited[trace[depth]] = 1; // 지금 사용한 글자는 더 이상 사용 못함
        for (int i = trace[depth] + 1; i < C; ++i) {
            // 지금 사용한 글자보다 큰 글자에 대해...
            if (!visited[i]) {
                // 다음 글자를 배당
                trace[depth + 1] = i;
                // 한 글자 더 만들었으니 계속 만듦
                continue_trace(depth + 1);
            }
        }
        visited[trace[depth]] = 0; // 사용할 수 있게 풀어줌
    }
}

// 주어진 문자열은 정렬이 안 되어 있다. 이를 정렬.
void sort_corpus() {
    char temp;
    // 왼쪽부터 최솟값으로 채워나간다.
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < C; ++j) {
            if (corpus[j] > corpus[i]) {
                // 현재의 왼쪽보다 작은 값이 있으면 맞바꿈
                temp = corpus[i];
                corpus[i] = corpus[j];
                corpus[j] = temp;
            }
        }
    }
}

int main() {
    scanf("%d %d", &L, &C);
    char buffer[10];
    for (int i = 0; i < C; ++i) {
        scanf("%s", buffer);
        corpus[i] = buffer[0];
    }
#ifdef DEBUG
    printf("Corpus: ");
    for (int i = 0; i < C; ++i) {
        printf("%c ", corpus[i]);
    }
#endif
    // 우선 문자들을 정렬
    sort_corpus();

#ifdef DEBUG
    printf("Corpus after sort: ");
    for (int i = 0; i < C; ++i) {
        printf("%c ", corpus[i]);
    }
    printf("\n");
#endif

    // 일단 첫 번째 글자로 모든 글자를 넣어 보고, 계속한다.
    for (int i = 0; i < C; ++i) {
        trace[0] = i;
        continue_trace(0);
    }
}
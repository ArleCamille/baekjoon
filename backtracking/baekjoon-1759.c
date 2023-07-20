#include <stdio.h>

int L, C;
char corpus[15];
int trace[15];
unsigned char visited[15];

void continue_trace(int depth) {
    if (depth == L - 1) {
        int vowels = 0;
        for (int i = 0; i < L; ++i) {
            switch (corpus[trace[i]]) {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                    ++vowels;
                    break;
                default:
                    break;
            }
        }
        if (vowels <= 0 || (L - vowels) < 2) return;
#ifdef DEBUG
        for (int i = 0; i < L; ++i) {
            printf("%d ", trace[i]);
        }
        printf("-> ");
#endif
        for (int i = 0; i < L; ++i) {
            printf("%c", corpus[trace[i]]);
        }
        printf("\n");
    } else {
        visited[trace[depth]] = 1;
        for (int i = trace[depth] + 1; i < C; ++i) {
            if (!visited[i]) {
                trace[depth + 1] = i;
                continue_trace(depth + 1);
            }
        }
        visited[trace[depth]] = 0;
    }
}

void sort_corpus() {
    char temp;
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < C; ++j) {
            if (corpus[j] > corpus[i]) {
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

    sort_corpus();

#ifdef DEBUG
    printf("Corpus after sort: ");
    for (int i = 0; i < C; ++i) {
        printf("%c ", corpus[i]);
    }
    printf("\n");
#endif

    for (int i = 0; i < C; ++i) {
        trace[0] = i;
        continue_trace(0);
    }
}
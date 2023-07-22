#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
    int id;
    struct NODE* next;
    struct NODE* prev;
} station_t;

station_t* stations[1000001];
int N, M;

int main(void) {
    scanf("%d %d", &N, &M);
    int i, j, k;
    station_t *first = NULL;
    station_t *last = NULL;
    station_t *new_node;
    // 1. 초기 노선도
    for (i = 0; i < N; ++i) {
        scanf("%d", &j);
        new_node = malloc(sizeof (station_t));
        if (!first) {
            first = last = new_node;
            new_node->next = new_node;
            new_node->prev = new_node;
        } else {
            last->next = new_node;
            first->prev = new_node;

            new_node->next = first;
            new_node->prev = last;

            last = new_node;
        }
        stations[j] = new_node;
        new_node->id = j;
    }

    // 2. 명령어 받기
    char buffer[10];
    station_t *current, *tmp1, *tmp2;
    for (i = 0; i < M; ++i) {
        scanf("%s", buffer);
        switch(buffer[0]) {
        case 'B':
            // 시공 계통
            scanf("%d %d", &j, &k);
            current = stations[j];
            new_node = malloc(sizeof (station_t));
            new_node->id = k;
            switch(buffer[1]) {
            case 'N':
                // 다음 역과 상호작용
                printf("%d\n", current->next->id);
                
                new_node->prev = current;
                new_node->next = current->next;

                tmp1 = current->next;

                current->next = new_node;
                tmp1->prev = new_node;
                break;
            case 'P':
                // 이전 역과 상호작용
                printf("%d\n", current->prev->id);

                new_node->next = current;
                new_node->prev = current->prev;

                tmp1 = current->prev;

                current->prev = new_node;
                tmp1->next = new_node;
                break;
            }
            break;
        case 'C':
            // 폐쇄 계통
            switch(buffer[1]) {
            case 'N':
                // 다음 역과 상호작용
                scanf("%d", &j);
                current = stations[j];
                printf("%d\n", current->next->id);

                tmp1 = current->next;
                tmp2 = tmp1->next;

                current->next = tmp2;
                tmp2->prev = current;

                stations[tmp1->id] = NULL;
                free(tmp1);
                break;
            case 'P':
                // 이전 역과 상호작용
                scanf("%d", &j);
                current = stations[j];
                printf("%d\n", current->prev->id);

                tmp1 = current->prev;
                tmp2 = tmp1->prev;

                current->prev = tmp2;
                tmp2->next = current;

                stations[tmp1->id] = NULL;
                free(tmp1);
                break;
            }
            break;
        }
    }

    return 0;
}
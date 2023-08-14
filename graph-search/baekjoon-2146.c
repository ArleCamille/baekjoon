#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N;
unsigned short max_island_id = 1;
unsigned short map[100][100];
unsigned short bridge_map[100][100];

//#define DEBUG

typedef struct ELEMENT {
    unsigned char x;
    unsigned char y;
    unsigned short length;
    struct ELEMENT *next;
} element_t;

element_t *head = NULL, *tail = NULL;

void queue_append(element_t element) {
    // 새로운 element를 할당받아서 현재 element를 복사
    element_t *new_elem = malloc(sizeof (element_t));
    if (new_elem == NULL)
        exit(1); // 더 이상 프로그램이 동작할 수 없음
    *new_elem = element;
    if (head == NULL) {
        // 큐가 비어 있음
        head = tail = new_elem;
    } else {
        // 큐 끝에 원소 추가
        tail->next = new_elem;
        tail = new_elem;
    }
}

element_t queue_pop() {
    if (head == NULL) {
        // 큐가 비었음
        return (element_t) { .x = 0, .y = 0, .length = 0 };
    }

    element_t result = *head;
    element_t *temp = head;
    head = head->next;
    free(temp);

    return result;
}

void mark_continent(unsigned short id, int x, int y) {
    if (x >= N || x < 0 || y >= N || y < 0)
        return; // 무효 좌표
    else if (map[y][x] == 1) {
        // 마킹
        map[y][x] = id;
        mark_continent(id, x - 1, y);
        mark_continent(id, x + 1, y);
        mark_continent(id, x, y - 1);
        mark_continent(id, x, y + 1);
    }
}

// map[y][x] == 0인가?
unsigned char is_sea(int x, int y) {
    if (x < 0 || x >= N || y < 0 || y >= N)
        return 0;
    return map[y][x] == 0;
}

// map[y][x]가 source와 다른 대륙인가?
unsigned char is_different_continent(int x, int y, unsigned short source) {
    if (x < 0 || x >= N || y < 0 || y >= N)
        return 0;
    return (map[y][x] >= 2 && map[y][x] != source);
}

int main(void) {
    scanf("%d", &N);
    int y, x;
    for (y = 0; y < N; ++y)
        for (x = 0; x < N; ++x)
            scanf("%hd", &map[y][x]);
    
    // 1. 각 대륙을 서로 다른 ID로 마킹
    for (y = 0; y < N; ++y) {
        for (x = 0; x < N; ++x) {
            if (map[y][x] == 1) {
                // 대륙 마킹 시작
                mark_continent(++max_island_id, x, y);
            }
        }
    }
#ifdef DEBUG
    puts("Map status:");
    for (y = 0; y < N; ++y) {
        for (x = 0; x < N; ++x) {
            printf("%d ", map[y][x]);
        }
        puts("");
    }
#endif

    // 각 대륙마다 처리.
    int continent;
    int min_bridge_length = 32767;
    element_t iter;
    for (continent = 2; continent <= max_island_id; ++continent) {
        // 다리 길이 지도 초기화
        memset(bridge_map, 255, sizeof (bridge_map));
        // 이 대륙에서 인접한 바다를 초기값에 넣음
        for (y = 0; y < N; ++y) {
            for (x = 0; x < N; ++x) {
                if (map[y][x] == continent) {
                    if (is_sea(x - 1, y))
                        queue_append((element_t) { .x = x - 1, .y = y, .length = 1 });
                    if (is_sea(x + 1, y))
                        queue_append((element_t) { .x = x + 1, .y = y, .length = 1 });
                    if (is_sea(x, y - 1))
                        queue_append((element_t) { .x = x, .y = y - 1, .length = 1 });
                    if (is_sea(x, y + 1))
                        queue_append((element_t) { .x = x, .y = y + 1, .length = 1 });
                }
            }
        }
        // 큐에서 탐색.
        for (iter = queue_pop(); iter.length; iter = queue_pop()) {
            x = iter.x;
            y = iter.y;
            if (iter.length >= bridge_map[y][x])
                continue; // 더 짧은 다리를 만들 수 없다면 넘김
            bridge_map[y][x] = iter.length;
#ifdef DEBUG
            printf("On the sea of (%d, %d), length so far is %d from continent %d\n", iter.x, iter.y, iter.length, continent);
#endif
            if (is_different_continent(x + 1, y, continent) ||
                is_different_continent(x - 1, y, continent) ||
                is_different_continent(x, y + 1, continent) ||
                is_different_continent(x, y - 1, continent)) {
                // 탐사 중단하고 최소 다리 길이 갱신
                if (iter.length < min_bridge_length) {
                    min_bridge_length = iter.length;
#ifdef DEBUG
                    puts("Found a possible bridge");
#endif
                }
            } else {
                // 인접한 바다를 계속 큐에 넣음
                if (is_sea(x - 1, y))
                    queue_append((element_t) {.x = x - 1, .y = y, .length = iter.length + 1});
                if (is_sea(x + 1, y))
                    queue_append((element_t) {.x = x + 1, .y = y, .length = iter.length + 1});
                if (is_sea(x, y - 1))
                    queue_append((element_t) {.x = x, .y = y - 1, .length = iter.length + 1});
                if (is_sea(x, y + 1))
                    queue_append((element_t) {.x = x, .y = y + 1, .length = iter.length + 1});
            }
        }
    }
    // 출력
    printf("%d\n", min_bridge_length);

    return 0;
}
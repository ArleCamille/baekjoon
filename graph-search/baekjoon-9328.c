#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct QUEUE_ELEMENT {
    int x;
    int y;
} queue_element_t;

#define QUEUE_CAPACITY 20000

queue_element_t queue[QUEUE_CAPACITY];
int head, tail;

void queue_append(int x, int y) {
    queue[tail++] = (queue_element_t) { .x = x, .y = y };
    if (tail >= QUEUE_CAPACITY)
        tail = 0;
}

int queue_pop(int *x, int *y) {
    if (head == tail)
        return 0;
    queue_element_t retval = queue[head++];
    if (head >= QUEUE_CAPACITY)
        head = 0;
    
    *x = retval.x;
    *y = retval.y;

    return 1;
}

void clear_queue() {
    head = tail = 0;
}

int total_cases;
int width, height;
char map[100][100];
unsigned char initial_key_state[26];
char buffer[101];
unsigned char visited[100][100];
unsigned char document_marked[100][100];
unsigned char visit_event_happened;

// 지도를 현재 가지고 있는 열쇠에 따라 변형시키는 함수
void reflect_key_state() {
    int i, j;
    for (i = 0; i < height; ++i)
        for (j = 0; j < width; ++j)
            if (map[i][j] >= 'A' && map[i][j] <= 'Z' && initial_key_state[map[i][j] - 'A'])
                map[i][j] = '.'; // 열 수 있으면 문을 없앤다
}

// 갈 수 있는 곳만 남김.
// (안 열린 문과 벽은 갈 수 없고, 무효 좌표도 필터링.)
unsigned char is_accessible(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return 0;
    char map_content = map[y][x];
    if (map_content == '*' || (map_content >= 'A' && map_content <= 'Z'))
        return 0;
    return 1;
}

void traverse(int start_x, int start_y) {
    if (!is_accessible(start_x, start_y) || visited[start_y][start_x])
        return; // 이미 들렀거나 접근 불가능한 곳은 시도하지 않는다.
    // 최초 진입점을 큐에 집어넣는다.
    queue_append(start_x, start_y);
    visit_event_happened = 1;

    // 큐 방문을 시작한다.
    int i, x, y, cond;
    while (queue_pop(&x, &y)) {
        if (visited[y][x] || !is_accessible(x, y))
            continue; // 상황이 변하지 않은 상태에서 방문한 곳이나 막혀 있는 곳은 삭제
#ifdef DEBUG
            char temp_content = map[y][x];
            map[y][x] = '^';
            puts("Visiting here:");
            for (i = 0; i < height; ++i)
                puts(map[i]);
            printf("Content here: %c\n", temp_content);
            map[y][x] = temp_content;
#endif
            if (map[y][x] >= 'a' && map[y][x] <= 'z') {
                // 열쇠임
                if (!initial_key_state[map[y][x] - 'a']) {
                    // 지금까지의 정보값을 날리고 다시 정보를 쓰기 시작한다.
                    memset(visited, 0, sizeof (visited));
                    // 열쇠 추가
                    initial_key_state[map[y][x] - 'a'] = 1;
                    // 현재의 정보를 반영해 맵을 다시 변형한다.
                    reflect_key_state();
                }
#ifdef DEBUG
                puts("Map state reflecting the current state:");
                for (i = 0; i < height; ++i)
                    puts(map[i]);
#endif
            } else if (map[y][x] == '$') { // 서류
#ifdef DEBUG
                puts("Document found here!");
#endif
                document_marked[y][x] = 1;
            }
            visited[y][x] = 1;
                // 주변 추가
            if (is_accessible(x - 1, y))
                queue_append(x - 1, y);
            if (is_accessible(x + 1, y))
                queue_append(x + 1, y);
            if (is_accessible(x, y + 1))
                queue_append(x, y + 1);
            if (is_accessible(x, y - 1))
                queue_append(x, y - 1);
    }
}

int main(void) {
    scanf("%d", &total_cases);
    int i, j;
    int document_count;
    for (int current_case = 0; current_case < total_cases; ++current_case) {
        scanf("%d %d", &height, &width);
        // 지도 입력
        for (i = 0; i < height; ++i) {
            scanf("%s", buffer);
            memcpy(map[i], buffer, width);
        }
        // 초기 열쇠 상태 입력
        memset(initial_key_state, 0, sizeof (initial_key_state));
        scanf("%s", buffer);
        if (buffer[0] != '0') {
            for (i = 0; buffer[i] != '\0'; ++i) {
                initial_key_state[buffer[i] - 'a'] = 1;
            }
        }
        // 초기 지도 상태 입력
        memset(visited, 0, sizeof (visited));
        memset(document_marked, 0, sizeof (document_marked));

        // 지도를 변형시킨다.
        // 처음부터 열쇠를 가지고 있는 곳은 개방한다.
        reflect_key_state();

#ifdef DEBUG
        puts("Map state reflecting the current key state:");
        for (i = 0; i < height; ++i)
            puts(map[i]);
#endif

        // 각 진입점을 모두 들러 본다.
        while(1) {
            visit_event_happened = 0;
            for (i = 0; i < height; ++i)
                traverse(0, i); // 맨 왼쪽
            for (i = 0; i < height; ++i)
                traverse(width - 1, i); // 맨 오른쪽
            for (i = 0; i < width; ++i)
                traverse(i, 0); // 맨 위쪽
            for (i = 0; i < width; ++i)
                traverse(i, height - 1); // 맨 아래쪽

#ifdef DEBUG
            if (visit_event_happened)
                puts("Traverse complete; next traverse...");
            else {
                puts("No traversal this round; breaking...");
                break;
            }
#else
            if (!visit_event_happened)
                break;
#endif
        }

        // 큐가 빌 때까지 방문했을 때, 서류 갯수는 몇 개?
        document_count = 0;
        for (i = 0; i < height; ++i)
            for (j = 0; j < width; ++j)
                document_count += document_marked[i][j];
        printf("%d\n", document_count);
    }
    return 0;
}
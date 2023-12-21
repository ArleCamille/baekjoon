#include <stdio.h>

int N; // 학생의 수. 1~100

typedef struct STUDENT {
    int id;
    struct STUDENT *prev;
    struct STUDENT *next;
} student_t; // 각 학생을 나타내는 연결 리스트

student_t students[100]; // 편의상, 동적 메모리 할당을 쓰지 않고 100명의 학생을 미리 당겨온다.
int next_slot; // students에서 몇 번 학생이 
student_t *root; // 첫 노드.
student_t *last = NULL; // 마지막 노드. (맨 앞에 있는 사람을 먼저 출력해야 하는 듯)

int main(void) {
    scanf("%d", &N); // 학생의 번호를 받아오는 관례적인 함수
    student_t *current;
    next_slot = 0;

    int j, limit;
    student_t *temp1;
    // 첫 학생에 대한 특별한 초기화.
    current = &students[++next_slot];
    current->id = 1;
    current->next = NULL;
    current->prev = NULL;
    root = current;
    last = current;
    scanf("%d", &limit); // 여기서의 limit값은 버려진다.

    for (int i = 2; i <= N; ++i) {
        // 새로운 원소를 생성.
        current = &students[++next_slot];
        current->id = i;

        temp1 = root; // temp1의 앞쪽에 넣어야 한다는 뜻
        scanf("%d", &limit);
        for (j = 0; j < limit; ++j) {
            temp1 = temp1->next;
        }
        if (temp1 == NULL) {
            // 말단에 넣어야 한다는 뜻.
            current->prev = last;
            current->next = NULL;
            last->next = current;
            last = current;
        } else {
            // temp1의 이전에 넣는다.
            current->next = temp1;
            current->prev = temp1->prev;
            temp1->prev = current;
            if (current->prev)
                current->prev->next = current;
            else
                root = current; // 이전에 아무 노드도 없다 = root이다.
        }
    }

#ifdef DEBUG
    current = root;
    puts("List structure");
    while (current != NULL) {
        printf("Entry: %p\n", current);
        printf("Id: %d\n", current->id);
        printf("Prev: %p\n", current->prev);
        printf("Next: %p\n", current->next);
        puts("");

        current = current->next;
    }
#endif

    // 실제 줄 선 순서를 출력
    current = last;
    while (current != NULL) {
        printf("%d ", current->id);
        current = current->prev;
    }
    puts("");
}
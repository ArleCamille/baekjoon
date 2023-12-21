#include <stdio.h>
#include <stdlib.h>
typedef struct NODE {
    char content;
    struct NODE* next;
    struct NODE* prev;
} editor_node_t;

editor_node_t *root = NULL, *last = NULL, *cursor = NULL;


int M;

int main(void) {
    char buffer[100001];
    scanf("%s", buffer);
    int i;
    editor_node_t *last_so_far;
    // 1. 초기 문자열 구성
    for (i = 0; buffer[i] != '\0'; ++i) {
        last_so_far = malloc(sizeof (editor_node_t));
        last_so_far->content = buffer[i];
        if (root == NULL) {
            last_so_far->next = NULL;
            last_so_far->prev = NULL;
            root = last = last_so_far;
        } else {
            last_so_far->prev = last;
            last->next = last_so_far;
            last_so_far->next = NULL;
            last = last_so_far;
        }
    }

    // 2. 에디터 명령 수행
    editor_node_t *temp1, *temp2;
    scanf("%d", &M);
    for (i = 0; i < M; ++i) {
        scanf("%s", buffer);
        switch (buffer[0]) {
        case 'L':
            // 커서를 왼쪽으로 한 칸.
            // 커서가 이미 문장 맨 앞이면 무시
            if (cursor == NULL) { // 커서가 완전 오른쪽
                cursor = last; // 커서를 마지막 글자 왼쪽으로
            } else if (cursor->prev) {
                cursor = cursor->prev;
            }
            break;
        case 'D':
            // 커서를 오른쪽으로 한 칸.
            // 커서가 이미 문장 맨 뒤면 무시
            if (cursor) {
                // 유효성 체크인 동시에 실질적으로 '커서가 문장 맨 뒤이면 무시' 효과
                cursor = cursor->next;
            }
            break;
        case 'B':
            if (cursor == NULL) {
                // 문장 맨 마지막 글자를 삭제하고 마지막 글자를 갱신.
                temp1 = last;
                if (temp1) {
                    if (temp1->prev) {
                        temp1->prev->next = NULL;
                    } else {
                        root = NULL;
                    }
                    last = temp1->prev;
                    free(temp1);
                }
            } else if (cursor->prev) {
                // 커서 왼쪽에 있는 문자를 삭제. (문장 맨 앞이면 무시)
                temp1 = cursor->prev;
                if (temp1->prev) {
                    temp1->prev->next = cursor;
                } else {
                    root = cursor;
                }
                cursor->prev = temp1->prev;

                // 메모리를 반환
                free(temp1);
            }
            break;
        case 'P':
            // 커서 왼쪽에 문자를 하나 추가.
            scanf("%s", buffer);
            temp1 = malloc(sizeof (editor_node_t));
            temp1->content = buffer[0];
            if (cursor == NULL) {
                // 커서가 맨 마지막에 있었음.
                temp1->prev = last;
                if (last)
                    last->next = temp1;
                temp1->next = NULL;

                // 리스트 마지막을 갱신
                last = temp1;
                if (!root)
                    root = temp1;
            } else {
                // 커서가 중간에 있었음.
                temp2 = cursor->prev;

                // 커서가 첫 글자였는지, 아니었는지?
                if (temp2)
                    temp2->next = temp1;
                else
                    root = temp1;
                
                cursor->prev = temp1;
                temp1->next = cursor;
                temp1->prev = temp2;
            }
            break;
        default:
            break;
        }
    }

    // 3. 최종적으로 문자열 반환
    for (temp1 = root; temp1; temp1 = temp1->next) {
        printf("%c", temp1->content);
    }
    puts("");
}
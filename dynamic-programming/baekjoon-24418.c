#include <stdio.h>

int n;

int memory[16][16];

int search(int x, int y) {
    if (x <= 0 || y <= 0)
        return 0; // 0에 대한 호출은 호출로 취급 안 함?
    else if (memory[x][y] > 0)
        return memory[x][y]; // 기억된 값을 그대로 출력
    
    int result = search(x - 1, y) + search(x, y - 1) + 1;  // 이 노드에 한 번 + 아래쪽 호출 종합 + 왼쪽 호출 종합
    memory[x][y] = result;
    return result;
}

/*
search(5, 5)
5, 5는 4,5 + 1회와 5,4 + 1회를 부른다.
이를 일반화하면 search(x, y) = search(x-1, y) + search(x, y-1) + 1
search(x, y)의 값을 한 번 구했다면, memory 배열에 저장 
*/

int main(void) {
    scanf("%d", &n);

    int recursive_result = search(n, n);

    // dynamic programming의 결과는 심플하게 n^2
    // recursive 쪽의 경우, matrix_path가 추가로 1회 더 호출하는 걸 감안.
    printf("%d %d\n", recursive_result + 1, n * n);

    return 0;
}
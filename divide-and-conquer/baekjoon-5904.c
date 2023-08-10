#include <stdio.h>
#include <stdlib.h>

// 가변길이 배열에 관한 코드
// 알고리즘과는 별 상관 없음
typedef struct VARARRAY {
    long long *counts;
    size_t length;
} vararray_t;

vararray_t vararray_allocate() {
    size_t initial_length = 4096 / sizeof (long long);
    void *space = calloc(initial_length, sizeof (long long));
    return (vararray_t) { .counts = (long long *) space, .length = space ? initial_length : 0 };
}

long long vararray_get(vararray_t *array, long long index) {
    if (index < 0 || ((size_t) index) >= array->length)
        return -1LL;
    return array->counts[index];
}

int vararray_set(vararray_t *array, long long index, long long value) {
    if (index < 0)
        return 0; // fails automatically for negative index
    if ((size_t) index >= array->length) {
        // expand array
        size_t new_length = array->length << 1;
        while (new_length <= index)
            new_length << 1;
        void *new_space = realloc(array->counts, new_length * sizeof (long long));
        if (!new_space) {
            // failed to expand the array
            return 0;
        }
        array->counts = new_space;
        array->length = new_length;
    }
    array->counts[index] = value;
    return 1;
}

void vararray_cleanup(vararray_t *array) {
    if (array->counts)
        free(array->counts);
}

long long N;
vararray_t counts;

// remaining자가 현재의 S(func_depth) 안에서 출력이 가능하면 그것을 출력하고 -1을 반환하거나,
// S(func_depth)의 총 길이를 출력하고 부모 함수로 돌아간다.
long long print_moo(long long position, long long func_depth) {
    // -1부터는 수열 자체가 없음 (base case)
    if (func_depth < 0)
        return 0;
    
    // 약간의 최적화: 수열의 맨 앞 글자는 무조건 m
    if (!position) {
        printf("m\n");
        return -1;
    }
    
    // 일단 S(k-1)을 시도
    long long prefix = vararray_get(&counts, func_depth - 1);
    long long remaining = position;
    long long total_length = 0LL;
    if (prefix > 0 && position >= prefix) {
        // S(k-1)의 길이만큼 빼기만 함
        remaining -= prefix;
        total_length += prefix;
    } else {
        // S(k-1)의 값을 저장하거나, 길이를 구하려고 시도.
        prefix = print_moo(position, func_depth - 1);
        if (prefix < 0) // 도중 출력에 성공
            return -1;
        total_length += prefix;
        remaining -= prefix;
    }
    // o가 k + 2개인 수열 (즉 k + 3문자의 문자열) 안에 있는가?
    if (!remaining) { // == 0과 같은 의미
        printf("m\n");
        return -1;
    } else if (remaining < func_depth + 3) {
        printf("o\n");
        return -1;
    }
    remaining -= func_depth + 3;
    total_length += func_depth + 3;
    // 다시 S(k-1)을 시도
    long long suffix = vararray_get(&counts, func_depth - 1);
    if (suffix <= 0 || remaining < suffix) {
        // 저장된게 없거나 S(k - 1)의 길이 미만이라면 S(k - 1)의 길이를 저장하거나 구하려고 시도.
        suffix = print_moo(remaining, func_depth - 1);
        if (suffix < 0) // 도중 출력에 성공
            return -1;
    }
    // 총 길이 결산
    remaining -= suffix;
    total_length += suffix;
    vararray_set(&counts, func_depth, total_length);
    return total_length;
}

int main(void) {
    scanf("%lld", &N);

    counts = vararray_allocate();

    // S(i) 안에 N번째 글자가 있는 i를 계속 찾는다.
    int i;
    for (i = 0; ; ++i)
        if (print_moo(N - 1, i) < 0)
            return 0;
    // should never reach here
    return 1;
}
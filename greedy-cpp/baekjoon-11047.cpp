#include <iostream>
#include <vector>

int N, K;
std::vector<int> A;

int main(void) {
    std::cin >> N >> K;
    int temp;
    for (int i = 0; i < N; ++i) {
        std::cin >> temp;
        A.push_back(temp);
    }
    int count = 0;
    while (K > 0) {
        while (temp > K) {
            A.pop_back();
            temp = A.back();
        }
        K -= temp;
        ++count;
    }
    std::cout << count << std::endl;
    return 0;
}
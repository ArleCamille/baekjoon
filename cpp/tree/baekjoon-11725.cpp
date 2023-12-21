#include <iostream>
#include <vector>

int N;

void dfs(const std::vector<std::vector<int>> &graph, std::vector<int> &parent_of, std::vector<bool> &visited, int node, int parent) {
    if (visited[node])
        return;
    visited[node] = true;
    parent_of[node] = parent;
    for (int i: graph[node]) {
        dfs(graph, parent_of, visited, i, node);
    }
}

int main(void) {
    std::cin >> N;
    int from, to;
    std::vector<std::vector<int>> edge_list(N, std::vector<int>());
    std::vector<int> parent_of(N, 0);
    std::vector<bool> visited(N, false);
    int i;
    for (i = 0; i < N - 1; ++i) {
        std::cin >> from >> to;
        edge_list[from-1].push_back(to-1);
        edge_list[to-1].push_back(from-1);
    }

    dfs(edge_list, parent_of, visited, 0, -2);
    for (auto iter = parent_of.begin() + 1; iter != std::end(parent_of); ++iter) {
        std::cout << (*iter) + 1 << std::endl;
    }

    return 0;
}
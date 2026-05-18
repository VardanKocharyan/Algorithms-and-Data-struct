#pragma once
#include <vector>

class Graph_list {
private:
    std::vector<std::vector<int>> adj; // V + E

public:
    Graph_list() = default;
    Graph_list(size_t v) : adj(v, {}) {}
    Graph_list(std::vector<std::pair<int, int>> a);
    Graph_list(const std::vector<std::vector<int>> a) : adj(a) {}
    Graph_list(std::initializer_list<std::pair<int, int>> ilist);

    addEdge(int u, int v, bool isUndigr = true);
    void dfs(int s);
private:
    void df(int u, std::vector<bool>& v);
};

Graph_list::Graph_list(std::vector<std::pair<int, int>> a)
{
    int max_size{-1};
    for (const std::pair<int, int> nb : a) max_size = std::max(max_size, std::max(nb.first, nb.second));
    adj.resize(max_size + 1);
    for (const std::pair<int, int> nb : a) adj[nb.first].push_back(nb.second);
}

Graph_list::Graph_list(std::initializer_list<std::pair<int, int>> ilist) 
{
    int max_size{-1};
    for (const std::pair<int, int> nb : ilist) max_size = std::max(max_size, std::max(nb.first, nb.second));
    adj.resize(max_size + 1);
    for (const std::pair<int, int> nb : ilist) adj[nb.first].push_back(nb.second);
}

Graph_list::addEdge(int u, int v, bool isUndigr = true) 
{
    adj[u].push_back(v);
    if (isUndigr) adj[v].push_back(u);
}

void Graph_list::dfs(int s)
{
    int n = adj.size();
    std::vector<bool> visited(n, false);

    //for 1+ components
    for (int i{}; i < n; ++i) {
        if (!visited[i]) dfs(i, visited);
    }
}

void Graph_list::df(int u, std::vector<bool>& v)
{
    adj[u] = true;
    for (int nb : adj[u]) {
        if (!v[nb]) df(nb, v);
    }
}

#pragma once
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <iostream>

class Graph_list {
private:
    // pair.first = neighbor, pair.second = weight
    std::vector<std::vector<std::pair<int, int>>> adj; 

public:
    Graph_list() = default;
    Graph_list(size_t v) : adj(v, std::vector<std::pair<int, int>>{}) {}
    Graph_list(const std::vector<std::pair<int, int>>& a); // Unweighted input
    Graph_list(const std::vector<std::vector<std::pair<int, int>>>& a) : adj(a) {}
    Graph_list(std::initializer_list<std::pair<int, int>> ilist);

private:
    // Internal helpers
    void df(int u, std::vector<int>& v);
    bool df(int s, int d, std::vector<int>& v);
    void allPaths(int start, int end, std::vector<bool>& v, std::vector<int>& p, std::vector<std::vector<int>>& res);
    bool TopSortDFS_helper(int u, std::vector<int>& v, std::vector<int>& res);
    void tj(int u, int& time, std::vector<int>& low, std::vector<int>& disc, std::stack<int>& st, std::vector<bool>& onStack, std::vector<std::vector<int>>& scc);
    void kj_dfs1(int u, std::vector<bool>& v, std::stack<int>& st);
    void kj_dfs2(int u, std::vector<bool>& v, std::vector<int>& tmp, const std::vector<std::vector<std::pair<int, int>>>& t_adj);
    void Bridg_dfs(int u, int p, int& time, std::vector<int>& low, std::vector<int>& disc, std::vector<std::pair<int, int>>& bridges);
    void AP_dfs(int u, int p, int& time, std::vector<int>& low, std::vector<int>& disc, std::vector<bool>& isAP);
    std::vector<Edge> makeEdgeVector();
    int kruskal(std::vector<Edge>& MST);

public:
    void addEdge(int u, int v, int w = 1, bool isUndigr = true);
    void dfs(int s);
    void bfs(int s);
    bool hasPath(int s, int d);

    std::vector<std::vector<std::pair<int, int>>> getTranspose();
    std::vector<std::vector<int>> getAllPaths(int u, int v);

    // Topological Sort
    std::vector<int> TopologicalSortDFS();
    std::vector<int> TopologicalSort_Kahn();

    // Connectivity
    std::vector<std::vector<int>> Tarjan();
    std::vector<std::vector<int>> Kosaraju();
    std::vector<std::pair<int, int>> Bridg();
    std::vector<bool> ArticulationPoint();
    bool BellmanFord(int src);
    int Kruskal();
    int Prim(int start = 0);

    // DSU
    struct DSU {
        std::vector<int> parent;
        std::vector<int> rank;

        DSU(int n) { 
            parent.resize(n);
            rank.assign(n, 0);
            for (int i{}; i < adj.size(); ++i) 
                parent[i] = i; 
        }
        ~DSU() = default;

        int find(int x);
        bool unite(int x, int y);
    };

    // Edge for Kruskal
    struct Edge {
        int u, v, w;
        Edge() = default;
        Edge(const Edge& e) : u(u), v(v), w(w) {}
        ~Edge() default;
        bool operator<(const Edge& other) const { return w < other.w; }
    }

};

// --- Implementations ---

Graph_list::Graph_list(const std::vector<std::pair<int, int>>& a) {
    int max_size{-1};
    for (const auto& edge : a) max_size = std::max({max_size, edge.first, edge.second});
    adj.resize(max_size + 1);
    for (const auto& edge : a) adj[edge.first].push_back({edge.second, 1}); // Default weight 1
}

Graph_list::Graph_list(std::initializer_list<std::pair<int, int>> ilist) {
    int max_size{-1};
    for (const auto& edge : ilist) max_size = std::max({max_size, edge.first, edge.second});
    adj.resize(max_size + 1);
    for (const auto& edge : ilist) adj[edge.first].push_back({edge.second, 1});
}

void Graph_list::addEdge(int u, int v, int w, bool isUndigr) {
    adj[u].push_back({v, w});
    if (isUndigr) adj[v].push_back({u, w});
}

void Graph_list::dfs(int s) {
    int n = adj.size();
    std::vector<int> visited(n, 0); 
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) df(i, visited);
    }
}

void Graph_list::df(int u, std::vector<int>& v) {
    v[u] = 1; // Visiting
    for (auto& edge : adj[u]) {
        int nb = edge.first;
        if (v[nb] == 1) {
            std::cout << "Cycle detected at " << nb << std::endl;
        } else if (v[nb] == 0) {
            df(nb, v);
        }
    }
    v[u] = 2; // Visited
}

void Graph_list::bfs(int s) {
    if (adj.empty()) return;
    std::vector<bool> visited(adj.size(), false);
    std::queue<int> q;

    q.push(s);
    visited[s] = true;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        for (auto& edge : adj[curr]) {
            if (!visited[edge.first]) {
                visited[edge.first] = true;
                q.push(edge.first);
            }
        }
    }
}

bool Graph_list::hasPath(int s, int d) {
    std::vector<int> visited(adj.size(), 0);
    return df(s, d, visited);
}

bool Graph_list::df(int s, int d, std::vector<int>& v) {
    if (s == d) return true;
    v[s] = 1;
    for (auto& edge : adj[s]) {
        int nb = edge.first;
        if (v[nb] == 0 && df(nb, d, v)) return true;
    }
    v[s] = 2;
    return false;
}

std::vector<std::vector<std::pair<int, int>>> Graph_list::getTranspose() {
    std::vector<std::vector<std::pair<int, int>>> new_adj(adj.size());
    for (int i = 0; i < adj.size(); ++i) {
        for (auto& edge : adj[i]) {
            new_adj[edge.first].push_back({i, edge.second});
        }
    }
    return new_adj;
}

std::vector<std::vector<int>> Graph_list::getAllPaths(int u, int v) {
    std::vector<bool> visited(adj.size(), false);
    std::vector<std::vector<int>> res;
    std::vector<int> p;
    allPaths(u, v, visited, p, res);
    return res;
}

void Graph_list::allPaths(int start, int end, std::vector<bool>& v, std::vector<int>& p, std::vector<std::vector<int>>& res) {
    v[start] = true;
    p.push_back(start);

    if (start == end) {
        res.push_back(p);
    } else {
        for (auto& edge : adj[start]) {
            if (!v[edge.first]) allPaths(edge.first, end, v, p, res);
        }
    }
    p.pop_back();
    v[start] = false;
}

std::vector<int> Graph_list::TopologicalSortDFS() {
    std::vector<int> visited(adj.size(), 0);
    std::vector<int> res;
    for (int i = 0; i < adj.size(); ++i) {
        if (!visited[i] && !TopSortDFS_helper(i, visited, res)) return {}; // Cycle found
    }
    std::reverse(res.begin(), res.end());
    return res;
}

bool Graph_list::TopSortDFS_helper(int u, std::vector<int>& v, std::vector<int>& res) {
    v[u] = 1;
    for (auto& edge : adj[u]) {
        if (v[edge.first] == 1) return false;
        if (v[edge.first] == 0 && !TopSortDFS_helper(edge.first, v, res)) return false;
    }
    v[u] = 2;
    res.push_back(u);
    return true;
}

std::vector<int> Graph_list::TopologicalSort_Kahn() {
    std::vector<int> indeg(adj.size(), 0);
    for (const auto& node_edges : adj) {
        for (const auto& edge : node_edges) indeg[edge.first]++;
    }

    std::queue<int> q;
    for (int i = 0; i < adj.size(); ++i) {
        if (indeg[i] == 0) q.push(i);
    }

    std::vector<int> res;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);
        for (auto& edge : adj[u]) {
            if (--indeg[edge.first] == 0) q.push(edge.first);
        }
    }
    return (res.size() == adj.size()) ? res : std::vector<int>{};
}

std::vector<std::vector<int>> Graph_list::Tarjan() {
    int n = adj.size();
    std::vector<std::vector<int>> scc;
    std::vector<int> disc(n, -1), low(n, -1);
    std::vector<bool> onStack(n, false);
    std::stack<int> st;
    int timer = 0;

    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) tj(i, timer, low, disc, st, onStack, scc);
    }
    return scc;
}

void Graph_list::tj(int u, int& time, std::vector<int>& low, std::vector<int>& disc, std::stack<int>& st, std::vector<bool>& onStack, std::vector<std::vector<int>>& scc) {
    disc[u] = low[u] = ++time;
    st.push(u);
    onStack[u] = true;

    for (auto& edge : adj[u]) {
        int v = edge.first;
        if (disc[v] == -1) {
            tj(v, time, low, disc, st, onStack, scc);
            low[u] = std::min(low[u], low[v]);
        } else if (onStack[v]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }

    if (low[u] == disc[u]) {
        std::vector<int> tmp;
        while (true) {
            int node = st.top(); st.pop();
            onStack[node] = false;
            tmp.push_back(node);
            if (u == node) break;
        }
        scc.push_back(tmp);
    }
}

std::vector<std::vector<int>> Graph_list::Kosaraju() {
    int n = adj.size();
    std::stack<int> st;
    std::vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) kj_dfs1(i, visited, st);
    }

    auto t_adj = getTranspose();
    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> scc;

    while (!st.empty()) {
        int u = st.top(); st.pop();
        if (!visited[u]) {
            std::vector<int> component;
            kj_dfs2(u, visited, component, t_adj);
            scc.push_back(component);
        }
    }
    return scc;
}

void Graph_list::kj_dfs1(int u, std::vector<bool>& v, std::stack<int>& st) {
    v[u] = true;
    for (auto& edge : adj[u]) {
        if (!v[edge.first]) kj_dfs1(edge.first, v, st);
    }
    st.push(u);
}

void Graph_list::kj_dfs2(int u, std::vector<bool>& v, std::vector<int>& tmp, const std::vector<std::vector<std::pair<int, int>>>& t_adj) {
    v[u] = true;
    tmp.push_back(u);
    for (auto& edge : t_adj[u]) {
        if (!v[edge.first]) kj_dfs2(edge.first, v, tmp, t_adj);
    }
}

std::vector<std::pair<int, int>> Graph_list::Bridg() {
    int n = adj.size();
    std::vector<std::pair<int, int>> bridges;
    std::vector<int> low(n, -1), disc(n, -1);
    int time = 0;
    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) Bridg_dfs(i, -1, time, low, disc, bridges);
    }
    return bridges;
}

void Graph_list::Bridg_dfs(int u, int p, int& time, std::vector<int>& low, std::vector<int>& disc, std::vector<std::pair<int, int>>& bridges) {
    disc[u] = low[u] = ++time;
    for (auto& edge : adj[u]) {
        int v = edge.first;
        if (v == p) continue;
        if (disc[v] == -1) {
            Bridg_dfs(v, u, time, low, disc, bridges);
            low[u] = std::min(low[u], low[v]);
            if (low[v] > disc[u]) bridges.push_back({u, v});
        } else {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}

std::vector<bool> Graph_list::ArticulationPoint() {
    int n = adj.size();
    std::vector<int> low(n, -1), disc(n, -1);
    std::vector<bool> isAP(n, false);
    int time = 0;
    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) AP_dfs(i, -1, time, low, disc, isAP);
    }
    return isAP;
}

void Graph_list::AP_dfs(int u, int p, int& time, std::vector<int>& low, std::vector<int>& disc, std::vector<bool>& isAP) {
    disc[u] = low[u] = ++time;
    int children = 0;
    for (auto& edge : adj[u]) {
        int v = edge.first;
        if (v == p) continue;
        if (disc[v] == -1) {
            children++;
            AP_dfs(v, u, time, low, disc, isAP);
            low[u] = std::min(low[u], low[v]);
            if (p != -1 && low[v] >= disc[u]) isAP[u] = true;
        } else {
            low[u] = std::min(low[u], disc[v]);
        }
    }
    if (p == -1 && children > 1) isAP[u] = true;
}

bool Graph_list::BellmanFord(int src) {
    int n = adj.size();
    std::vector<int> dist(n, std::numeric_limits<int>::max();
    dist[src] = 0;

    for (int i{}; i < n - 1; ++i) {
        bool upd = false;
        
        for (int u{}; u < n; ++u) {
            
            if (dist[nb] == std::numeric_limits<int>::max()) continue;

            for (auto& edge : adj[u]) {
                int nb = edge.first;
                int w = edge.second;
                
                if (dist[u] + w < dist[nb]) {
                    dist[nb] = dist[u] + w;
                    upd = true;
                }
            }
        }

        if (!upd) return true;
    }

    //have a cycle
    for (int u{}; u < n; ++u) {
        
        if (dist[nb] == std::numeric_limits<int>::max()) continue;

        for (auto& edge : adj[u]) {
            int nb = edge.first;
            int w = edge.second;
            if (dist[u] + w < dist[nb]) return false;
        }
    }

    return true;
}

int Graph_list::DSU::find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

bool Graph_list::DSU::unite(int x, int y) {
    int a = Graph_list::DSU::find(x);
    int b = Graph_list::DSU::find(y);

    if (a == b) return false;

    if (rank[a] > rank[b]) {
        parent[b] = a;
    } else if (rank[a] < rank[b]) {
        parent[a] = b;
    } else {
        parent[b] = a;
        ++rank[a];
    }

    return true;
}

std::vector<typename Graph_list::Edge> Graph_list::makeEdgeVector() {
    using Edge = typename Graph_list::Edge;

    std::vector<Edge> edge;
    int n = adj.size();

    for (int i{}; i < n; ++i) {
        for (auto& [v, w] : adj[u]) {
            if (u < v) {
                Edge e(u, v, w);
                edge.push_back(std::move(e));
            }
        }
    }

    return edge;
}

std::pair<int, bool> Graph_list::kruskal(std::vector<typename Graph_list::Edge>& MST) {
    using Edge = typename Graph_list::Edge;
    using DSU = typename Graph_list::DSU;

    int n = adj.size();
    DSU dsu(n);
    std::vector<Edge> edges(std::move(makeEdgeVector()));

    std::sort(edges.begin(), edges.end(),
             [] (const Edge& lhs,
                 const Edge& rhs) {
                return lhs.w < rhs.w;
             } )

    int res{};
    for (Edge& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            res += e.w;
            MST.push_back(e);
        }

        if (MST.size() == n - 1) return {res, true};
    }

    return {res, (n == 0 || n == 1 ? true : false)};
}


int Graph_list::Kruskal() {
    std::vector<Edge> mst();
    auto res = kruskal(mst);
    return res.second ? res.first : -1;
}

int Prim(int start) {
    using tiii = std::tuple<int, int, int>; // {weight, current_node, parent}

    int n = adj.size();
    if (n == 0) return 0;

    std::vector<bool> inMST(n, false);
    std::vector<tiii> mst;
    std::priority_queue< tiii, 
                         std::vector<tiii>, 
                         std::greater<tiii> > pq;
    int res{}, used{};

    pq.push({0, start, -1});

    while (!pq.empty()) {
        auto& [w, u, p] = pq.top(); pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;

        ++used;
        res += w;

        if (p != -1) {
            mst.push_back({p, u, w});
        }

        if (used == n) return res;
        for (auto& [nb, weight] : adj[u]) {
            if (!inMST[nb]) pq.push({weight, nb, u});
        }
    }

    return used == n ? res : -1;
}

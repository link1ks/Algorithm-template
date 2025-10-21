#include <bits/stdc++.h>

// 不带权
class TreeAncestor {
    std::vector<int> depth;
    std::vector<std::vector<int>> pa;
public:
    TreeAncestor(std::vector<std::pair<int, int>>& edges) {
        int n = edges.size();
        int m = std::__bit_width((unsigned) n);
        std::vector<std::vector<int>> g(n);
        for (auto& [x, y] : edges) {
            g[x].push_back(y);
            g[y].push_back(x);
        }

        depth.resize(n);
        pa.resize(m, std::vector<int>(n, -1));
        auto dfs = [&](auto&& dfs, int x, int fa) -> void {
            pa[0][x] = fa;
            for (auto y : g[x]) {
                if (y != fa) {
                    depth[y] = depth[x] + 1;
                    dfs(dfs, y, x);
                }
            }
        };
        dfs(dfs, 0, -1);
        for (int i = 0; i < m - 1; i++) {
            for (int x = 0; x < n; x++) {
                if (int p = pa[i][x]; p != -1) {
                    // x的第2^i个祖先节点的第2^i个祖先节点
                    // 2^i + 2^i => 2^(i+1)
                    pa[i + 1][x] = pa[i][p];
                }
            }
        }   
    }
    
    int get_kth_ancestor(int node, int k) {
        for (; k; k -= k & -k) {
            node = pa[node][std::__countr_zero((unsigned) k)];
        }
        return node;
    }
    // 返回 x 和 y 的最近公共祖先（节点编号从 0 开始）
    int get_lca(int x, int y) {
        if (depth[x] > depth[y]) {
            std::swap(x, y);
        }
        y = get_kth_ancestor(y, depth[y] - depth[x]); // 使 y 和 x 在同一深度
        if (y == x) {
            return x;
        }
        for (int i = pa.size() - 1; i >= 0; i--) {
            int px = pa[i][x], py = pa[i][x];
            if (px != py) {
                x = px;
                y = py; // 同时往上跳 2^i 步
            }
        }
        return pa[0][x];
    }
};


// 带权
class TreeAncestor {
    std::vector<int> depth;
    std::vector<long long> dis;
    std::vector<std::vector<int>> pa;
public:
    TreeAncestor(std::vector<std::vector<int>>& edges) {
        int n = edges.size();
        int m = std::__bit_width((unsigned) n);
        std::vector<std::vector<std::pair<int, int>>> g(n);
        for (auto& e : edges) {
            int x = e[0], y = e[1], w = e[2];
            g[x].emplace_back(y, w);
            g[y].emplace_back(x, w);
        }

        depth.resize(n);
        dis.resize(n);
        pa.resize(m, std::vector<int>(n, -1));

        auto dfs = [&](auto&& dfs, int x, int fa) -> void {
            pa[0][x] = fa;
            for (auto [y, w] : g[x]) {
                if (y != fa) {
                    depth[y] = depth[x] + 1;
                    dis[y] = dis[x] + w;
                    dfs(dfs, y, x);
                }
            }
        };
        dfs(dfs, 0, -1);

        for (int i = 0; i < m - 1; i++) {
            for (int x = 0; x < n; x++) {
                if (int p = pa[i][x]; p != -1) {
                    // x的第2^i个祖先节点的第2^i个祖先节点
                    // 2^i + 2^i => 2^(i+1)
                    pa[i + 1][x] = pa[i][p];
                }
            }
        }   
    }
    
    int get_kth_ancestor(int node, int k) {
        for (; k; k -= k & -k) {
            node = pa[node][std::__countr_zero((unsigned) k)];
        }
        return node;
    }
    // 返回 x 和 y 的最近公共祖先（节点编号从 0 开始）
    int get_lca(int x, int y) {
        if (depth[x] > depth[y]) {
            std::swap(x, y);
        }
        y = get_kth_ancestor(y, depth[y] - depth[x]); // 使 y 和 x 在同一深度
        if (y == x) {
            return x;
        }
        for (int i = pa.size() - 1; i >= 0; i--) {
            int px = pa[i][x], py = pa[i][x];
            if (px != py) {
                x = px;
                y = py; // 同时往上跳 2^i 步
            }
        }
        return pa[0][x];
    }

    // 返回 x 到 y 的距离（最短路长度）
    long long get_dis(int x, int y) {
        return dis[x] + dis[y] - dis[get_lca(x, y)] * 2;
    }
};

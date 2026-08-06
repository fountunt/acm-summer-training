# 拓扑排序 (Topological Sort)

## 概念

对有向无环图（DAG）的顶点排成线性序列，使得对每条有向边 u→v，u 都排在 v 之前。若图中存在环，则不存在拓扑序。

---

## Kahn 算法（BFS）— O(n + m)

> 核心：反复取入度为 0 的点加入拓扑序，并删除其出边。队列实现，最常用。

```cpp
// g: 邻接表，n: 顶点数（1..n）
// 返回拓扑序；若图中存在环，返回空 vector
vector<int> topo_sort(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n + 1, 0);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++indeg[v];

    queue<int> q;
    for (int u = 1; u <= n; ++u)
        if (indeg[u] == 0) q.push(u);

    vector<int> res;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        res.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);
    }
    return res.size() == n ? res : vector<int>{};  // 空 = 有环
}
```

### 判断是否有环

```cpp
// 若 res.size() != n，说明存在环（残留的入度 > 0 的点在环上）
bool has_cycle = (res.size() != n);
```

---

## 字典序最小 / 最大拓扑序 — O(n log n)

> 当题目要求输出字典序最小的拓扑序时，用 `priority_queue`（或 `set`）替代普通队列。
> 字典序最大则将队列换成 `priority_queue<int>` 大根堆。

```cpp
// 字典序最小拓扑序
vector<int> topo_lex_min(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n + 1, 0);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++indeg[v];

    priority_queue<int, vector<int>, greater<int>> pq;  // 小根堆
    for (int u = 1; u <= n; ++u)
        if (indeg[u] == 0) pq.push(u);

    vector<int> res;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        res.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) pq.push(v);
    }
    return res.size() == n ? res : vector<int>{};
}
```

> 注意：反图 + 倒序输出同样可以求拓扑序，但字典序性质会改变（反图求的是"倒序字典序"），需按题意选择。

---

## DFS 拓扑排序（基于时间戳）— O(n + m)

> 用 DFS 时间戳 `st[u]`（入栈）与 `ed[u]`（出栈）判环；若 `u` 的邻居 `v` 的 `st[v]` 已标记且未出栈，则存在环。常用于需要边信息或递归处理的场景。

```cpp
// 返回是否无环；topo 中存逆拓扑序，需 reverse
bool dfs_topo(int n, const vector<vector<int>>& g,
              vector<int>& st, vector<int>& ed, vector<int>& topo,
              int& timer, int u) {
    st[u] = ++timer;  // 入栈
    for (int v : g[u]) {
        if (st[v] == 0) {              // 未访问，递归
            if (!dfs_topo(n, g, st, ed, topo, timer, v)) return false;
        } else if (ed[v] == 0) {       // 已在栈中（祖先），存在环
            return false;
        }
    }
    ed[u] = ++timer;  // 出栈
    topo.push_back(u);
    return true;
}

// 入口：遍历所有未访问节点
bool topo_by_dfs(int n, const vector<vector<int>>& g, vector<int>& topo) {
    vector<int> st(n + 1, 0), ed(n + 1, 0);
    int timer = 0;
    topo.clear();
    for (int u = 1; u <= n; ++u)
        if (st[u] == 0)
            if (!dfs_topo(n, g, st, ed, topo, timer, u)) return false;
    reverse(topo.begin(), topo.end());  // DFS 出栈序是逆拓扑序
    return true;
}
```

---

## DAG 上的 DP（最长路 / 计数）— O(n + m)

> 拓扑序是 DAG 上 DP 的自然顺序：按拓扑序递推，转移依赖的"前驱"必然先处理完。
> 适合 DAG 最长路径、最长链、关键路径、路径计数等问题。

### 最长路（从源点出发）

```cpp
// 求 DAG 上从起点 s 出发到各点的最长距离
// dp[u] = max(dp[v] + w) 的等价形式，v 为 u 的前驱
vector<long long> dag_longest_path(int n, const vector<vector<pair<int,int>>>& g,
                                   int s, const vector<int>& topo) {
    const long long NEG = -1e18;
    vector<long long> dp(n + 1, NEG);
    dp[s] = 0;
    for (int u : topo) {              // 按拓扑序
        if (dp[u] == NEG) continue;
        for (auto& [v, w] : g[u])
            dp[v] = max(dp[v], dp[u] + w);
    }
    return dp;
}
```

### 路径计数（取模）

```cpp
const long long MOD = 1e9 + 7;
// 从 s 到各点的路径条数
vector<long long> dag_path_cnt(int n, const vector<vector<int>>& g,
                               int s, const vector<int>& topo) {
    vector<long long> dp(n + 1, 0);
    dp[s] = 1;
    for (int u : topo)
        for (int v : g[u])
            dp[v] = (dp[v] + dp[u]) % MOD;
    return dp;
}
```

---

## 模板题应用示例

> 经典组合：**拓扑排序 + 树上/图上 DP**。
> 例：已知 `x_i > x_j` 的若干约束（本质是建边 j→i），求满足约束的最小字典序排列，或求可行方案数。

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n + 1);
    vector<int> indeg(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        int u, v;  // 约束: u 先于 v
        cin >> u >> v;
        g[u].push_back(v);
        ++indeg[v];
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    for (int u = 1; u <= n; ++u)
        if (indeg[u] == 0) pq.push(u);

    vector<int> res;
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        res.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) pq.push(v);
    }

    if ((int)res.size() != n) {
        cout << "-1\n";  // 存在环，无解
        return 0;
    }
    for (int x : res) cout << x << ' ';
    cout << '\n';
    return 0;
}
```

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 判环 | Kahn 算法 `res.size() != n` 即为有环；DFS 判环用"在栈中"而非"已访问" |
| 队列选择 | 普通 queue 得到的是按入度 0 顺序的某个合法拓扑序，不保证字典序 |
| 字典序 | 需要字典序最小/最大时换 `priority_queue`，代价 O(n log n) |
| 建边方向 | 约束 `a 先于 b` 建边 a→b；反过来建边则拓扑序方向相反 |
| 多起点 | 入度 0 的点可能有多个，拓扑序不唯一 |
| DAG 依赖 | 拓扑序上才能安全做 DP（转移顺序保证正确），有环时不能直接 DP |
| 下标 | 顶点 1..n，indeg 数组开 n+1 |

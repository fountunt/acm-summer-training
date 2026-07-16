# SPFA（Shortest Path Faster Algorithm）

## 概念

SPFA 是 Bellman-Ford 的队列优化版本，用于：
- **单源最短路**（可以处理负权边，比 Dijkstra 适用范围更广）
- **正环 / 负环检测**（这是 Floyd / Dijkstra 做不了的）

核心思想：只有**本轮被更新过**的点才可能去松弛它的邻居，用队列避免无意义扫描。

---

## 基础模板：最短路 + 负环检测

```cpp
struct SPFA {
    int n;
    vector<vector<pair<int, long long>>> g;  // 邻接表: (to, weight)
    vector<long long> dist;
    vector<int> cnt;    // 记录到每个点的最短路边数
    vector<bool> inq;   // 是否在队列中

    SPFA(int n_) : n(n_), g(n_ + 1) {}

    void addEdge(int u, int v, long long w) {
        g[u].push_back({v, w});
    }

    // 返回是否有负环
    bool spfa(int s) {
        dist.assign(n + 1, LLONG_MAX / 2);
        cnt.assign(n + 1, 0);
        inq.assign(n + 1, false);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        inq[s] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = false;

            for (auto [v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {        // 最短路松弛
                    dist[v] = dist[u] + w;
                    cnt[v] = cnt[u] + 1;
                    if (cnt[v] >= n) return true;   // 负环
                    if (!inq[v]) {
                        q.push(v);
                        inq[v] = true;
                    }
                }
            }
        }
        return false;  // 无负环
    }

    long long getDist(int v) { return dist[v]; }
};
```

**使用**：
```cpp
SPFA sp(n);
for (int i = 0; i < m; i++) {
    int u, v, w; cin >> u >> v >> w;
    sp.addEdge(u, v, w);
}
if (sp.spfa(1)) cout << "有负环\n";
else cout << sp.getDist(n) << '\n';
```

---

## 判负环（超级源点版）

当图不保证连通时，从某个单点出发可能跑不到所有环。需要加**超级源点**：

```cpp
bool hasNegativeCycle() {
    // dist 初始化为 0 而不是 INF，等价于加超级源点 s 连 0 边到所有点
    dist.assign(n + 1, 0);
    cnt.assign(n + 1, 0);
    inq.assign(n + 1, true);   // 所有点入队
    queue<int> q;
    for (int i = 1; i <= n; i++) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = false;
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) return true;  // 负环
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return false;
}
```

**dist[v] = 0 为什么能抓到所有负环？**
- 超级源点 s 用 0 权边连到所有真实点
- 从 s 出发：s → 任意 v 的距离 = 0（第一步）
- 之后走真实边，如果图里有负环，路径权值会不断减小
- cnt[v] ≥ n → 走了 ≥ n 条边还在减小 → 负环

---

## 判正环（本题模型）

把松弛条件从 `<` 改成 `>` 即可：

```cpp
bool hasPositiveCycle(double logw) {
    vector<double> dist(n + 1, 0.0);
    vector<int> cnt(n + 1, 0);
    vector<bool> inq(n + 1, true);
    queue<int> q;
    for (int i = 1; i <= n; i++) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = false;
        for (auto [v, w_base] : g[u]) {
            double nd = dist[u] + w_base + logw;
            if (nd > dist[v]) {              // 判正环用 >
                dist[v] = nd;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) return true;  // 正环
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return false;
}
```

---

## 手写队列优化（卡常专用）

某些毒瘤 OJ 上 STL queue 可能被卡，用手写循环队列：

```cpp
struct SPFA {
    int n;
    vector<vector<pair<int, int>>> g;
    vector<long long> dist;
    vector<int> cnt, q;       // q 是手写队列
    vector<bool> inq;
    int qh, qt;               // 队头、队尾

    SPFA(int n_) : n(n_), g(n_ + 1), q(n_ + 5) {}

    void addEdge(int u, int v, int w) {
        g[u].push_back({v, w});
    }

    bool spfa(int s) {
        dist.assign(n + 1, LLONG_MAX / 2);
        cnt.assign(n + 1, 0);
        inq.assign(n + 1, false);
        qh = qt = 0;

        dist[s] = 0;
        q[qt++] = s;
        inq[s] = true;

        while (qh != qt) {
            int u = q[qh++];
            if (qh == n + 5) qh = 0;
            inq[u] = false;

            for (auto [v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    cnt[v] = cnt[u] + 1;
                    if (cnt[v] >= n) return true;
                    if (!inq[v]) {
                        q[qt++] = v;
                        if (qt == n + 5) qt = 0;
                        inq[v] = true;
                    }
                }
            }
        }
        return false;
    }
};
```

---

## SLF 优化（Small Label First）

出队时优先处理 dist 较小的点，加速收敛：

```cpp
// 替换 queue 为 deque
deque<int> dq;
dq.push_back(s);
inq[s] = true;

while (!dq.empty()) {
    int u = dq.front(); dq.pop_front();
    inq[u] = false;
    for (auto [v, w] : g[u]) {
        if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            cnt[v] = cnt[u] + 1;
            if (cnt[v] >= n) return true;
            if (!inq[v]) {
                if (!dq.empty() && dist[v] < dist[dq.front()])
                    dq.push_front(v);   // dist 小 → 放前面优先处理
                else
                    dq.push_back(v);
                inq[v] = true;
            }
        }
    }
}
```

---

## 常见误区

| 误区 | 说明 |
|------|------|
| SPFA 比 Dijkstra 快 | **错**。SPFA 最坏 O(n·m)，Dijkstra+堆 O(m log n)。正权图用 Dijkstra |
| `cnt[v] > n` 判环 | `cnt[v] >= n` 才是对的（n 条边已经能确定有环） |
| 只从一个源点跑就能判全图 | 图不连通时单源跑不到所有环，必须用超级源点或所有点入队 |
| 判正环用 `<` | 正环用 `>`（要变大）、负环用 `<`（要变小），别混淆 |
| dist 开 `long long` 溢出 | INF 要设 `LLONG_MAX / 2` 防加法溢出 |

---

## 复杂度

| 情况 | 复杂度 |
|------|--------|
| **平均** | O(m) ~ O(m log n) |
| **最坏** | O(n·m)（网格图、菊花图等构造数据） |
| **判环** | O(n·m)（最坏需要 n 轮才能确定有环） |

> 注意：SPFA 在竞赛中可能被卡（尤其是 Codeforces 的 hack 机制）。  
> 正权图**永远用 Dijkstra**，只有负权边或判环时才用 SPFA / Bellman-Ford。

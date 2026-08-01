# 最小生成树 (Minimum Spanning Tree)

## 概念

在无向连通图中，找出一个边集，连接所有顶点且总权值最小。

---

## Kruskal — O(m log m)

> 基于**边**的贪心 + 并查集。适合稀疏图（m 较小）。

```cpp
struct Edge {
    int u, v, w;
    bool operator<(const Edge& o) const { return w < o.w; }
};

struct DSU {
    vector<int> fa, sz;
    DSU(int n) {
        fa.resize(n);
        sz.resize(n, 1);
        for (int i = 0; i < n; ++i) fa[i] = i;
    }
    int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        fa[y] = x; sz[x] += sz[y];
        return true;
    }
};

// 返回 (最小生成树权值, 是否能构成生成树)
pair<long long, bool> kruskal(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end());
    DSU dsu(n + 1);
    long long ans = 0;
    int cnt = 0;
    for (auto& [u, v, w] : edges) {
        if (dsu.unite(u, v)) {
            ans += w;
            if (++cnt == n - 1) break;
        }
    }
    return {ans, cnt == n - 1};
}
```

---

## Prim — O((n + m) log n)

> 基于**点**的贪心 + 优先队列。适合稠密图（m 接近 n²）。

```cpp
using PII = pair<int, int>;  // (权值, 顶点)

// 邻接表: g[u] = {v, w}
long long prim(int n, const vector<vector<pair<int, int>>>& g) {
    const long long INF = 1e18;
    vector<long long> dis(n + 1, INF);
    vector<bool> vis(n + 1, false);
    priority_queue<PII, vector<PII>, greater<PII>> pq;

    long long ans = 0;
    int cnt = 0;
    dis[1] = 0;
    pq.emplace(0, 1);

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        ans += d;
        if (++cnt == n) break;
        for (auto& [v, w] : g[u]) {
            if (!vis[v] && w < dis[v]) {
                dis[v] = w;
                pq.emplace(w, v);
            }
        }
    }
    return cnt == n ? ans : -1;  // -1 表示不连通
}
```

---

## 0-1 MST 特化 — O(n + m)

> 权重只有 0 和 1 的完全图。
> 核心思想：先用 0 边把顶点连成若干连通块（补图 BFS），块内用 0 边连接，块间必有一条 1 边。
> 答案 = (连通块数 - 1) × 1。

```cpp
// n: 顶点数, bad[u] 存与 u 相连的权值为 1 的边
int zero_one_mst(int n, const vector<vector<int>>& bad) {
    set<int> unvisited;                // 尚未访问的顶点集合
    for (int i = 1; i <= n; ++i) unvisited.insert(i);

    int comps = 0;
    while (!unvisited.empty()) {
        ++comps;
        int s = *unvisited.begin();
        unvisited.erase(unvisited.begin());
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            // 标记与 u 有 1 边的邻居，剩余未访问顶点都与 u 有 0 边
            vector<int> removed;
            for (int v : unvisited) {
                // 检查 v 是否与 u 有 0 边 (即不在 bad[u] 中)
                if (!binary_search(bad[u].begin(), bad[u].end(), v)) {
                    removed.push_back(v);
                }
            }
            for (int v : removed) {
                unvisited.erase(v);
                q.push(v);
            }
        }
    }
    return comps - 1;  // 最小生成树权值 = (块数 - 1)
}
```

在 0-1 MST 问题中，`bad[u]` 需要**事先排序**以支持 `binary_search`。若 `bad[u]` 未排序，可用 `unordered_set` 代替。

---

## 次小生成树 (Strictly Second MST)

```cpp
// 先用 Kruskal 求出 MST，然后枚举每条不在 MST 中的边
// 用它替换 MST 中对应路径上的最大边，取最小值
// 需要 LCA + 树上倍增维护路径最大边

struct LCA {
    int LOG;
    vector<int> dep;
    vector<vector<pair<int,int>>> g; // g[u] = {v, w}
    vector<vector<int>> up, mx;

    LCA(int n, const vector<vector<pair<int,int>>>& graph) : g(graph) {
        LOG = __lg(n) + 1;
        dep.resize(n + 1);
        up.assign(n + 1, vector<int>(LOG));
        mx.assign(n + 1, vector<int>(LOG, 0));
        dfs(1, 0, 0);
    }

    void dfs(int u, int p, int w) {
        up[u][0] = p;
        mx[u][0] = w;
        for (int j = 1; j < LOG; ++j) {
            up[u][j] = up[up[u][j - 1]][j - 1];
            mx[u][j] = max(mx[u][j - 1], mx[up[u][j - 1]][j - 1]);
        }
        for (auto& [v, w2] : g[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            dfs(v, u, w2);
        }
    }

    int query_max(int u, int v) {
        int res = 0;
        if (dep[u] < dep[v]) swap(u, v);
        int diff = dep[u] - dep[v];
        for (int j = 0; j < LOG; ++j) {
            if (diff >> j & 1) {
                res = max(res, mx[u][j]);
                u = up[u][j];
            }
        }
        if (u == v) return res;
        for (int j = LOG - 1; j >= 0; --j) {
            if (up[u][j] != up[v][j]) {
                res = max({res, mx[u][j], mx[v][j]});
                u = up[u][j];
                v = up[v][j];
            }
        }
        res = max({res, mx[u][0], mx[v][0]});
        return res;
    }
};
```

---

## 完全图特殊公式 MST（边权由点权推导）

> n 很大（如 1e5），不能 O(n²) 建图。利用公式性质只保留 O(n) 条关键边，再跑 Kruskal。

### 曼哈顿距离 MST — O(n log n)

> 边权 w(i,j) = |x_i - x_j| + |y_i - y_j|
> 核心：每个点只需向 4 个方向（45° 扇区）各连最近的 1 个点，共 4n 条边。

```cpp
struct Point { int x, y, id; };

// 生成曼哈顿距离的候选边（4n 条），存入 edges
void build_manhattan_edges(vector<Point>& p, vector<Edge>& edges) {
    int n = p.size();
    // 四种坐标变换，对应四个方向
    for (int rot = 0; rot < 4; ++rot) {
        // 按 x+y 排序后，只需考虑 x'-y' 最小的前驱
        sort(p.begin(), p.end(), [](const Point& a, const Point& b) {
            return a.x + a.y < b.x + b.y;
        });
        // 离散化 y - x
        vector<int> vy;
        for (auto& pt : p) vy.push_back(pt.y - pt.x);
        sort(vy.begin(), vy.end());
        vy.erase(unique(vy.begin(), vy.end()), vy.end());

        // BIT 维护 (y-x) 对应最小的 (x+y) 及其 id
        int m = vy.size();
        vector<int> bit(m + 2, 1e9), id(m + 2, -1);
        auto upd = [&](int pos, int val, int idx) {
            for (++pos; pos <= m; pos += pos & -pos)
                if (val < bit[pos]) bit[pos] = val, id[pos] = idx;
        };
        auto qry = [&](int pos) {
            int res = 1e9, idx = -1;
            for (++pos; pos; pos -= pos & -pos)
                if (bit[pos] < res) res = bit[pos], idx = id[pos];
            return idx;
        };

        for (int i = n - 1; i >= 0; --i) {
            int rk = lower_bound(vy.begin(), vy.end(), p[i].y - p[i].x) - vy.begin();
            int j = qry(rk);
            if (j != -1) {
                int w = abs(p[i].x - p[j].x) + abs(p[i].y - p[j].y);
                edges.push_back({p[i].id, p[j].id, w});
            }
            upd(rk, p[i].x + p[i].y, i);
        }
        // 旋转坐标：(x,y) -> (y, -x)，处理下一个扇区
        for (auto& pt : p) swap(pt.x, pt.y), pt.x = -pt.x;
    }
}
```

### 异或 MST — O(n log A)，A 为值域

> 边权 w(i,j) = a_i XOR a_j
> 核心：建 01 字典树，按位从高到低分治，左右子树之间连一条最小异或边。

```cpp
struct Trie01 {
    vector<array<int, 2>> ch;
    int tot;
    Trie01(int n) { ch.resize(n * 32, {0, 0}); tot = 1; }

    void insert(int x) {
        int p = 1;
        for (int i = 30; i >= 0; --i) {
            int c = x >> i & 1;
            if (!ch[p][c]) ch[p][c] = ++tot;
            p = ch[p][c];
        }
    }
    // 查询与 x 异或最小的数
    int query_min(int x) {
        int p = 1, res = 0;
        for (int i = 30; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch[p][c]) p = ch[p][c];
            else p = ch[p][c ^ 1], res |= 1 << i;
        }
        return res;
    }
};

// 分治求异或 MST 总权值
long long xor_mst(vector<int> a, int l, int r, int bit, Trie01& trie) {
    if (bit < 0 || l >= r) return 0;
    int mid = l - 1;
    for (int i = l; i <= r; ++i)
        if (!(a[i] >> bit & 1)) swap(a[++mid], a[i]);

    long long res = xor_mst(a, l, mid, bit - 1, trie)
                  + xor_mst(a, mid + 1, r, bit - 1, trie);

    if (mid < l || mid >= r) return res; // 只有一边有元素

    // 左边插入 trie，右边查询最小异或边
    trie = Trie01(mid - l + 1);
    for (int i = l; i <= mid; ++i) trie.insert(a[i]);

    int mn = 2e9;
    for (int i = mid + 1; i <= r; ++i)
        mn = min(mn, trie.query_min(a[i]));

    return res + mn;
}
```

### 模运算 MST — O(V log V)，V 为值域

> 边权 w(i,j) = min(a_i mod a_j, a_j mod a_i) = 较小数对较大数取模
> 核心：枚举每个数 v 的倍数区间，每个区间取最小存在数连边，共 O(V log V) 条边。

```cpp
// a: 去重排序后的数组，返回 MST 总权值
ll mod_mst(vector<int> a) {
    int k = a.size();
    if (k <= 1) return 0;
    int maxn = a.back();

    vector<int> val_to_idx(maxn + 2, -1);
    for (int i = 0; i < k; ++i) val_to_idx[a[i]] = i;

    // next_exist[x]: >= x 的最小存在数
    vector<int> next_exist(maxn + 2, maxn + 1);
    for (int x = maxn; x >= 0; --x) {
        if (val_to_idx[x] != -1) next_exist[x] = x;
        else next_exist[x] = next_exist[x + 1];
    }

    vector<tuple<int, int, int>> edges; // (w, u, v)
    for (int i = 0; i < k; ++i) {
        int v = a[i];
        for (int j = v; j <= maxn; j += v) {
            int start = (j == v) ? (v + 1) : j; // 跳过 v 自身
            if (start > maxn) break;
            int u = next_exist[start];
            if (u < j + v) { // u 在 [j, j+v) 区间内
                edges.emplace_back(u % v, i, val_to_idx[u]);
            }
        }
    }

    sort(edges.begin(), edges.end());
    DSU dsu(k);
    ll ans = 0;
    int cnt = 0;
    for (auto& [w, u, v] : edges) {
        if (dsu.unite(u, v)) {
            ans += w;
            if (++cnt == k - 1) break;
        }
    }
    return ans;
}
```

### 其他常见公式

| 公式 | 处理方法 | 边数 |
|------|----------|------|
| \|x_i - x_j\| + \|y_i - y_j\| | 曼哈顿 MST，4 方向 + BIT | 4n |
| max(\|x_i - x_j\|, \|y_i - y_j\|) | 切比雪夫距离，转曼哈顿后同理 | 4n |
| a_i XOR a_j | 01 Trie + 分治 Borůvka | n log A |
| a_i mod a_j | 枚举倍数区间 + next_exist | V log V |
| a_i + a_j | 直接排序后相邻连边即可 | n-1 |
| min(a_i, a_j) | 完全图，答案 = 全局最小值 × (n-1) + 总和 - 最小值 | - |

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 下标 | 顶点通常 1..n，并查集大小开 n+1 |
| 不连通 | Kruskal 返回的 cnt < n-1 或 Prim 返回 -1 说明图不连通 |
| 重边 | Kruskal 自动处理重边（取最小），Prim 是邻接表取 min |
| 0-1 MST | 不要直接建完全图 O(n²)，用补图 BFS 思想 |
| 公式生成边 | 曼哈顿/异或等完全图问题，利用性质只建 O(n) 条边再 Kruskal |
| 求 MST 边集 | Kruskal 可以在 unite 成功时记录 Edge 到 vector |
| long long | 权值和可能超过 int，用 long long |

# 树上倍增 (Binary Lifting on Tree)

## 核心思想

预处理每个节点向上跳 $2^j$ 步到达的祖先，以及路径上的信息（max / min / sum / xor / gcd 等）。  
查询时将 $k$ 拆成二进制，在 $\mathcal{O}(\log n)$ 内完成。

---

## 通用预处理框架

```cpp
// n: 节点数, tree[u] = {v, w}
// dep[1] = 0, 根为 1
const int LOG = __lg(n) + 1;
vector<int> dep(n + 1);
vector<vector<int>> up(n + 1, vector<int>(LOG));
// value[u][j] 表示 u 向上跳 2^j 步路径上的某种信息
vector<vector<int>> val(n + 1, vector<int>(LOG));

function<void(int, int, int)> dfs = [&](int u, int p, int w) {
    up[u][0] = p;
    val[u][0] = w;                    // 初始化跳 1 步的信息
    for (int j = 1; j < LOG; ++j) {
        up[u][j] = up[up[u][j - 1]][j - 1];
        val[u][j] = merge(val[u][j - 1], val[up[u][j - 1]][j - 1]);
    }
    for (auto& [v, w2] : tree[u]) {
        if (v == p) continue;
        dep[v] = dep[u] + 1;
        dfs(v, u, w2);
    }
};
dfs(1, 0, 0);
```

其中 `merge(a, b)` 根据需求定义：

| 用途 | merge |
|------|-------|
| 路径最大边 | `max(a, b)` |
| 路径最小边 | `min(a, b)` |
| 路径权值和 | `a + b` |
| 路径异或 | `a ^ b` |
| 路径 GCD | `gcd(a, b)` |

---

## 基础操作

### 1. 查询第 k 级祖先

```cpp
int kth_ancestor(int u, int k) {
    for (int j = 0; j < LOG; ++j)
        if (k >> j & 1) u = up[u][j];
    return u;
}
// kth_ancestor(u, 3) 返回 u 的曾祖父（向上跳 3 步）
// 注意：k 超过 dep[u] 时会跳到 0（根的上一个虚拟节点）
```

### 2. 查询到根的路径信息

```cpp
int query_to_root(int u) {
    int res = identity;  // 根据 merge 操作取单位元
    for (int j = LOG - 1; j >= 0; --j) {
        if (dep[u] >= (1 << j)) {
            res = merge(res, val[u][j]);
            u = up[u][j];
        }
    }
    return res;
}
```

---

## LCA（倍增版）

```cpp
int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    int diff = dep[u] - dep[v];
    for (int j = 0; j < LOG; ++j)
        if (diff >> j & 1) u = up[u][j];
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; --j)
        if (up[u][j] != up[v][j])
            u = up[u][j], v = up[v][j];
    return up[u][0];
}
```

---

## 路径查询（任意两点 u → v）

### 路径上的最大边

```cpp
int path_max(int u, int v) {
    int res = 0;
    if (dep[u] < dep[v]) swap(u, v);
    int diff = dep[u] - dep[v];
    for (int j = 0; j < LOG; ++j)
        if (diff >> j & 1) res = max(res, val[u][j]), u = up[u][j];
    if (u == v) return res;
    for (int j = LOG - 1; j >= 0; --j)
        if (up[u][j] != up[v][j]) {
            res = max({res, val[u][j], val[v][j]});
            u = up[u][j], v = up[v][j];
        }
    res = max({res, val[u][0], val[v][0]});
    return res;
}
```

### 路径上的权值和

```cpp
ll path_sum(int u, int v) {
    ll res = 0;
    if (dep[u] < dep[v]) swap(u, v);
    int diff = dep[u] - dep[v];
    for (int j = 0; j < LOG; ++j)
        if (diff >> j & 1) res += val[u][j], u = up[u][j];
    if (u == v) return res;
    for (int j = LOG - 1; j >= 0; --j)
        if (up[u][j] != up[v][j]) {
            res += val[u][j] + val[v][j];
            u = up[u][j], v = up[v][j];
        }
    res += val[u][0] + val[v][0];
    return res;
}
```

---

## 常见变形

### 1. 路径上的第 k 小/大值

不能直接用倍增合并（max/min 可合并，但 k-th 不可合并），需要**倍增跳到目标位置**配合别的数据结构（如可持久化线段树）。

### 2. 路径上的严格次大值

```cpp
// val 存 pair<int,int> = (最大值, 次大值)
auto merge = [](pair<int,int> a, pair<int,int> b) -> pair<int,int> {
    int mx = max(a.first, b.first);
    int sec = max({a.first == mx ? a.second : a.first,
                   b.first == mx ? b.second : b.first, 0});
    return {mx, sec};
};
```

### 3. 链上二分（在 u → v 路径上找满足条件的最深节点）

```cpp
// 从 u 往上跳，找到最深且满足 check 的节点
int search_up(int u, int top) {  // top 是上边界（不含）
    for (int j = LOG - 1; j >= 0; --j)
        if (dep[up[u][j]] > dep[top] && check(u, j))
            u = up[u][j];
    return u;
}
```

### 4. 同时维护多个信息

```cpp
struct Info {
    int mx, mn, cnt;
    ll sum;
};

Info merge(Info a, Info b) {
    return {max(a.mx, b.mx), min(a.mn, b.mn),
            a.cnt + b.cnt, a.sum + b.sum};
};
```

---

## 细节 & 注意事项

| 要点 | 说明 |
|------|------|
| LOG 取值 | `__lg(n) + 1`，n=2e5 时 LOG=19 |
| 根节点 | 根节点的 `up[root][0] = 0`，`val[root][0]` 为单位元 |
| 建树顺序 | 必须先建好**树**（不是原图），树上倍增只能在树上跑 |
| 单位元 | max → 0, min → INF, sum → 0, xor → 0, gcd → 0 |
| 记忆化 | `val[u][j-1]` 和 `up[u][j-1]` 的下标不能反 |
| 数组类型 | 权值 > 2e9 用 long long |

---

## 时间复杂度

| 操作 | 复杂度 |
|------|--------|
| 预处理 DFS | O(n log n) |
| 查询 LCA | O(log n) |
| 查询路径信息 | O(log n) |
| 查询第 k 级祖先 | O(log n) |
| 空间 | O(n log n) |

# 前缀和与差分 (Prefix Sum & Difference Array)

## 概念

前缀和与差分互为**逆运算**：

| 操作 | 前缀和 | 差分 |
|------|--------|------|
| 定义 | `pref[i] = sum(a[1..i])` | `diff[i] = a[i] - a[i-1]` |
| 区间查询 | O(1) | O(n) |
| 区间修改 | O(n) | O(1) |

**核心思想：**
- **前缀和**：预处理 O(n)，查询 O(1) — 用于频繁区间求和
- **差分**：修改 O(1)，还原 O(n) — 用于频繁区间加值
- 对 diff 求前缀和 = 原数组；对 a 求前缀和 = pref

---

## 一、一维前缀和

### 构建与区间查询

```cpp
vector<int> a(n + 1);               // 原数组，1-indexed
for (int i = 1; i <= n; ++i) cin >> a[i];

vector<long long> pref(n + 1, 0);   // 前缀和数组
for (int i = 1; i <= n; ++i)
    pref[i] = pref[i - 1] + a[i];

// 查询区间 [l, r] 的和
auto range_sum = [&](int l, int r) -> long long {
    return pref[r] - pref[l - 1];
};
```

### 扩展：前缀异或 / 前缀积 / 前缀 max

```cpp
// 前缀异或 — 可用来 O(1) 求区间异或
vector<int> pxor(n + 1, 0);
for (int i = 1; i <= n; ++i) pxor[i] = pxor[i - 1] ^ a[i];
// [l, r] 异或 = pxor[r] ^ pxor[l - 1]

// 前缀积（取模，配合逆元）
vector<int> prod(n + 1, 1), inv(n + 1, 1);
for (int i = 1; i <= n; ++i) prod[i] = 1LL * prod[i - 1] * a[i] % MOD;
// [l, r] 积 = 1LL * prod[r] * inv[l - 1] % MOD（需要预处理逆元）

// 前缀 max / min（只能查询前缀，无法任意区间）
```

---

## 二、一维差分

### 差分构建 + 区间加 + 还原

```cpp
// 原数组 a[1..n]（1-indexed）
vector<int> a(n + 2);       // 多开防止 r+1 越界
for (int i = 1; i <= n; ++i) cin >> a[i];

// 1. 构建差分数组
vector<int> diff(n + 3, 0);
for (int i = 1; i <= n; ++i)
    diff[i] = a[i] - a[i - 1];

// 2. 区间加操作
auto range_add = [&](int l, int r, int val) {
    diff[l] += val;
    diff[r + 1] -= val;
};

// 3. 还原为原数组（对 diff 求前缀和）
for (int i = 1; i <= n; ++i) {
    diff[i] += diff[i - 1];
    a[i] = diff[i];  // a[i] 即为修改后的值
}
```

### 简化版本（初始全 0）

```cpp
vector<int> diff(n + 3, 0);

auto range_add = [&](int l, int r, int val) {
    diff[l] += val;
    diff[r + 1] -= val;
};

// 最后对 diff 求前缀和即得最终数组
for (int i = 1; i <= n; ++i) {
    diff[i] += diff[i - 1];
    cout << diff[i] << " \n"[i == n];
}
```

### 多次差分（区间加等差数列）

> 对区间 [l, r] 加上首项为 s、公差为 d 的等差数列：  
> a[l] += s, a[l+1] += s+d, ..., a[r] += s + (r-l)*d

用**二阶差分**实现：

```cpp
// diff1 维护增量，diff2 维护增量变化率
vector<int> diff1(n + 3, 0), diff2(n + 3, 0);

void range_add_ap(int l, int r, int s, int d) {
    diff2[l] += d;                     // 从 l 开始变化率 +d
    diff2[r + 1] -= d;                 // 在 r+1 结束
    diff1[l] += s;                     // 首项
    diff1[r + 1] -= s + d * (r - l);   // 末尾补偿
}

// 还原
for (int i = 1; i <= n; ++i) {
    diff2[i] += diff2[i - 1];
    diff1[i] += diff1[i - 1] + diff2[i];
    // 此时 diff1[i] 即为最终数组 a[i]
}
```

---

## 三、二维前缀和

### 构建与子矩阵查询

```
pref[i][j] = a[i][j]
           + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1]
```

```cpp
vector<vector<long long>> pref(n + 1, vector<long long>(m + 1, 0));

// 构建
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
        cin >> a[i][j];
        pref[i][j] = pref[i - 1][j] + pref[i][j - 1]
                   - pref[i - 1][j - 1] + a[i][j];
    }

// 查询子矩形 (x1,y1) → (x2,y2) 的和
auto rect_sum = [&](int x1, int y1, int x2, int y2) -> long long {
    return pref[x2][y2] - pref[x1 - 1][y2]
         - pref[x2][y1 - 1] + pref[x1 - 1][y1 - 1];
};
```

---

## 四、二维差分

### 矩形区域加

对子矩形 (x1,y1) → (x2,y2) 整体加 val：

```
diff[x1][y1]     += val
diff[x1][y2+1]   -= val
diff[x2+1][y1]   -= val
diff[x2+1][y2+1] += val
```

```cpp
const int N = 1005;
int diff[N][N];

void rect_add(int x1, int y1, int x2, int y2, int val) {
    diff[x1][y1]     += val;
    diff[x1][y2 + 1] -= val;
    diff[x2 + 1][y1] -= val;
    diff[x2 + 1][y2 + 1] += val;
}

// 还原（对二维前缀和）
void restore(int n, int m) {
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) {
            diff[i][j] += diff[i - 1][j] + diff[i][j - 1]
                        - diff[i - 1][j - 1];
        }
}
```

如果初始矩阵非零：

```cpp
// 构建二维差分（将 a 转化为 diff）
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
        diff[i][j] = a[i][j] - a[i - 1][j] - a[i][j - 1] + a[i - 1][j - 1];
    }

// 执行若干次 rect_add(...)

// 还原
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
        diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1];
        a[i][j] = diff[i][j];
    }
```

---

## 五、树上前缀和

### 根到节点的路径和

将树看作有根树，DFS 时累积根到当前节点的信息：

```cpp
// 邻接表建图
vector<vector<int>> g(N);       // 无权
// vector<vector<pair<int,int>>> g(N);  // 带权
vector<long long> sum(N);       // sum[u] = 根到 u 的路径权值和
vector<int> a(N);               // 点权

void dfs_sum(int u, int p) {
    sum[u] = sum[p] + a[u];            // 点权累加
    // sum[u] = sum[p] + w(u, p);      // 边权累加（根 sum[1] = 0）
    for (int v : g[u]) {
        if (v == p) continue;
        dfs_sum(v, u);
    }
}
```

### 任意两点路径查询（配合 LCA）

需要预处理 `up[u][j]`（倍增 LCA）或 `tin[u] / tout[u]`（Tarjan LCA）：

```cpp
// 倍增 LCA 预处理（邻接表版）
int dep[N], up[N][LOG];
void dfs_lca(int u, int p) {
    dep[u] = dep[p] + 1;
    up[u][0] = p;
    for (int j = 1; j < LOG; ++j)
        up[u][j] = up[up[u][j - 1]][j - 1];
    for (int v : g[u]) {
        if (v == p) continue;
        dfs_lca(v, u);
    }
}

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

// 路径 u→v 的点权和
// sum[u] + sum[v] - sum[lca] - sum[fa[lca]]
auto path_sum_point = [&](int u, int v) -> long long {
    int w = lca(u, v);
    return sum[u] + sum[v] - sum[w] - (up[w][0] ? sum[up[w][0]] : 0);
};

// 路径 u→v 的边权和（边权下放为子节点权值）
// sum[u] + sum[v] - 2 * sum[lca]
auto path_sum_edge = [&](int u, int v) -> long long {
    int w = lca(u, v);
    return sum[u] + sum[v] - 2 * sum[w];
};
```

### 树上前缀异或（点权）

```cpp
// 路径 u→v 的点权异或 = pxor[u] ^ pxor[v] ^ a[lca]
// 异或的性质：x ^ x = 0，所以除了 lca 外其他公共部分抵消

vector<int> pxor(N);
void dfs_xor(int u, int p) {
    pxor[u] = pxor[p] ^ a[u];
    for (int v : g[u]) {
        if (v == p) continue;
        dfs_xor(v, u);
    }
}
auto path_xor = [&](int u, int v) -> int {
    int w = lca(u, v);
    return pxor[u] ^ pxor[v] ^ a[w];
};
```

---

## 六、树上差分

树上差分用于快速处理**路径加**操作，最后 DFS 一次还原。

| 类型 | 操作 | 公式 |
|------|------|------|
| **点差分** | 路径 u→v 上每个点 +val | `diff[u] += val; diff[v] += val; diff[lca] -= val; diff[fa[lca]] -= val;` |
| **边差分** | 路径 u→v 上每条边 +val | `diff[u] += val; diff[v] += val; diff[lca] -= 2 * val;` |

### 前置：邻接表 + LCA（倍增）

```cpp
const int N = 2e5 + 5, LOG = 18;
vector<vector<int>> g(N);   // 无权树
// vector<vector<pair<int,int>>> g(N);  // 带权树

int dep[N], up[N][LOG];
void dfs_lca(int u, int p) {
    dep[u] = dep[p] + 1;
    up[u][0] = p;
    for (int j = 1; j < LOG; ++j)
        up[u][j] = up[up[u][j - 1]][j - 1];
    for (int v : g[u]) {
        if (v == p) continue;
        dfs_lca(v, u);
    }
}

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

### 点差分

> m 次操作，每次给路径 u→v 上的**每个节点** +val，求最终每个点的值。

```cpp
int diff[N];

void path_add_point(int u, int v, int val) {
    int w = lca(u, v);
    diff[u] += val;
    diff[v] += val;
    diff[w] -= val;
    if (up[w][0]) diff[up[w][0]] -= val;  // fa[w] 存在时减
}

// DFS 还原（子树累加）
void dfs_restore(int u, int p) {
    for (int v : g[u]) {
        if (v == p) continue;
        dfs_restore(v, u);
        diff[u] += diff[v];
    }
}
```

**公式推导：**

```
          ↑ diff[u] +val         ←── diff[w] -val
         / \
        /   \
       /     \
      w       \
     / \       \
    /   \       ↑ diff[v] +val
   /     \
  ────────
  ↑ diff[fa[w]] -val
```

### 边差分

> m 次操作，每次给路径 u→v 上的**每条边** +val，求最终每条边的值。

将"边权"下放到**子节点**（`diff[u]` 表示 u → fa[u] 这条边的值）：

```cpp
int diff[N];

void path_add_edge(int u, int v, int val) {
    int w = lca(u, v);
    diff[u] += val;
    diff[v] += val;
    diff[w] -= 2 * val;     // w 处减两次，子树加和时恰好抵消
}

// DFS 还原
void dfs_restore(int u, int p) {
    for (int v : g[u]) {
        if (v == p) continue;
        dfs_restore(v, u);
        diff[u] += diff[v];
    }
    // diff[u] 即为边 (u → p) 的最终权值（根节点无意义）
}
```

### 点差分 vs 边差分

| 类型 | 标记方式 | 还原后含义 |
|------|----------|-----------|
| 点差分 | `u += v, v += v, w -= v, fa[w] -= v` | `diff[u]` = 节点 u 的值 |
| 边差分 | `u += v, v += v, w -= 2*v` | `diff[u]` = 边 (u→fa[u]) 的值 |

---

## 经典应用场景

| 问题 | 解法 |
|------|------|
| 静态区间和查询 | 一维前缀和 |
| 区间加 + 单点查 | 一维差分 |
| 区间加 + 区间查 | 差分 + 树状数组 |
| 静态子矩阵和查询 | 二维前缀和 |
| 子矩形加 + 单点查 | 二维差分 |
| 子矩形加 + 子矩形查 | 差分 + 二维树状数组 |
| 静态树上路径和查询 | 树上前缀和 + LCA |
| 路径加 + 单点查（树上） | 树上点差分 |
| 路径加 + 边权查（树上） | 树上边差分 |
| 覆盖次数 / 染色次数 | 树上差分（统计路径经过次数） |

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 数组大小 | 差分/前缀和多开 2 个，防止 `r+1` 越界 |
| 下标从 1 开始 | 差分公式依赖 `r+1`，前缀和公式依赖 `l-1`，必须 1-indexed |
| 前缀和类型 | 和用 `long long`，异或用 `int`，积用取模 |
| 多维还原 | 从外层到内层依次前缀和，注意符号 |
| 树上差分要 LCA | 需要预处理倍增 LCA 或 Tarjan LCA |
| 边差分下放 | 边权下放为子节点权值，根节点无对应边 |
| 多次还原 | 每次还原后 diff 会被前缀和覆盖，如需多次操作应备份 |
| 树上前缀和与差分 | 前缀和查询时用 LCA，差分修改时用 LCA，两者互补 |

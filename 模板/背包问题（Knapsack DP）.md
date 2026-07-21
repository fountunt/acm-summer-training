# 背包问题 (Knapsack DP)

## 概念

给定一个容量为 `W` 的背包和若干物品（每个有体积 `v[i]` 和价值 `val[i]`），选择物品使总价值最大。

| 类型 | 物品限制 | 容量枚举顺序 |
|------|----------|-------------|
| 01背包 | 每件最多选 1 次 | 外层物品，**内层倒序** |
| 完全背包 | 每件无限取 | 外层物品，**内层正序** |
| 多重背包 | 每件有限 `cnt[i]` 次 | 二进制拆分后按 01背包 |
| 分组背包 | 每组最多选 1 个 | 外层组，内层容量倒序，最内层组内枚举 |

**核心思想**：一维滚动数组，`dp[w]` 表示容量 `w` 时的最大价值。倒序保证每种物品只用一次（不覆盖上一轮状态），正序允许重复取。

---

## 一、01背包（基础）

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, W;
    cin >> n >> W;
    vector<int> v(n), val(n);
    for (int i = 0; i < n; ++i)
        cin >> v[i] >> val[i];

    vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; ++i)              // 枚举物品
        for (int w = W; w >= v[i]; --w)       // 容量倒序
            dp[w] = max(dp[w], dp[w - v[i]] + val[i]);

    cout << dp[W] << '\n';
    return 0;
}
```

### 恰好装满

初始化为 `-INF`，`dp[0] = 0`，最后检查 `dp[W] > 0`。

```cpp
const int INF = 1e9;
vector<int> dp(W + 1, -INF);
dp[0] = 0;
for (int i = 0; i < n; ++i)
    for (int w = W; w >= v[i]; --w)
        dp[w] = max(dp[w], dp[w - v[i]] + val[i]);
// dp[W] < 0 表示无法恰好装满
```

### 方案数

```cpp
vector<int> dp(W + 1, 0);
dp[0] = 1;
for (int i = 0; i < n; ++i)
    for (int w = W; w >= v[i]; --w)
        dp[w] += dp[w - v[i]];               // 完全背包时改为正序
```

### 最小字典序方案（记录路径）

物品倒序 DP，正序回溯时优先选编号小的。

```cpp
vector<int> dp(W + 1, 0);
vector<vector<bool>> chosen(n, vector<bool>(W + 1, false));

for (int i = n - 1; i >= 0; --i) {           // 物品倒序
    for (int w = W; w >= v[i]; --w) {
        if (dp[w - v[i]] + val[i] > dp[w]) {
            dp[w] = dp[w - v[i]] + val[i];
            chosen[i][w] = true;
        }
    }
}

// 正序输出方案
for (int i = 0, w = W; i < n; ++i) {
    if (chosen[i][w]) {
        cout << i + 1 << ' ';
        w -= v[i];
    }
}
```

---

## 二、完全背包

```cpp
vector<int> dp(W + 1, 0);
for (int i = 0; i < n; ++i)
    for (int w = v[i]; w <= W; ++w)          // 容量正序
        dp[w] = max(dp[w], dp[w - v[i]] + val[i]);
```

---

## 三、多重背包

### 二进制拆分优化（推荐）

将 `cnt[i]` 拆成 `1, 2, 4, ..., rem`，转化为 `O(W * Σlog(cnt[i]))` 的 01背包。

```cpp
struct Item { int v, val; };
vector<Item> items;

// 二进制拆分
for (int i = 0; i < n; ++i) {
    int v_i, val_i, cnt;
    cin >> v_i >> val_i >> cnt;
    for (int k = 1; k <= cnt; k <<= 1) {
        items.push_back({k * v_i, k * val_i});
        cnt -= k;
    }
    if (cnt)
        items.push_back({cnt * v_i, cnt * val_i});
}

// 01背包
vector<int> dp(W + 1, 0);
for (auto [v, val] : items)
    for (int w = W; w >= v; --w)
        dp[w] = max(dp[w], dp[w - v] + val);
```

### 单调队列优化（严格 O(NW)）

按容量模 `v[i]` 的余数分组，单调队列维护滑动窗口最大值。当 `W * Σcnt[i]` 接近极限时使用。

```cpp
vector<int> dp(W + 1, 0);
for (int i = 0; i < n; ++i) {
    int v_i, val_i, cnt;
    cin >> v_i >> val_i >> cnt;
    auto pre = dp;
    for (int r = 0; r < v_i; ++r) {                        // 余数分组
        deque<pair<int, int>> dq;                          // (下标, 值)
        for (int k = 0, w = r; w <= W; ++w, ++k) {
            int cur = pre[w] - k * val_i;
            while (!dq.empty() && dq.back().second <= cur)
                dq.pop_back();
            dq.push_back({k, cur});
            while (!dq.empty() && dq.front().first < k - cnt)
                dq.pop_front();
            dp[w] = dq.front().second + k * val_i;
        }
    }
}
```

---

## 四、分组背包

每组物品最多选一个。最内层遍历组内物品，容量倒序。

```cpp
vector<vector<pair<int, int>>> groups;       // groups[g] = { (v, val), ... }
groups.resize(m);                            // m 组
for (int g = 0; g < m; ++g) {
    int sz; cin >> sz;
    groups[g].resize(sz);
    for (auto &[v, val] : groups[g])
        cin >> v >> val;
}

vector<int> dp(W + 1, 0);
for (auto &items : groups) {
    for (int w = W; w >= 0; --w)             // 容量倒序
        for (auto [v, val] : items)
            if (w >= v)
                dp[w] = max(dp[w], dp[w - v] + val);
}
```

---

## 五、二维费用背包

```cpp
// dp[c1][c2]：第一维费用倒序，第二维费用倒序
vector<vector<int>> dp(C1 + 1, vector<int>(C2 + 1, 0));
for (int i = 0; i < n; ++i) {
    int c1, c2, val;
    cin >> c1 >> c2 >> val;
    for (int j = C1; j >= c1; --j)
        for (int k = C2; k >= c2; --k)
            dp[j][k] = max(dp[j][k], dp[j - c1][k - c2] + val);
}
```

---

## 六、依赖背包（树形 + 分组背包）

选父节点才能选子节点。将子树看作分组背包，根节点作为组内额外物品。

```cpp
vector<vector<int>> g(n);                    // 邻接表
vector<int> v(n), val(n);
vector<int> dp(W + 1, -INF);

function<void(int)> dfs = [&](int u) {
    // 必须先选 u：初始化 dp 为父节点占用的容量
    dp[u][v[u]] = val[u];                    // 二维写法：dp[u][w] 表示选 u 子树容量 w
    for (int c : g[u]) {
        dfs(c);
        // 分组背包合并子树
        for (int w = W; w >= 0; --w)
            for (int k = 0; k <= w; ++k)
                dp[u][w] = max(dp[u][w], dp[u][w - k] + dp[c][k]);
    }
};
```

实际刷题中常用一维+dfs 序或 **金明的预算方案** 式简化（附件不超过 2 个时直接枚举四种主附件组合）。

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 枚举顺序 | **01 背包倒序，完全背包正序** — 这是最常见 bug |
| 恰好装满 | 初始化为 `-INF`，`dp[0] = 0` |
| 最大/最小值 | 求最大初始化为 `0` 或 `-INF`；求最小初始化为 `INF` |
| 方案数取模 | `dp[w] = (dp[w] + dp[w - v]) % MOD` |
| 数据范围 | `n ≤ 100, W ≤ 1e5` 用基础 01/完全；`W ≥ 1e4` 注意 `long long` |
| 多重背包 | `Σcnt[i] > 1e4` 时务必用二进制拆分 |
| 二维费用 | 容量和重量都需倒序枚举 |
| 滚动数组 | 一维 `dp[w]` 比二维省内存，但注意状态覆盖问题 |

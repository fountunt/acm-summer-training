# DP 问题模板 (Dynamic Programming Templates)

> 替换原「背包问题」文件，覆盖竞赛中 10+ 类常见 DP。

---

## 一、背包类 DP（简要速查）

| 类型 | 枚举顺序 | 要点 |
|------|----------|------|
| 01背包 | 物品→容量**倒序** | 每件最多 1 次 |
| 完全背包 | 物品→容量**正序** | 每件无限次 |
| 多重背包 | 二进制拆分 → 01背包 | `Σcnt > 1e4` 必拆 |
| 分组背包 | 组→容量倒序→组内物品 | 每组最多 1 个 |
| 二维费用 | `dp[c1][c2]` 双倒序 | 两种限制 |

```cpp
// 01背包
for (int i = 0; i < n; ++i)
    for (int w = W; w >= v[i]; --w)
        dp[w] = max(dp[w], dp[w - v[i]] + val[i]);

// 完全背包
for (int i = 0; i < n; ++i)
    for (int w = v[i]; w <= W; ++w)
        dp[w] = max(dp[w], dp[w - v[i]] + val[i]);

// 多重背包（二进制拆分）
for (int i = 0, k = 1; i < n; ++i) {
    int v_i, val_i, cnt; cin >> v_i >> val_i >> cnt;
    for (k = 1; k <= cnt; k <<= 1) {
        items.push_back({k * v_i, k * val_i});
        cnt -= k;
    }
    if (cnt) items.push_back({cnt * v_i, cnt * val_i});
}

// 分组背包
for (auto &g : groups)       // 组
    for (int w = W; w >= 0; --w)
        for (auto [v, val] : g)
            if (w >= v) dp[w] = max(dp[w], dp[w - v] + val);
```

---

## 二、线性 DP

### 1. LIS（最长上升子序列, O(n log n)）

```cpp
int lengthOfLIS(vector<int> &a) {
    vector<int> lis;
    for (int x : a) {
        auto it = lower_bound(lis.begin(), lis.end(), x);
        if (it == lis.end()) lis.push_back(x);
        else *it = x;
    }
    return lis.size();
}
// 严格下降用 upper_bound + greater；非降用 upper_bound
```

**路径还原**（记录每个元素结尾的 LIS 前驱）：

```cpp
vector<int> restoreLIS(vector<int> &a) {
    int n = a.size();
    vector<int> lis, idx(n), parent(n, -1);
    for (int i = 0; i < n; ++i) {
        auto it = lower_bound(lis.begin(), lis.end(), a[i]);
        int pos = it - lis.begin();
        if (it == lis.end()) lis.push_back(a[i]);
        else *it = a[i];
        idx[pos] = i;
        if (pos > 0) parent[i] = idx[pos - 1];
    }
    vector<int> path;
    for (int cur = idx[lis.size() - 1]; cur != -1; cur = parent[cur])
        path.push_back(a[cur]);
    reverse(path.begin(), path.end());
    return path;
}
```

### 2. LCS（最长公共子序列, O(nm) → O(nm/64) bitset 优化）

```cpp
vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
        if (s[i - 1] == t[j - 1])
            dp[i][j] = dp[i - 1][j - 1] + 1;
        else
            dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
    }
}
// 答案: dp[n][m]
```

---

## 三、区间 DP

```cpp
for (int len = 1; len <= n; ++len)           // 区间长度
    for (int l = 0; l + len - 1 < n; ++l) { // 左端点
        int r = l + len - 1;
        // 初始化：if (len == 1) 直接设值
        for (int k = l; k < r; ++k)           // 分割点
            dp[l][r] = max(dp[l][r], dp[l][k] + dp[k + 1][r] + cost(l, r));
    }
```

**常用技巧**（四边形不等式优化 → 减少 k 枚举范围）：

```cpp
// 维护 opt[l][r] 为最优分割点，枚举范围 [opt[l][r-1], opt[l+1][r]]
// 要求 cost 满足四边形不等式 + 单调性（石子合并等问题）
for (int len = 2; len <= n; ++len)
    for (int l = 0, r = l + len - 1; r < n; ++l, ++r) {
        dp[l][r] = INF;
        for (int k = opt[l][r - 1]; k <= opt[l + 1][r]; ++k)
            if (dp[l][k] + dp[k + 1][r] + cost(l, r) < dp[l][r])
                dp[l][r] = dp[l][k] + dp[k + 1][r] + cost(l, r), opt[l][r] = k;
    }
```

### 经典：石子合并（环形 → 2n 链化）

```cpp
int n; cin >> n;
vector<int> a(2 * n);
for (int i = 0; i < n; ++i) cin >> a[i], a[i + n] = a[i];
vector<int> ps(2 * n + 1, 0);
for (int i = 0; i < 2 * n; ++i) ps[i + 1] = ps[i] + a[i];

vector dp(2 * n, vector<int>(2 * n, 0));
vector dm(2 * n, vector<int>(2 * n, INF));
for (int i = 0; i < 2 * n; ++i) dm[i][i] = 0;

for (int len = 2; len <= n; ++len) {
    for (int l = 0; l + len - 1 < 2 * n; ++l) {
        int r = l + len - 1;
        for (int k = l; k < r; ++k) {
            int sum = ps[r + 1] - ps[l];
            dp[l][r] = max(dp[l][r], dp[l][k] + dp[k + 1][r] + sum);
            dm[l][r] = min(dm[l][r], dm[l][k] + dm[k + 1][r] + sum);
        }
    }
}
int ans_max = 0, ans_min = INF;
for (int i = 0; i < n; ++i)
    ans_max = max(ans_max, dp[i][i + n - 1]),
    ans_min = min(ans_min, dm[i][i + n - 1]);
cout << ans_min << '\n' << ans_max << '\n';
```

---

## 四、树形 DP

### 基础：树上最大独立集（打家劫舍）

```cpp
vector<int> g[N];
int dp[N][2];                           // 0:不选, 1:选

void dfs(int u, int p) {
    dp[u][0] = 0; dp[u][1] = val[u];
    for (int v : g[u]) if (v != p) {
        dfs(v, u);
        dp[u][0] += max(dp[v][0], dp[v][1]);
        dp[u][1] += dp[v][0];
    }
}
// 答案: max(dp[1][0], dp[1][1])
```

### 树上背包（依赖背包）

```cpp
// dp[u][j] = u 子树选 j 个节点的最大价值
vector<int> dp(N, -INF), tmp;
vector<int> sz(N, 0);

function<void(int, int)> dfs = [&](int u, int p) {
    dp[u] = {0};                         // 不选任何节点
    for (int v : g[u]) if (v != p) {
        dfs(v, u);
        tmp.assign(dp[u].size() + dp[v].size() - 1, -INF);
        for (int i = 0; i < dp[u].size(); ++i)
            for (int j = 0; j < dp[v].size(); ++j)
                tmp[i + j] = max(tmp[i + j], dp[u][i] + dp[v][j]);
        dp[u] = move(tmp);
    }
    // 强制选 u（若需要）：dp[u].insert(dp[u].begin(), -INF);
    // for (auto &x : dp[u]) x += val[u]; 然后 dp[u][0] = -INF;
};
```

### 换根 DP（reroot）

```cpp
// 计算每个节点作为根时的答案（例如到所有点距离和）
vector<int> g[N];
int sz[N], dp[N], ans[N], n;

void dfs1(int u, int p) {               // 第一次：计算子树大小和初始答案
    sz[u] = 1;
    for (int v : g[u]) if (v != p) {
        dfs1(v, u);
        sz[u] += sz[v];
        dp[u] += dp[v] + sz[v];         // dp[u] = u 子树内距离和
    }
}

void dfs2(int u, int p) {               // 第二次：换根
    ans[u] = dp[u];
    for (int v : g[u]) if (v != p) {
        // 将根从 u 换到 v
        int pu = dp[u], pv = dp[v];
        int su = sz[u], sv = sz[v];

        dp[u] -= dp[v] + sz[v];
        sz[u] = n - sz[v];
        dp[v] += dp[u] + sz[u];

        dfs2(v, u);

        // 恢复
        dp[u] = pu, dp[v] = pv;
        sz[u] = su, sz[v] = sv;
    }
}
// 调用: dfs1(1, 0); dfs2(1, 0); ans[i] 即为答案
```

---

## 五、数位 DP

```cpp
// 统计 [0, R] 中满足条件的个数（典型：不含 4 和连续 62）
int dp[20][2][10];                      // pos, tight, last
int a[20];                              // 数字的各位

int dfs(int pos, int tight, int last) {
    if (pos < 0) return 1;
    int &res = dp[pos][tight][last];
    if (~res) return res;
    res = 0;
    int up = tight ? a[pos] : 9;
    for (int d = 0; d <= up; ++d) {
        if (d == 4) continue;
        if (last == 6 && d == 2) continue;
        res += dfs(pos - 1, tight && d == up, d);
    }
    return res;
}

int solve(int x) {
    memset(dp, -1, sizeof(dp));
    int len = 0;
    while (x) a[len++] = x % 10, x /= 10;
    return dfs(len - 1, 1, 0);
}
// 调用: solve(R) - solve(L - 1)
```

---

## 六、状压 DP（Bitmask DP）

```cpp
// 例：最短哈密顿路径（TSP），n ≤ 20
// dp[mask][i] = 已访问 mask，最后在 i 的最小距离
vector<vector<int>> dp(1 << n, vector<int>(n, INF));
dp[1][0] = 0;                           // 从 0 出发
for (int mask = 1; mask < (1 << n); ++mask) {
    for (int u = 0; u < n; ++u) if (mask >> u & 1) {
        if (dp[mask][u] == INF) continue;
        for (int v = 0; v < n; ++v) if (!(mask >> v & 1)) {
            int nmask = mask | (1 << v);
            dp[nmask][v] = min(dp[nmask][v], dp[mask][u] + dist[u][v]);
        }
    }
}
// 答案: dp[(1<<n)-1][n-1]（回到 n-1）
```

**枚举子集**（SOS DP 核心操作）：

```cpp
for (int sub = mask; sub; sub = (sub - 1) & mask) {
    // sub 是 mask 的非空子集
}
// 总复杂度 O(3^n)
```

**SOS DP（子集求和/超集求和）**：

```cpp
// 子集求和：dp[mask] = Σ f[sub] (sub ⊆ mask)
vector<int> dp(1 << n);
for (int i = 0; i < (1 << n); ++i) dp[i] = f[i];
for (int b = 0; b < n; ++b)
    for (int mask = 0; mask < (1 << n); ++mask)
        if (mask >> b & 1)
            dp[mask] += dp[mask ^ (1 << b)];

// 超集求和：只需改枚举方向
// for (int mask = 0; mask < (1<<n); ++mask)
//     if (!(mask >> b & 1))
//         dp[mask] += dp[mask | (1 << b)];
```

---

## 七、概率 / 期望 DP

```cpp
// 例：DAG 上从 1 到 n 的期望步数（等概率选择出边）
vector<double> dp(n + 1, 0);           // dp[u] = u→n 的期望步数
vector<int> deg(n + 1);
// 反向建图，拓扑序 DP
for (int u = n; u >= 1; --u) {
    if (u == n) continue;
    for (int v : rev_g[u])
        dp[u] += (dp[v] + 1.0) / deg[u];
}
```

**高斯消元解期望**（图中带环时）：

```cpp
// dp[u] = 1 + Σ p[v][u] * dp[v]（转移有环时列方程）
// 对每个未知数 dp[i] 列方程，高斯消元 O(n^3)
```

---

## 八、计数 DP

### 整数划分（n 拆成若干个正整数之和）

```cpp
// dp[j] = 组成 j 的方案数，完全背包思想
vector<long long> dp(N + 1, 0);
dp[0] = 1;
for (int i = 1; i <= N; ++i)            // 枚举数字
    for (int j = i; j <= N; ++j)
        dp[j] = (dp[j] + dp[j - i]) % MOD;
```

### 卡特兰数（出栈序列、括号匹配、二叉树计数）

```cpp
// C[n] = Σ C[k] * C[n-1-k], C[0] = 1
vector<long long> C(N + 1, 0);
C[0] = 1;
for (int i = 1; i <= N; ++i)
    for (int k = 0; k < i; ++k)
        C[i] = (C[i] + C[k] * C[i - 1 - k]) % MOD;
```

---

## 九、DP 优化技巧

### 1. 单调队列优化

适用：`dp[i] = max/min{ dp[j] + cost(j, i) }`，且 cost 满足滑窗性质。

```cpp
// 例：dp[i] = max(dp[j] + a[i]), j ∈ [i - k, i - 1]
deque<int> dq;
for (int i = 0; i < n; ++i) {
    while (!dq.empty() && dq.front() < i - k) dq.pop_front();
    if (!dq.empty())
        dp[i] = dp[dq.front()] + a[i];  // 根据具体转移
    while (!dq.empty() && dp[dq.back()] <= dp[i])
        dq.pop_back();
    dq.push_back(i);
}
```

### 2. 斜率优化（Convex Hull Trick, 下凸壳维护最小值）

适用：`dp[i] = min{ dp[j] + (a[i] - a[j])² }` 等可化为 `y = kx + b` 形式。

```cpp
// 维护下凸壳：k 单调递增，x 查询单调
struct Line {
    long long k, b;
    long long operator()(long long x) { return k * x + b; }
};

double intersect(Line l1, Line l2) {
    return 1.0 * (l2.b - l1.b) / (l1.k - l2.k);
}

vector<Line> hull;
int ptr = 0;                            // 指针

void add(Line l) {                      // 按斜率递增插入
    while (hull.size() >= 2) {
        int m = hull.size();
        if (intersect(hull[m - 2], hull[m - 1]) >= intersect(hull[m - 1], l))
            hull.pop_back();
        else break;
    }
    hull.push_back(l);
}

long long query(long long x) {          // 按 x 递增查询
    while (ptr + 1 < hull.size() && hull[ptr](x) >= hull[ptr + 1](x))
        ++ptr;
    return hull[ptr](x);
}
```

### 3. 线性递推（BM / 多项式模幂 / Bostan–Mori）

适用：k 阶线性递推，`n ≤ 1e18`。详见 `线性递推（Linear Recurrence）.md`。

```cpp
using Mat = vector<vector<long long>>;
const long long MOD = 1e9 + 7;

Mat mul(const Mat &a, const Mat &b) {
    int n = a.size(), m = b[0].size(), p = a[0].size();
    Mat c(n, vector<long long>(m, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < p; ++k) if (a[i][k])
            for (int j = 0; j < m; ++j)
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
    return c;
}

Mat qpow(Mat a, long long b) {
    int n = a.size();
    Mat res(n, vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) res[i][i] = 1;
    for (; b; b >>= 1) {
        if (b & 1) res = mul(res, a);
        a = mul(a, a);
    }
    return res;
}

// 例：斐波那契 F[n] = F[n-1] + F[n-2]
// [F[2], F[1]] = [1, 1]
// [F[n], F[n-1]] = [1,1] * [1,1; 1,0]^{n-2}
// Mat base = {{1, 1}, {1, 0}};
// Mat fn = qpow(base, n - 2);
// ans = (fn[0][0] + fn[0][1]) % MOD;
```

---

## 附录：DP 类型速查表

| 类型 | 复杂度 | 适用场景 | 注意 |
|------|--------|----------|------|
| 线性 DP | O(n) ~ O(n²) | 序列上递推 | 注意初始化与边界 |
| 区间 DP | O(n³) | 合并、回文、括号匹配 | 四边形不等式可降 O(n²) |
| 背包 DP | O(nW) | 资源分配 | 枚举顺序是灵魂 |
| 树形 DP | O(n) ~ O(nk²) | 树上问题 | ➕ 换根可求全源 |
| 数位 DP | O(len × state) | [L,R] 内计数 | 记忆化搜索更直观 |
| 状压 DP | O(2ⁿ × n²) | 小 n（≤20）的全排列/集合 | 子集枚举 O(3ⁿ) |
| 概率 DP | O(n + m) | 期望步数、胜率 | 有环时需高斯消元 |
| 计数 DP | O(n²) | 划分数、排列数 | 通常要取模 |

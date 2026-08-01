# 搜索算法 (DFS / BFS / 回溯)

## 目录

1. [DFS（深度优先搜索）](#一dfs深度优先搜索)
2. [BFS（广度优先搜索）](#二bfs广度优先搜索)
3. [回溯（Backtracking）](#三回溯backtracking)
4. [双向 BFS](#四双向-bfs)
5. [IDDFS（迭代加深搜索）](#五iddfs迭代加深搜索)
6. [Flood Fill](#六flood-fill)
7. [状态压缩 BFS](#七状态压缩-bfs)
8. [A* 搜索](#八a-搜索)

---

## 一、DFS（深度优先搜索）

### 1. 图的 DFS（链式前向星）

```cpp
#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
vector<int> g[N];
bool vis[N];

void dfs(int u) {
    vis[u] = true;
    // 访问 u
    for (int v : g[u]) {
        if (!vis[v])
            dfs(v);
    }
}
```

### 2. 网格 DFS（四方向 / 八方向）

```cpp
const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
// 八方向 const int dirs[8][2] = {{0,1},{1,0},{0,-1},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}};
int n, m;
vector<string> grid;

bool inBound(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

void dfs(int x, int y) {
    if (!inBound(x, y) || grid[x][y] != '1') return;
    grid[x][y] = '0';  // 标记已访问（直接修改原图，省去 vis 数组）
    for (auto [dx, dy] : dirs)
        dfs(x + dx, y + dy);
}
```

### 3. DFS 求连通块数量 / 大小

```cpp
int cnt = 0;
function<void(int, int)> dfs = [&](int x, int y) {
    if (!inBound(x, y) || grid[x][y] != '1') return;
    grid[x][y] = '0';
    cnt++;
    for (auto [dx, dy] : dirs)
        dfs(x + dx, y + dy);
};

for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
        if (grid[i][j] == '1') {
            cnt = 0;
            dfs(i, j);
            // cnt 即为当前连通块大小
        }
```

### 4. 拓扑排序（DFS 后序）

```cpp
vector<int> g[N];
bool vis[N];
vector<int> order;  // 拓扑序

void dfs(int u) {
    vis[u] = true;
    for (int v : g[u])
        if (!vis[v])
            dfs(v);
    order.push_back(u);  // 后序加入
}

// 主函数：对所有点 dfs
for (int i = 1; i <= n; ++i)
    if (!vis[i]) dfs(i);
reverse(order.begin(), order.end());
// order 即为拓扑序（若有环则不可用）
```

### 5. 检测环（有向图）

```cpp
// 0 = 未访问, 1 = 在当前栈中, 2 = 已安全退出
int state[N];
bool hasCycle = false;

void dfs(int u) {
    state[u] = 1;
    for (int v : g[u]) {
        if (state[v] == 1) hasCycle = true;  // 发现环
        else if (state[v] == 0) dfs(v);
    }
    state[u] = 2;
}
```

---

## 二、BFS（广度优先搜索）

### 1. 图的 BFS

```cpp
const int N = 1e5 + 5;
vector<int> g[N];
int dist[N];  // -1 表示未访问

void bfs(int s) {
    queue<int> q;
    fill(dist, dist + N, -1);
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
```

### 2. 网格 BFS（最短路）

```cpp
const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
int n, m;
vector<string> grid;
int dist[1005][1005];

int bfs(int sx, int sy, int ex, int ey) {
    memset(dist, -1, sizeof(dist));
    queue<pair<int, int>> q;
    dist[sx][sy] = 0;
    q.emplace(sx, sy);

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == ex && y == ey) return dist[x][y];
        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (inBound(nx, ny) && grid[nx][ny] != '#' && dist[nx][ny] == -1) {
                dist[nx][ny] = dist[x][y] + 1;
                q.emplace(nx, ny);
            }
        }
    }
    return -1;  // 不可达
}
```

### 3. 多源 BFS

同时从多个起点出发，常用于"感染"、"距离最近"类问题。

```cpp
const int INF = 1e9;
int dist[1005][1005];

void multiSourceBFS(vector<pair<int, int>>& sources) {
    queue<pair<int, int>> q;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            dist[i][j] = INF;

    for (auto [x, y] : sources) {
        dist[x][y] = 0;
        q.emplace(x, y);
    }

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (inBound(nx, ny) && dist[nx][ny] == INF) {
                dist[nx][ny] = dist[x][y] + 1;
                q.emplace(nx, ny);
            }
        }
    }
}
```

### 4. 0-1 BFS（边权为 0 或 1）

用 `deque`，0 权边 `push_front`，1 权边 `push_back`。

```cpp
vector<pair<int, int>> g[N];  // (to, weight)  weight ∈ {0, 1}

void bfs01(int s) {
    deque<int> dq;
    vector<int> dist(N, INF);
    dist[s] = 0;
    dq.push_front(s);

    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        for (auto [v, w] : g[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
}
```

### 5. BFS 打印路径

```cpp
int pre[1005][1005];  // 用方向编码存储前驱：0=右,1=下,2=左,3=上
char dirChar[] = {'R', 'D', 'L', 'U'};

// BFS 过程中记录 pre[nx][ny] = dirIdx

// 回溯路径
string path;
int x = ex, y = ey;
while (!(x == sx && y == sy)) {
    int d = pre[x][y];
    path += dirChar[d];
    x -= dirs[d][0];
    y -= dirs[d][1];
}
reverse(path.begin(), path.end());
// path 即为从起点到终点的路径
```

---

## 三、回溯（Backtracking）

### 1. 全排列

```cpp
int n;
vector<int> a;
vector<bool> used;
vector<int> perm;

void dfs() {
    if (perm.size() == n) {
        for (int x : perm) cout << x << ' ';
        cout << '\n';
        return;
    }
    for (int i = 0; i < n; ++i) {
        if (used[i]) continue;
        used[i] = true;
        perm.push_back(a[i]);
        dfs();
        perm.pop_back();
        used[i] = false;
    }
}
```

### 2. 全排列（去重，含重复元素）

```cpp
vector<int> a;
vector<bool> used;
vector<int> perm;

void dfs() {
    if (perm.size() == a.size()) {
        // 处理 perm
        return;
    }
    for (int i = 0; i < a.size(); ++i) {
        if (used[i]) continue;
        // 去重：同一层相同值只用第一个
        if (i > 0 && a[i] == a[i - 1] && !used[i - 1]) continue;
        used[i] = true;
        perm.push_back(a[i]);
        dfs();
        perm.pop_back();
        used[i] = false;
    }
}

// 主函数先 sort(a.begin(), a.end());
```

### 3. 组合（C(n, k)）

```cpp
int n, k;
vector<int> comb;

void dfs(int start) {
    if (comb.size() == k) {
        for (int x : comb) cout << x << ' ';
        cout << '\n';
        return;
    }
    // 剪枝：剩余数量不足时提前返回
    for (int i = start; i <= n - (k - comb.size()) + 1; ++i) {
        comb.push_back(i);
        dfs(i + 1);
        comb.pop_back();
    }
}
// dfs(1);
```

### 4. 子集枚举

```cpp
vector<int> a;
vector<int> subset;

void dfs(int idx) {
    // 每个节点都输出（不只是叶子）
    for (int x : subset) cout << x << ' ';
    cout << '\n';

    for (int i = idx; i < a.size(); ++i) {
        subset.push_back(a[i]);
        dfs(i + 1);
        subset.pop_back();
    }
}
// dfs(0);

// 二进制枚举法（n 较小，n ≤ 20）
for (int mask = 0; mask < (1 << n); ++mask) {
    for (int i = 0; i < n; ++i)
        if (mask >> i & 1) cout << a[i] << ' ';
    cout << '\n';
}
```

### 5. N 皇后

```cpp
int n, ans = 0;
bool col[20], diag1[40], diag2[40];  // 列、主对角线、副对角线

void dfs(int r) {
    if (r == n) { ans++; return; }
    for (int c = 0; c < n; ++c) {
        int d1 = r - c + n;      // 主对角线编号（r - c + n 防负）
        int d2 = r + c;          // 副对角线编号
        if (col[c] || diag1[d1] || diag2[d2]) continue;
        col[c] = diag1[d1] = diag2[d2] = true;
        dfs(r + 1);
        col[c] = diag1[d1] = diag2[d2] = false;
    }
}
```

### 6. 数独（Sudoku）

```cpp
int board[9][9];
bool row[9][10], col[9][10], block[3][3][10];

bool dfs(int pos) {
    if (pos == 81) return true;
    int r = pos / 9, c = pos % 9;
    if (board[r][c]) return dfs(pos + 1);

    for (int v = 1; v <= 9; ++v) {
        if (row[r][v] || col[c][v] || block[r / 3][c / 3][v]) continue;
        board[r][c] = v;
        row[r][v] = col[c][v] = block[r / 3][c / 3][v] = true;
        if (dfs(pos + 1)) return true;
        row[r][v] = col[c][v] = block[r / 3][c / 3][v] = false;
        board[r][c] = 0;
    }
    return false;
}
```

### 7. 子集和 / 剪枝经典

```cpp
vector<int> a;
int target;
bool found = false;

void dfs(int idx, int sum) {
    if (found) return;
    if (sum == target) { found = true; return; }
    if (idx == a.size() || sum > target) return;  // 可行性剪枝

    // 选
    dfs(idx + 1, sum + a[idx]);
    // 不选
    dfs(idx + 1, sum);
}
```

**常用剪枝策略：**

| 剪枝 | 说明 |
|------|------|
| 可行性剪枝 | 当前状态一定无解时提前返回 |
| 最优性剪枝 | 当前状态不可能优于已找到的最优解时提前返回 |
| 重复性剪枝 | 同层相同值跳过（组合去重） |
| 上下界剪枝 | 剩余元素全选也不够 / 最小也超 |
| 奇偶剪枝 | 曼哈顿距离奇偶性与剩余步数不符 |
| 记忆化搜索 | 记录状态避免重复计算 |

---

## 四、双向 BFS

适用于起点和终点都已知的**最短路**，可将搜索空间从 `b^d` 降至约 `2 * b^(d/2)`。

```cpp
int bidirectionalBFS(int s, int t) {
    if (s == t) return 0;
    queue<int> qs, qt;
    unordered_map<int, int> ds, dt;  // 记录距离

    qs.push(s); ds[s] = 0;
    qt.push(t); dt[t] = 0;

    while (!qs.empty() && !qt.empty()) {
        // 优先扩展小的队列
        if (qs.size() > qt.size()) {
            swap(qs, qt);
            swap(ds, dt);
        }
        int sz = qs.size();
        while (sz--) {
            int u = qs.front(); qs.pop();
            for (int v : g[u]) {
                if (ds.count(v)) continue;  // 已在本侧访问过
                ds[v] = ds[u] + 1;
                if (dt.count(v))            // 相遇
                    return ds[v] + dt[v];
                qs.push(v);
            }
        }
    }
    return -1;
}
```

**网格双向 BFS：**

```cpp
int d1[1005][1005], d2[1005][1005];
queue<pair<int, int>> q1, q2;

int bidirectionalGridBFS(pair<int,int> s, pair<int,int> t) {
    if (s == t) return 0;
    memset(d1, -1, sizeof(d1));
    memset(d2, -1, sizeof(d2));

    q1.push(s); d1[s.first][s.second] = 0;
    q2.push(t); d2[t.first][t.second] = 0;

    auto expand = [&](queue<pair<int,int>>& q, int d[1005][1005], int other[1005][1005]) {
        int sz = q.size();
        while (sz--) {
            auto [x, y] = q.front(); q.pop();
            for (auto [dx, dy] : dirs) {
                int nx = x + dx, ny = y + dy;
                if (!inBound(nx, ny) || grid[nx][ny] == '#' || d[nx][ny] != -1) continue;
                d[nx][ny] = d[x][y] + 1;
                if (other[nx][ny] != -1) return d[nx][ny] + other[nx][ny];
                q.emplace(nx, ny);
            }
        }
        return -1;
    };

    while (!q1.empty() && !q2.empty()) {
        int res;
        if (q1.size() <= q2.size()) res = expand(q1, d1, d2);
        else res = expand(q2, d2, d1);
        if (res != -1) return res;
    }
    return -1;
}
```

---

## 五、IDDFS（迭代加深搜索）

限制 DFS 深度逐层增加，结合 DFS 的空间效率与 BFS 的最优性。

### 基础框架

```cpp
int maxDepth;
bool found = false;

void dfs(int depth, ...) {
    if (found) return;
    if (depth == maxDepth) {
        // 检查是否达到目标
        if (/* goal */) found = true;
        return;
    }
    // 剪枝：乐观估计剩余步数
    if (depth + heuristic() > maxDepth) return;
    // 继续搜索
    for (...) {
        dfs(depth + 1, ...);
    }
}

// 主函数
for (maxDepth = 0; ; ++maxDepth) {
    found = false;
    dfs(0, ...);
    if (found) break;
}
```

### 埃及分数（IDDFS 经典）

```cpp
// 将分数 a/b 拆分为单位分数之和
vector<long long> ans, cur;

// 返回满足 1/x ≤ a/b 的最小 x
long long firstDenom(long long a, long long b) {
    return b / a + 1;
}

bool dfs(long long a, long long b, long long start, int depth) {
    if (depth == 0) {
        if (a == 0) {
            if (ans.empty() || cur.back() < ans.back())
                ans = cur;
            return true;
        }
        return false;
    }
    // 剪枝：剩余最大深度也达不到 a/b
    // a/b ≥ depth * (1/x)  =>  x ≥ depth * b / a
    if (ans.size() && start >= ans.back()) return false;

    bool ok = false;
    for (long long x = max(start, firstDenom(a, b)); ; ++x) {
        if (a * x >= b * depth) break;  // 剪枝：即使后面全放 1/x 也大于剩余
        long long na = a * x - b;
        long long nb = b * x;
        long long g = gcd(na, nb);
        cur.push_back(x);
        if (dfs(na / g, nb / g, x + 1, depth - 1)) ok = true;
        cur.pop_back();
    }
    return ok;
}

// 主函数
long long a, b;
cin >> a >> b;
for (int d = 1; ; ++d) {
    ans.clear();
    if (dfs(a, b, firstDenom(a, b), d)) break;
}
// 输出 ans
```

---

## 六、Flood Fill

### DFS 版（递归）

```cpp
void floodFill(int x, int y, char oldVal, char newVal) {
    if (!inBound(x, y) || grid[x][y] != oldVal) return;
    grid[x][y] = newVal;
    for (auto [dx, dy] : dirs)
        floodFill(x + dx, y + dy, oldVal, newVal);
}
```

### BFS 版（显式队列，防爆栈）

```cpp
void floodFillBFS(int sx, int sy, char oldVal, char newVal) {
    if (grid[sx][sy] != oldVal) return;
    queue<pair<int, int>> q;
    q.emplace(sx, sy);
    grid[sx][sy] = newVal;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (inBound(nx, ny) && grid[nx][ny] == oldVal) {
                grid[nx][ny] = newVal;
                q.emplace(nx, ny);
            }
        }
    }
}
```

---

## 七、状态压缩 BFS

用二进制位表示状态，适合小规模（n ≤ 20）的最短路。

### 经典：钥匙与门

```cpp
// grid: '.' 空地, '#' 墙, 'a'-'f' 钥匙, 'A'-'F' 门, 'S' 起点, 'E' 终点
int n, m;
int dist[30][30][1 << 6];  // dist[x][y][keyMask]
string grid[30];

int bfs(int sx, int sy) {
    memset(dist, -1, sizeof(dist));
    queue<array<int, 3>> q;  // (x, y, keyMask)
    dist[sx][sy][0] = 0;
    q.push({sx, sy, 0});

    while (!q.empty()) {
        auto [x, y, mask] = q.front(); q.pop();
        int d = dist[x][y][mask];
        if (grid[x][y] == 'E') return d;

        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (!inBound(nx, ny) || grid[nx][ny] == '#') continue;
            char ch = grid[nx][ny];
            int nmask = mask;

            if (ch >= 'A' && ch <= 'F') {  // 门
                int need = ch - 'A';
                if (!(mask >> need & 1)) continue;
            }
            if (ch >= 'a' && ch <= 'f') {  // 钥匙
                nmask |= (1 << (ch - 'a'));
            }
            if (dist[nx][ny][nmask] != -1) continue;
            dist[nx][ny][nmask] = d + 1;
            q.push({nx, ny, nmask});
        }
    }
    return -1;
}
```

### BFS + 状态压缩（15-Puzzle 类）

```cpp
// 将局面编码为整数（n ≤ 15 时可用 long long）
// 例：八数码，用无符号 64 位整数编码
using State = unsigned long long;

unordered_map<State, int> dist;

int bfs(State start, State goal) {
    if (start == goal) return 0;
    queue<State> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        State s = q.front(); q.pop();
        int d = dist[s];
        // 解码、生成下一状态、检查
        // ...
    }
    return -1;
}
```

---

## 八、A\* 搜索

带启发函数的 BFS，用优先队列按 `f(n) = g(n) + h(n)` 扩展。

```cpp
struct Node {
    int x, y;
    int g, h;  // 已走步数 + 启发值
    bool operator>(const Node& o) const { return g + h > o.g + o.h; }
};

int heuristic(int x, int y, int ex, int ey) {
    // 曼哈顿距离（可采纳）
    return abs(x - ex) + abs(y - ey);
    // 欧几里得距离：sqrt((x-ex)^2 + (y-ey)^2)
}

int astar(int sx, int sy, int ex, int ey) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    int dist[1005][1005];
    memset(dist, 0x3f, sizeof(dist));
    dist[sx][sy] = 0;
    pq.push({sx, sy, 0, heuristic(sx, sy, ex, ey)});

    while (!pq.empty()) {
        auto [x, y, g, h] = pq.top(); pq.pop();
        if (x == ex && y == ey) return g;
        if (g != dist[x][y]) continue;  // 过期节点

        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (!inBound(nx, ny) || grid[nx][ny] == '#') continue;
            int ng = g + 1;
            if (ng < dist[nx][ny]) {
                dist[nx][ny] = ng;
                pq.push({nx, ny, ng, heuristic(nx, ny, ex, ey)});
            }
        }
    }
    return -1;
}
```

### 常用启发函数

| 场景 | 启发函数 h(n) | 说明 |
|------|--------------|------|
| 网格（四方向） | 曼哈顿距离 `\|dx\| + \|dy\|` | 可采纳，一致 |
| 网格（八方向） | 切比雪夫距离 `max(\|dx\|, \|dy\|)` | 可采纳 |
| 任意地图 | 欧几里得距离 `sqrt(dx² + dy²)` | 可采纳，但浮点慢 |
| 数独 | 剩余空格最少行 | 不一定可采纳但效率高 |
| 拼图 | 曼哈顿距离和（每块到目标） | 可采纳 |

> **可采纳**：`h(n) ≤ 实际最小代价`，保证找到最优解。

---

## 复杂度总结

| 算法 | 时间 | 空间 | 适用场景 |
|------|------|------|---------|
| DFS | O(V+E) | O(V) | 所有解、连通性、拓扑排序 |
| BFS | O(V+E) | O(V) | 最短路（无权）、层次遍历 |
| 回溯 | O(分支数^深度) | O(深度) | 排列/组合/子集、约束满足 |
| 双向 BFS | O(b^(d/2)) | O(b^(d/2)) | 起点终点已知的最短路 |
| IDDFS | O(b^d) | O(d) | 深度未知的最优解 |
| A\* | O(b^d) 实际快 | O(b^d) | 有启发函数的寻路 |

---

## 注意事项

| 要点 | 说明 |
|------|------|
| DFS 爆栈 | 递归深度 > 1e5 时考虑显式栈或 BFS |
| BFS 队列 | 网格问题用 `queue` 即可，别用 `priority_queue` 除非 A\* |
| 回溯剪枝 | 先排序 + 可行性/最优性剪枝，效率差距巨大 |
| 双向 BFS 相遇 | 检查相遇时是 `ds[v] + dt[v]` 而非 `ds[v] + dt[v] - 1` |
| A\* 距离数组 | 不要用 `visited` 标记，要用 `dist` 判断是否更优 |
| 状态压缩 | 状态数 = `n * m * (1 << k)`，k 过大时无解 |
| 多测清空 | 邻接表、dist、vis 务必清空 |
| 曼哈顿距离 | `abs(x1-x2) + abs(y1-y2)` |
| 网格坐标 | 统一 `x = row, y = col` 避免混乱 |

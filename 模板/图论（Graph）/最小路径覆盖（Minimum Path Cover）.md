# 最小路径覆盖 (Minimum Path Cover)

## 概念

用**最少的顶点不相交有向路径**覆盖 DAG 的所有顶点。

> **关键结论**：DAG 最小路径覆盖数 = **n − 二分图最大匹配数**。
> 建模：每个点拆成**出槽**（能发出的边）与**入槽**（能接收的边）两个顶点；
> 每条有向边 u→v 是"出槽(u) − 入槽(v)"的一条候选匹配边；每个槽至多被匹配一次。
> 匹配边恰对应"被并进链里的原边"，故 链数 = n − 匹配数。

**典型应用**：DAG 上**加最少有向边使拓扑序唯一**（答案 = 链数 − 1，见文末）。

---

## 一、树上特化：O(n) 树形 DP 求最大匹配

> 原图是**树**时，槽图为无环森林，不必建二分图跑匈牙利，直接树形 DP 即可 O(n)。

### 状态设计

以 1 为根（迭代 DFS 免爆栈）。对每个点 u：

| 状态 | 含义 |
|------|------|
| `f[u]` | 子树内最大匹配数（父边**不选**） |
| `inu[u]` | u 的**入槽被父边占用**时子树最优 |
| `outu[u]` | u 的**出槽被父边占用**时子树最优 |

对每个孩子 c（`parDir[c]`：+1 表示边 u→c，-1 表示边 c→u）：
- 边 **u→c**：不选贡献 `f[c]`；选则贡献 `1 + inu[c]`（用 u 出槽、c 入槽）；
- 边 **c→u**：不选贡献 `f[c]`；选则贡献 `1 + outu[c]`（用 u 入槽、c 出槽）。

每个点至多选一条出边、一条入边，各取最大改进：

```
f[u]   = Σ f[c] + mo + mi
inu[u] = Σ f[c] + mo       // 入槽已被占，仍可再选一条出边
outu[u]= Σ f[c] + mi       // 出槽已被占，仍可再选一条入边
```

**最大匹配数 = `f[root]`**。

### 代码（求最大匹配）

```cpp
#include <bits/stdc++.h>
using namespace std;

// 输入: n, adj[u] = {v, dir}（dir = +1 表示 u->v, -1 表示 v->u）
// 返回: 最大匹配数
int max_matching_tree(int n, const vector<vector<pair<int,int>>>& adj) {
    vector<int> parent(n + 1, 0), parDir(n + 1, 0), order;
    vector<int> st = {1};
    parent[1] = -1;
    while (!st.empty()) {                      // 迭代 DFS 定根
        int u = st.back(); st.pop_back();
        order.push_back(u);
        for (auto& [v, dr] : adj[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            parDir[v] = dr;                    // 子视角: 边 parent->v 则 +1, v->parent 则 -1
            st.push_back(v);
        }
    }
    vector<vector<int>> children(n + 1);
    for (int v = 1; v <= n; ++v)
        if (parent[v] > 0) children[parent[v]].push_back(v);

    vector<int> f(n + 1, 0), inu(n + 1, 0), outu(n + 1, 0);
    vector<int> moChild(n + 1, 0), miChild(n + 1, 0);
    for (int it = (int)order.size() - 1; it >= 0; --it) {   // 自底向上
        int u = order[it];
        int base = 0, mo = 0, mi = 0;
        for (int c : children[u]) {
            base += f[c];
            if (parDir[c] == +1) {             // u->c 出边
                int imp = (1 + inu[c]) - f[c];
                if (imp > mo) mo = imp, moChild[u] = c;
            } else {                           // c->u 入边
                int imp = (1 + outu[c]) - f[c];
                if (imp > mi) mi = imp, miChild[u] = c;
            }
        }
        f[u] = base + mo + mi;
        inu[u] = base + mo;
        outu[u] = base + mi;
    }
    return f[1];
}
```

### 重建：标记实际选中的边（自顶向下传"槽位占用"状态）

```cpp
// state[u]: 0 两槽空闲, 1 入槽被父边占, 2 出槽被父边占
vector<int> outSel(n + 1, 0), inSel(n + 1, 0);   // 选中的出/入边
vector<int> state(n + 1, 0);
queue<int> q;
q.push(1);
while (!q.empty()) {
    int u = q.front(); q.pop();
    int s = state[u];
    for (int c : children[u]) {
        if (parDir[c] == +1) {                  // u->c, 至多选一条
            int imp = (1 + inu[c]) - f[c];
            if (c == moChild[u] && imp > 0 && s != 2) {
                outSel[u] = c; inSel[c] = u; state[c] = 1;
            } else state[c] = 0;
        } else {                                // c->u, 至多选一条
            int imp = (1 + outu[c]) - f[c];
            if (c == miChild[u] && imp > 0 && s != 1) {
                inSel[u] = c; outSel[c] = u; state[c] = 2;
            } else state[c] = 0;
        }
        q.push(c);
    }
}
// 选中边由 outSel/inSel 表示: 边 u->v 被选 ⇔ outSel[u]==v && inSel[v]==u
```

---

## 二、由选边拼出链（覆盖）

选中的边满足"每点入 ≤1、出 ≤1"，构成若干条顶点不相交的有向链。

```cpp
// 链头 = 没有入边被选中的点；沿出边一路走到链尾
vector<int> chainOf(n + 1, 0), chainStart, chainEnd;
int cid = 0;
for (int u = 1; u <= n; ++u) {
    if (inSel[u] != 0) continue;                 // 不是链头
    ++cid;
    int v = u, last = u;
    while (v != 0) { chainOf[v] = cid; last = v; v = outSel[v]; }
    chainStart.push_back(u);
    chainEnd.push_back(last);
}
// cid = 链数 = n - maxMatching
```

---

## 三、应用：加最少边使 DAG 拓扑序唯一

> **结论**：DAG 拓扑序**唯一 ⟺ 存在哈密顿路径**（经过所有点的有向路径）。
> 故 = 先做最少链覆盖，再加 **链数 − 1** 条边把链**首尾相接**成一条大链。
> 新边方向：`上一条链的链尾 → 下一条链的链头`。

链的先后必须满足所有未被选中的原边（箭头指向的链排后面），用 **Kahn 拓扑排序**定序：

```cpp
vector<int> indeg(cid + 1, 0);
vector<vector<int>> cg(cid + 1);
for (auto& [u, v] : raw) {                       // raw: 所有原边 (u->v)
    if (outSel[u] == v && inSel[v] == u) continue;  // 已选进链，跳过
    int cu = chainOf[u], cv = chainOf[v];
    if (cu != cv) { cg[cu].push_back(cv); ++indeg[cv]; }   // 注意 ++
}
queue<int> cq;
for (int i = 1; i <= cid; ++i)
    if (indeg[i] == 0) cq.push(i);
vector<int> corder;
while (!cq.empty()) {
    int u = cq.front(); cq.pop();
    corder.push_back(u);
    for (int v : cg[u])
        if (--indeg[v] == 0) cq.push(v);
}

cout << cid - 1 << '\n';                         // 最少新边数 K
for (int i = 0; i + 1 < cid; ++i)
    cout << chainEnd[corder[i] - 1] << ' '
         << chainStart[corder[i + 1] - 1] << '\n';
```

> 注意：链间建图时 `indeg[cv]` 必须写 `++indeg[cv]`，漏掉 `++` 会令所有链入度恒为 0，拓扑序变成编号序 → 出错（易错点）。

---

## 四、一般 DAG：二分图最大匹配（匈牙利算法 O(nm)）

> 树可用上面的 O(n) DP；**任意 DAG** 需显式建二分图跑最大匹配（n ≤ 500 时匈牙利够用，更大用 Hopcroft–Karp）。

```cpp
// 左部: 每个点的出槽, 右部: 每个点的入槽
// 边 u->v 对应 match 候选: L[u] -> R[v]
int kuhn(int n, const vector<vector<int>>& g) {  // g[L] = 可匹配的 R 列表
    vector<int> matchR(n + 1, 0);
    vector<int> vis(n + 1, 0);
    function<bool(int)> dfs = [&](int u) -> bool {
        for (int v : g[u]) {
            if (vis[v]) continue;
            vis[v] = 1;
            if (!matchR[v] || dfs(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
        return false;
    };
    int res = 0;
    for (int u = 1; u <= n; ++u) {
        fill(vis.begin(), vis.end(), 0);
        if (dfs(u)) ++res;
    }
    return res;
}
// 最小路径覆盖数 = n - res
```

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 唯一拓扑序 ⟺ 哈密顿路径 | 相邻元素之间必有直接有向边，否则可交换 |
| 答案公式 | 加最少边 = 链数 − 1 = (n−1) − 最大匹配 |
| 每点两槽 | 入槽/出槽各 ≤1，保证选中边拼成链而非分叉 |
| 树形 DP 顺序 | 先定根得到 order，再**自底向上**递推 |
| 重建要传状态 | `state` 记录父边占了哪个槽，决定能否选对应孩子边 |
| 链间排序 | 未选原边连不同链 → 建链间 DAG → Kahn；`++indeg` 勿漏 |
| 新边方向 | 上链链尾 → 下链链头，保证最终成一条哈密顿链 |
| 大 n 递归 | 定根/DP 用迭代栈，避免 1e5 递归爆栈 |

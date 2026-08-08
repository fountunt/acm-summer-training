# 异或约束图传播（XOR Constraint Graph Propagation）

## 概念

有一类问题给出若干 **布尔变量** 和一组形如

```
x ⊕ y = c        （c = 0 或 1）
```

的约束，要求给所有变量赋 0/1 值满足全部约束（题目保证有解，或要求判断有解）。其中 `⊕` 是异或：`c=0` 表示 x、y **相同**，`c=1` 表示 x、y **相反**。

**核心模型**：把每个变量当作图上的一个点；每条约束 `x⊕y=c` 当作一条**无向边**，边权为 `c`。任选一个点赋初值 0，沿边用

```
val[v] = val[u] ⊕ w
```

把整张连通图的值全部推出。若图不连通，则每个连通分量各自任选起点赋 0 即可（每个分量整体取反也满足约束）。

> 与「K 核 / 不动点传播」不同：那是"局部条件满足 → 迭代扩散到最大集合"；这里是"沿等式链直接传递值"，一遍 BFS 即完成，无迭代。

---

## 通用模板 — O(V + E)

```cpp
#include <bits/stdc++.h>
using namespace std;

// ====== 异或约束图：求所有变量取值 ======
// V: 变量个数（编号 0..V-1）；约束 u ⊕ v = w
// 题目保证有解时，返回值满足全部约束；整体取反同样合法。
vector<int> xor_constraint(int V, const vector<tuple<int,int,int>>& cons){
    vector<vector<pair<int,int>>> adj(V);
    auto add = [&](int u,int v,int w){ adj[u].push_back({v,w}); adj[v].push_back({u,w}); };
    for (auto [u,v,w] : cons) add(u,v,w);      // w: 0=相同, 1=相反

    vector<int> val(V, -1);
    for (int st = 0; st < V; ++st) if (val[st] == -1) {
        val[st] = 0;                            // 每个连通分量任选起点赋 0
        queue<int> q; q.push(st);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [v, w] : adj[u])
                if (val[v] == -1) { val[v] = val[u] ^ w; q.push(v); }
        }
    }
    // 若需判无解：遇到 val[v] != -1 且 val[v] != val[u]^w 即冲突
    return val;
}
```

### 关键点

| 要点 | 说明 |
|------|------|
| 边权含义 | `w=0`：两变量必须相同；`w=1`：必须相反 |
| 双向加边 | 无向边（`x⊕y=c` 对称），`add` 时 u、v 都加 |
| 多连通分量 | 每个分量独立取反仍满足约束；外层 for 兜底 |
| 整体取反 | 把所有变量 `^1`（等价于全部关系反转）仍满足所有约束，解往往仍合法 |
| 冲突判断 | 传播时若已赋值点与新推值不同 → 无解 |
| 复杂度 | O(V + E)，一遍 BFS |

---

## 识别特征

题目让你从"成对的**相同/相反**关系"（或 XOR 等式、奇偶约束）还原一组布尔变量/排列时：

- 每给一个**组合**，能推出其中两个变量的关系（相同或相反）→ 拆成一条边权 0/1 的无向边；
- 例如：三色中位数、两两比较符号、奇偶翻转、括号匹配类型问题。

---

## 应用示例：三色中位数（牛客暑期多校 C 题）

红 x 只、绿 y 只、蓝 z 只，名次全不同。给每对异色一个布尔变量（0 = 前者更快）：

```
x[i][j] = 0 ⟺ R_i < G_j      // R-G 对
y[i][k] = 0 ⟺ R_i < B_k      // R-B 对
z[j][k] = 0 ⟺ G_j < B_k      // G-B 对
```

对询问 (R_i, G_j, B_k)，机器给出的**中位数颜色** c 恰好给出**两条**异或约束：

```
x[i][j] ⊕ y[i][k] = (c == 'R')     // 中间是 R ⟹ 两者相反
y[i][k] ⊕ z[j][k] = (c == 'B')     // 中间是 B ⟹ 两者相反
```

⚠️ **必须两条都加**：只按字符挑一条，某些退化数据（如全部中位数为 B）会使 R-G 变量连不进图，恢复错误。

之后：BFS 传播出所有异色大小关系 → 每个关系画成有向边（先到 → 后到）→ 拓扑排序即得总名次。

```cpp
// 变量编号（x,y,z ≤ 150）
auto idX=[&](int i,int j){return (i-1)*y+(j-1);};
auto idY=[&](int i,int k){return x*y+(i-1)*z+(k-1);};
auto idZ=[&](int j,int k){return x*y+x*z+(j-1)*z+(k-1);};
int V = x*y + x*z + y*z;

vector<vector<pair<int,int>>> adj(V);
auto add=[&](int u,int v,int w){adj[u].push_back({v,w});adj[v].push_back({u,w});};
auto addC=[&](int i,int j,int k){ char c = s[i][j][k];
    add(idX(i,j), idY(i,k), c=='R');     // x ⊕ y = [中位数是 R]
    add(idY(i,k), idZ(j,k), c=='B');     // y ⊕ z = [中位数是 B]
};
// 只需用"i,j,k 至少一个为 1"的询问即可连通全图：
for(int i=1;i<=x;i++)for(int j=1;j<=y;j++) addC(i,j,1);   // k=1
for(int i=1;i<=x;i++)for(int k=1;k<=z;k++) addC(i,1,k);   // j=1
for(int j=1;j<=y;j++)for(int k=1;k<=z;k++) addC(1,j,k);   // i=1

// BFS 传播 → 得所有 x/y/z 的值 → 画有向边 → 拓扑排序
```

复杂度：读入 O(xyz)，构图+传播 O(xy+xz+yz)。

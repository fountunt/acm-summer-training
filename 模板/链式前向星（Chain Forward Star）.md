# 链式前向星 (Chain Forward Star)

## 概念

链式前向星是用**数组模拟链表**实现的静态邻接表，是 OI/ACM 中最常用的建图方式之一。

**与 `vector<vector<int>>` 的对比：**

| 特性 | 链式前向星 | `vector<vector<int>>` |
|------|-----------|----------------------|
| 内存 | 紧凑（边存一起，全部连续） | 每个顶点一个 vector，总连续但可能多次分配 |
| 建图速度 | O(m)，无动态扩容 | O(m)，但有少量动态扩容开销 |
| 遍历邻居 | 按插入顺序反向遍历 | 按插入顺序正向遍历 |
| 删边 | 不支持（除非打标记） | 不支持 |
| 反向边（Dinic） | 容易，`eid ^ 1` | 需要存指针/index |

> **结论：** 绝大多数情况下两者性能差距可以忽略。链式前向星适合**卡常**、**最大流**（需要反向边）、以及习惯数组写法的选手。

---

## 基础模板

```cpp
// === 链式前向星（无权） ===
const int N = 1e5 + 5;    // 最大顶点数
const int M = 2e5 + 5;    // 最大边数（无向图要 ×2）

int head[N], to[M], nxt[M], eid = 1;  // eid 从 1 开始，方便 ^1 取反向边

void addEdge(int u, int v) {
    to[++eid] = v;
    nxt[eid] = head[u];
    head[u] = eid;
}

// 遍历 u 的所有邻居
for (int i = head[u]; i; i = nxt[i]) {
    int v = to[i];
    // process(u, v)
}
```

```cpp
// === 链式前向星（带权） ===
const int N = 1e5 + 5;
const int M = 2e5 + 5;

int head[N], to[M], nxt[M], w[M], eid = 1;

void addEdge(int u, int v, int weight) {
    to[++eid] = v;
    w[eid] = weight;
    nxt[eid] = head[u];
    head[u] = eid;
}

// 遍历 u 的所有邻居
for (int i = head[u]; i; i = nxt[i]) {
    int v = to[i], weight = w[i];
    // process(u, v, weight)
}
```

---

## 无向图

在 `addEdge` 外面包一层即可：

```cpp
void addUndirected(int u, int v) {  // 无权
    addEdge(u, v);
    addEdge(v, u);
}

void addUndirected(int u, int v, int w) {  // 带权
    addEdge(u, v, w);
    addEdge(v, u, w);
}
```

---

## 反向边技巧（用于最大流 / 二分图匹配）

`eid` 从 **1** 开始，则 `i ^ 1` 就是 `i` 对应的反向边：

```cpp
int head[N], to[M], nxt[M], cap[M], eid = 1;

void addEdge(int u, int v, int c) {
    to[++eid] = v;
    cap[eid] = c;
    nxt[eid] = head[u];
    head[u] = eid;
}

// 添加一对正向 + 反向边
void addFlow(int u, int v, int c) {
    addEdge(u, v, c);   // 正向边，eid = X
    addEdge(v, u, 0);   // 反向边，eid = X ^ 1
}

// 在 Dinic/ISAP 中使用：
int& flow = cap[i];
int& rflow = cap[i ^ 1];  // 反向边容量
```

> 如果 `eid` 从 0 开始，则 `i ^ 1` 在 `i` 为偶数时拿到 `i+1`，奇数时拿到 `i-1`，效果一样。
> 用 `eid = 1` 的另一个好处：`head[]` 初始化为 0 表示"无边"，遍历时 `for (int i = head[u]; i; i = nxt[i])`。

---

## 边集数组形式（结构体版）

适合需要排序（如 Kruskal）或保留边列表的场景：

```cpp
struct Edge {
    int u, v, w, nxt;
} edges[M];
int head[N], eid = 1;

void addEdge(int u, int v, int w) {
    edges[++eid] = {u, v, w, head[u]};
    head[u] = eid;
}

// 遍历
for (int i = head[u]; i; i = edges[i].nxt) {
    int v = edges[i].v, w = edges[i].w;
    // process
}

// Kruskal 时需要单独拎出边列表
vector<Edge> edge_list(edges + 1, edges + 1 + eid);
sort(edge_list.begin(), edge_list.end(), [](auto& a, auto& b) {
    return a.w < b.w;
});
```

---

## 常见操作

### 1. 清空（重新建图）

```cpp
eid = 1;
memset(head, 0, sizeof(head));
// to / nxt / w 可以不清空，下一轮直接覆盖
```

### 2. 获取总边数

```cpp
int total_edges() { return eid - 1; }  // 单向边计数
int total_undirected_edges() { return (eid - 1) / 2; }  // 无向边计数
```

### 3. 枚举所有边

```cpp
for (int i = 2; i <= eid; ++i) {
    int u = to[i ^ 1];     // 对于 addEdge(u, v, w)，反向边的 to 就是 u
    // 但注意：只有成对加的边才能这样用
}
```

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 数组大小 | 无向图边数要 **×2**，`M` 记得开够 |
| eid 初值 | 习惯 `eid = 1`（遍历时 `head[u]` 为 0 表示空，免初始化） |
| 多测清空 | `eid = 1; memset(head, 0, sizeof(head));` 即可 |
| 遍历顺序 | 链式前向星是**逆序**遍历邻居（后插入的先访问），vector 是正序 |
| 结构体 vs 纯数组 | 结构体版方便存边列表排序，纯数组版访问更快（少一层间接） |
| 开 long long | 权值超过 2e9 时 `w[]` 类型改为 `long long` |
| 反向边配对的限制 | `addFlow` 必须**连续调用**，正向 + 反向作为一对 |
| 动态加边 | 边数已知时用链式前向星即可；若完全未知，用 vector 更灵活 |

## 复杂度

| 操作 | 复杂度 |
|------|--------|
| 加边 | O(1) |
| 遍历某个点的所有边 | O(deg(u)) |
| 遍历所有边 | O(m) |
| 空间 | O(n + m) |

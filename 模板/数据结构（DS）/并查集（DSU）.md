# 并查集 (Disjoint Set Union / Union-Find)

## 概念

维护若干个不相交的集合，支持 **合并** 和 **查询所属集合** 两种操作。

- **路径压缩**：查询时直接把节点指向根，降低树高。
- **按秩合并**：将较矮的树合并到较高的树上，保证树高 O(log n)。
- 两者结合后，均摊复杂度为反阿克曼函数，几乎为常数。

---

## 基础模板

```cpp
struct DSU {
    vector<int> fa, sz;  // 父节点 / 集合大小

    DSU(int n) {
        fa.resize(n);
        sz.resize(n, 1);
        for (int i = 0; i < n; ++i) fa[i] = i;
    }

    int find(int x) {                       // 查询根 + 路径压缩
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    bool same(int x, int y) {               // 判断是否同一集合
        return find(x) == find(y);
    }

    bool unite(int x, int y) {              // 合并，返回是否合并成功
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        fa[y] = x;
        sz[x] += sz[y];
        return true;
    }

    int size(int x) {                       // 集合大小
        return sz[find(x)];
    }
};
```

---

## 按秩合并（优化版，只维护树高）

```cpp
struct DSU {
    vector<int> fa, rk;

    DSU(int n) {
        fa.resize(n);
        rk.resize(n, 0);
        for (int i = 0; i < n; ++i) fa[i] = i;
    }

    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }

    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (rk[x] < rk[y]) swap(x, y);
        fa[y] = x;
        if (rk[x] == rk[y]) ++rk[x];
        return true;
    }
};
```

---

## 带权并查集（维护每个节点到根的距离）

```cpp
struct DSU {
    vector<int> fa;
    vector<long long> dist;  // dist[x] = x 到 fa[x] 的权值（按需修改类型）

    DSU(int n) {
        fa.resize(n);
        dist.resize(n, 0);
        for (int i = 0; i < n; ++i) fa[i] = i;
    }

    int find(int x) {
        if (fa[x] == x) return x;
        int root = find(fa[x]);
        dist[x] += dist[fa[x]];   // 路径压缩时累加权值
        return fa[x] = root;
    }

    // 合并：将 y 所在集合合并到 x 所在集合，关系为 val(y) - val(x) = w
    bool unite(int x, int y, long long w) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        // 保证 rx 的深度 <= ry，方便调整
        fa[rx] = ry;
        dist[rx] = -dist[x] + w + dist[y];
        return true;
    }

    long long diff(int x, int y) {  // 返回 val(x) - val(y)
        return dist[x] - dist[y];
    }
};
```

用途：判断种类 / 相对关系（如食物链、区间奇偶性）。

---

## 可撤销并查集（支持回退）

```cpp
struct RollbackDSU {
    vector<int> fa, sz;
    vector<tuple<int, int, int>> stk;  // 记录历史操作

    RollbackDSU(int n) {
        fa.resize(n);
        sz.resize(n, 1);
        for (int i = 0; i < n; ++i) fa[i] = i;
    }

    int find(int x) {
        while (fa[x] != x) x = fa[x];
        return x;  // 注意：不可路径压缩（否则无法回退）
    }

    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        stk.emplace_back(x, y, sz[x]);
        fa[y] = x;
        sz[x] += sz[y];
        return true;
    }

    void rollback() {
        auto [x, y, old_sz] = stk.back();
        stk.pop_back();
        fa[y] = y;
        sz[x] = old_sz;
    }

    int snapshot() { return stk.size(); }
    void rollback_to(int snap) { while (stk.size() > snap) rollback(); }
};
```

用途：离线分治（CDQ / 线段树分治）、DFS 回溯中的动态连通性。

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 下标 | 一般从 0 开始，若输入从 1 开始，可 DSU(n + 1) 并用下标 1..n |
| 路径压缩 + 按秩合并 | 两者一起用才能达到近乎 O(1) 的均摊复杂度 |
| 并查集不可逆 | 除 RollbackDSU 外，正常并查集无法分裂集合 |
| 带权并查集 | find 时要先记录原父节点，递归回来后再更新权值 |
| 初始化 | 多个测试用例时，每轮重新构造 DSU(n) 即可 |

# 线段树 (Segment Tree)

## 概念

线段树是基于分治的二叉树，维护序列的区间信息（和、最值、gcd、最大子段和等）。

- **建树** O(n)，**单点/区间操作** O(log n)
- 每个节点代表区间 `[l, r]`，左右孩子各管一半
- 懒标记实现区间修改的 O(log n) 更新

---

## ① 通用线段树模板（单点修改 + 区间查询）

**套用方式：只改 `Node` 的字段 和 `merge()`，其余一行不用动。**
适用于区间和、最值、gcd、异或、按位或、最大子段和等一切"可合并"的信息，能覆盖线段树大部分题目。

```cpp
struct SegTree {
    struct Node {
        long long sum;                 // ① 需要维护的信息（按题目增减字段）
        Node(long long v = 0) : sum(v) {}
    };
    static Node merge(const Node &a, const Node &b) {   // ② 区间合并规则
        return Node(a.sum + b.sum);
        // 最值：return Node(max(a.sum, b.sum));
        // gcd ：return Node(__gcd(a.sum, b.sum));
        // 异或：return Node(a.sum ^ b.sum);
    }
    int n;
    vector<Node> t;
    SegTree(int _n) : n(_n), t(4 * _n) {}
    SegTree(const vector<long long> &a) : SegTree((int)a.size() - 1) {  // a[1..n]
        build(1, 1, n, a);
    }

    void pull(int p) { t[p] = merge(t[p << 1], t[p << 1 | 1]); }

    void build(int p, int l, int r, const vector<long long> &a) {
        if (l == r) { t[p] = Node(a[l]); return; }
        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void upd(int p, int l, int r, int idx, long long v) {
        if (l == r) { t[p] = Node(v); return; }
        int m = (l + r) >> 1;
        if (idx <= m) upd(p << 1, l, m, idx, v);
        else upd(p << 1 | 1, m + 1, r, idx, v);
        pull(p);
    }

    Node qry(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p];
        int m = (l + r) >> 1;
        if (qr <= m) return qry(p << 1, l, m, ql, qr);
        if (ql > m)  return qry(p << 1 | 1, m + 1, r, ql, qr);
        return merge(qry(p << 1, l, m, ql, qr), qry(p << 1 | 1, m + 1, r, ql, qr));
    }

    // ---------- 对外接口（下标从 1 开始）----------
    void init(const vector<long long> &a) { build(1, 1, n, a); }
    void set(int idx, long long v) { upd(1, 1, n, idx, v); }
    Node query(int l, int r) { return qry(1, 1, n, l, r); }
};
```

> 查询采用"两边各取一部分再 merge"的写法，**不需要定义单位元**（identity），所以改成最值 / gcd 时无需额外初始化 −∞ 或 0。

**改造示例 — 最大子段和**（只改 Node 和 merge）：

```cpp
struct Node {
    long long sum, lmx, rmx, mx;      // 区间和、前缀最大、后缀最大、最大子段和
    Node(long long v = 0)
        : sum(v), lmx(max(0LL, v)), rmx(max(0LL, v)), mx(max(0LL, v)) {}  // 0 表示允许空段
};
static Node merge(const Node &a, const Node &b) {
    Node c;
    c.sum = a.sum + b.sum;
    c.lmx = max(a.lmx, a.sum + b.lmx);
    c.rmx = max(b.rmx, b.sum + a.rmx);
    c.mx = max({a.mx, b.mx, a.rmx + b.lmx});
    return c;
}
```

---

## ② 通用懒标记线段树模板（区间修改 + 区间查询）

**支持区间加 / 区间乘 / 区间赋值，维护区间和、最大、最小。**
套用方式：改 `Node`（维护信息）、`Tag`（懒标记）、`apply()` 三处。

```cpp
struct LazySeg {
    struct Node {
        long long sum, mn, mx;                     // ① 需要维护的信息
        Node(long long _s = 0, long long _mn = 1e18, long long _mx = -1e18)
            : sum(_s), mn(_mn), mx(_mx) {}
    };
    struct Tag {                                   // ② 懒标记：先乘后加
        long long mul, add;
        Tag(long long _m = 1, long long _a = 0) : mul(_m), add(_a) {}
        bool empty() const { return mul == 1 && add == 0; }
    };
    int n;
    vector<Node> t;
    vector<Tag> lz;

    LazySeg(int _n) : n(_n), t(4 * _n), lz(4 * _n) {}

    Node merge(const Node &a, const Node &b) {
        return Node(a.sum + b.sum, min(a.mn, b.mn), max(a.mx, b.mx));
    }
    // ③ 用懒标记 g 修改节点 p 的信息（区间长为 len）
    void apply(int p, int len, const Tag &g) {
        t[p].sum = t[p].sum * g.mul + g.add * len;
        if (g.mul < 0) swap(t[p].mn, t[p].mx);     // 乘负数时最值互换
        t[p].mn = t[p].mn * g.mul + g.add;
        t[p].mx = t[p].mx * g.mul + g.add;
        lz[p].mul *= g.mul;
        lz[p].add = lz[p].add * g.mul + g.add;
    }
    void push(int p, int l, int r) {
        if (lz[p].empty()) return;
        int m = (l + r) >> 1;
        apply(p << 1, m - l + 1, lz[p]);
        apply(p << 1 | 1, r - m, lz[p]);
        lz[p] = Tag();
    }
    void build(int p, int l, int r, const vector<long long> &a) {
        if (l == r) { t[p] = Node(a[l], a[l], a[l]); return; }
        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        t[p] = merge(t[p << 1], t[p << 1 | 1]);
    }
    void upd(int p, int l, int r, int ql, int qr, const Tag &g) {
        if (ql <= l && r <= qr) { apply(p, r - l + 1, g); return; }
        push(p, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) upd(p << 1, l, m, ql, qr, g);
        if (qr > m)  upd(p << 1 | 1, m + 1, r, ql, qr, g);
        t[p] = merge(t[p << 1], t[p << 1 | 1]);
    }
    Node qry(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p];
        push(p, l, r);
        int m = (l + r) >> 1;
        if (qr <= m) return qry(p << 1, l, m, ql, qr);
        if (ql > m)  return qry(p << 1 | 1, m + 1, r, ql, qr);
        return merge(qry(p << 1, l, m, ql, qr), qry(p << 1 | 1, m + 1, r, ql, qr));
    }

    // ---------- 对外接口 ----------
    void init(const vector<long long> &a) { build(1, 1, n, a); }
    void range_add(int l, int r, long long v) { upd(1, 1, n, l, r, Tag(1, v)); }
    void range_mul(int l, int r, long long v) { upd(1, 1, n, l, r, Tag(v, 0)); }
    void range_set(int l, int r, long long v) { upd(1, 1, n, l, r, Tag(0, v)); }  // 赋值 = 乘 0 加 v
    long long range_sum(int l, int r) { return qry(1, 1, n, l, r).sum; }
    long long range_max(int l, int r) { return qry(1, 1, n, l, r).mx; }
    long long range_min(int l, int r) { return qry(1, 1, n, l, r).mn; }
};
```

> 只做**区间加**：删掉 `apply` 里所有 `mul` 相关行，`Tag` 只留 `add`。
> 只做**区间赋值**：更简化为 `t[p] = v * len; lz[p] = v;`。
> 若乘数恒非负，可删掉 `if (g.mul < 0) swap(...)` 一行。

---

## ③ 线段树 Beats（区间加 + 区间 chmin + 区间求和 / 最大值）

维护区间最大值、严格次大值、最大值个数。区间 chmin 时：若最大值 ≤ v 直接跳过；若 次大值 < v < 最大值，整体打标记；否则暴力下递归。均摊 O((n + q) log n)。

```cpp
struct SegTreeBeats {
    struct Node {
        long long mx, se, cnt, sum, add;
        // mx: 最大值, se: 严格次大值, cnt: 最大值个数
        // sum: 区间和, add: 区间加懒标记
    };
    int n;
    vector<Node> t;

    SegTreeBeats(int _n) : n(_n), t(4 * _n) {}

    void pull(int p) {
        auto &ls = t[p << 1], &rs = t[p << 1 | 1];
        t[p].sum = ls.sum + rs.sum;
        if (ls.mx > rs.mx) {
            t[p].mx = ls.mx;
            t[p].cnt = ls.cnt;
            t[p].se = max(ls.se, rs.mx);
        } else if (rs.mx > ls.mx) {
            t[p].mx = rs.mx;
            t[p].cnt = rs.cnt;
            t[p].se = max(rs.se, ls.mx);
        } else {
            t[p].mx = ls.mx;
            t[p].cnt = ls.cnt + rs.cnt;
            t[p].se = max(ls.se, rs.se);
        }
    }

    void apply_add(int p, int l, int r, long long v) {
        t[p].mx += v;
        if (t[p].cnt < (r - l + 1)) t[p].se += v;   // 存在次大值才加，避免 -1e18 哨兵误判
        t[p].sum += v * (r - l + 1);
        t[p].add += v;
    }

    void apply_chmin(int p, long long v) {          // 前提：se < v < mx
        if (t[p].mx <= v) return;
        t[p].sum -= t[p].cnt * (t[p].mx - v);
        t[p].mx = v;
    }

    void push(int p, int l, int r) {
        if (t[p].add != 0) {
            apply_add(p << 1, l, (l + r) >> 1, t[p].add);
            apply_add(p << 1 | 1, ((l + r) >> 1) + 1, r, t[p].add);
            t[p].add = 0;
        }
        apply_chmin(p << 1, t[p].mx);
        apply_chmin(p << 1 | 1, t[p].mx);
    }

    void build(int p, int l, int r, const vector<long long> &a) {
        t[p].add = 0;
        if (l == r) {
            t[p].mx = t[p].sum = a[l];
            t[p].cnt = 1;
            t[p].se = -1e18;
            return;
        }
        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void range_add(int p, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) { apply_add(p, l, r, v); return; }
        push(p, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) range_add(p << 1, l, m, ql, qr, v);
        if (qr > m)  range_add(p << 1 | 1, m + 1, r, ql, qr, v);
        pull(p);
    }

    void range_chmin(int p, int l, int r, int ql, int qr, long long v) {
        if (t[p].mx <= v) return;
        if (ql <= l && r <= qr && t[p].se < v) { apply_chmin(p, v); return; }
        push(p, l, r);
        int m = (l + r) >> 1;
        if (ql <= m) range_chmin(p << 1, l, m, ql, qr, v);
        if (qr > m)  range_chmin(p << 1 | 1, m + 1, r, ql, qr, v);
        pull(p);
    }

    long long range_sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p].sum;
        push(p, l, r);
        int m = (l + r) >> 1;
        long long res = 0;
        if (ql <= m) res += range_sum(p << 1, l, m, ql, qr);
        if (qr > m)  res += range_sum(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }

    long long range_max(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p].mx;
        push(p, l, r);
        int m = (l + r) >> 1;
        long long res = -1e18;
        if (ql <= m) res = max(res, range_max(p << 1, l, m, ql, qr));
        if (qr > m)  res = max(res, range_max(p << 1 | 1, m + 1, r, ql, qr));
        return res;
    }
};
```

> 对称的区间 **chmax**：将序列取负后调用 chmin，或仿写一份 apply_chmax。

---

## ④ 区间取模 / 区间开方

利用性质：一个数取模或开方后，在有限次内变成 0 / 1（开方）或不再变化（取模），可暴力递归到叶子。

```cpp
struct ModSqrtSeg {
    int n;
    vector<long long> t, mx;   // mx: 区间最大值，用于剪枝

    ModSqrtSeg(int _n) : n(_n), t(4 * _n), mx(4 * _n) {}

    void pull(int p) {
        t[p] = t[p << 1] + t[p << 1 | 1];
        mx[p] = max(mx[p << 1], mx[p << 1 | 1]);
    }

    void build(int p, int l, int r, const vector<long long> &a) {
        if (l == r) { t[p] = mx[p] = a[l]; return; }
        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void range_mod(int p, int l, int r, int ql, int qr, long long mod) {
        if (mx[p] < mod) return;              // 最大值 < mod，取模无变化
        if (l == r) { t[p] %= mod; mx[p] = t[p]; return; }
        int m = (l + r) >> 1;
        if (ql <= m) range_mod(p << 1, l, m, ql, qr, mod);
        if (qr > m)  range_mod(p << 1 | 1, m + 1, r, ql, qr, mod);
        pull(p);
    }

    void range_sqrt(int p, int l, int r, int ql, int qr) {
        if (mx[p] <= 1) return;               // 全为 0/1，不再变化
        if (l == r) { t[p] = (long long)sqrt(t[p]); mx[p] = t[p]; return; }
        int m = (l + r) >> 1;
        if (ql <= m) range_sqrt(p << 1, l, m, ql, qr);
        if (qr > m)  range_sqrt(p << 1 | 1, m + 1, r, ql, qr);
        pull(p);
    }

    long long range_sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p];
        int m = (l + r) >> 1;
        long long res = 0;
        if (ql <= m) res += range_sum(p << 1, l, m, ql, qr);
        if (qr > m)  res += range_sum(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }
};
```

---

## ⑤ 线段树合并（权值线段树合并）

常用于树上问题（作为 dsu on tree 的替代），把两棵线段树合并成一棵。需动态开点。

```cpp
struct MergeSegTree {
    struct Node {
        long long sum;
        int lc, rc;
        Node() : sum(0), lc(0), rc(0) {}
    };
    vector<Node> t;
    int idx;                                  // 当前已用节点数

    MergeSegTree(int cap = 4'000'000) {
        t.resize(cap + 1);
        idx = 0;
    }

    int new_node() { return ++idx; }

    void add(int &p, int l, int r, int pos, long long val) {
        if (!p) p = new_node();
        t[p].sum += val;
        if (l == r) return;
        int m = (l + r) >> 1;
        if (pos <= m) add(t[p].lc, l, m, pos, val);
        else add(t[p].rc, m + 1, r, pos, val);
    }

    long long qry(int p, int l, int r, int ql, int qr) {
        if (!p || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return t[p].sum;
        int m = (l + r) >> 1;
        return qry(t[p].lc, l, m, ql, qr) + qry(t[p].rc, m + 1, r, ql, qr);
    }

    int merge(int a, int b, int l, int r) {   // 把 b 合并进 a，返回新根
        if (!a || !b) return a | b;
        if (l == r) { t[a].sum += t[b].sum; return a; }
        int m = (l + r) >> 1;
        t[a].lc = merge(t[a].lc, t[b].lc, l, m);
        t[a].rc = merge(t[a].rc, t[b].rc, m + 1, r);
        t[a].sum = t[a].sum + t[b].sum;
        return a;
    }

    void split(int p, int l, int r, long long k, int &lch, int &rch) {  // 前 k 小到左树
        if (!p) { lch = rch = 0; return; }
        if (l == r) {
            if (k == 0) { lch = 0; rch = p; return; }
            lch = p; rch = 0;
            t[lch].sum = k;
            if (k < t[p].sum) {                // 劈开叶子：剩余部分归右树
                rch = new_node();
                t[rch].sum = t[p].sum - k;
            }
            return;
        }
        lch = new_node(); rch = new_node();
        int m = (l + r) >> 1;
        long long left_sum = t[t[p].lc].sum;
        if (k >= left_sum) {
            t[lch].rc = t[p].rc; t[rch].rc = 0;
            split(t[p].lc, l, m, k - left_sum, t[lch].lc, t[rch].lc);
        } else {
            t[lch].rc = 0; t[rch].rc = t[p].rc;
            split(t[p].lc, l, m, k, t[lch].lc, t[rch].lc);
        }
        t[lch].sum = t[t[lch].lc].sum + t[t[lch].rc].sum;
        t[rch].sum = t[t[rch].lc].sum + t[t[rch].rc].sum;
    }
};
```

---

## ⑥ 可持久化线段树（主席树）

每次修改复制一条到根的新路径，老版本保持不变。用于区间第 k 大、区间内 <= x 的个数等。

```cpp
struct PST {
    struct Node {
        long long sum;
        int lc, rc;
        Node() : sum(0), lc(0), rc(0) {}
    };
    vector<Node> t;
    vector<int> roots;            // 每个版本的根
    int idx, n;                   // n: 值域大小

    PST(int _n, int cap = 4'000'000) : n(_n) {
        t.resize(cap + 1);
        idx = 0;
        roots.push_back(newnode());   // 版本 0：空树
    }

    int newnode() { t[++idx] = Node(); return idx; }

    // 从版本 pre 复制一份，在 pos 上加 val，存为 cur
    void add(int &cur, int pre, int l, int r, int pos, long long val) {
        cur = newnode();
        t[cur] = t[pre];
        t[cur].sum += val;
        if (l == r) return;
        int m = (l + r) >> 1;
        if (pos <= m) add(t[cur].lc, t[pre].lc, l, m, pos, val);
        else add(t[cur].rc, t[pre].rc, m + 1, r, pos, val);
    }

    long long qry(int p, int l, int r, int ql, int qr) {
        if (!p || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return t[p].sum;
        int m = (l + r) >> 1;
        return qry(t[p].lc, l, m, ql, qr) + qry(t[p].rc, m + 1, r, ql, qr);
    }

    // 区间 [L,R] 版本中的第 k 小（权值线段树上二分）
    int kth(int L, int R, int l, int r, int k) {
        if (l == r) return l;
        int m = (l + r) >> 1;
        int left_cnt = t[t[R].lc].sum - t[t[L].lc].sum;
        if (k <= left_cnt) return kth(t[L].lc, t[R].lc, l, m, k);
        return kth(t[L].rc, t[R].rc, m + 1, r, k - left_cnt);
    }

    // ---------- 对外接口（值域 [1, n]）----------
    int version() { return (int)roots.size() - 1; }
    void add_version(int pos, long long val) {
        int nxt;
        add(nxt, roots.back(), 1, n, pos, val);
        roots.push_back(nxt);
    }
    long long range_sum(int ver, int l, int r) { return qry(roots[ver], 1, n, l, r); }
    int range_kth(int lver, int rver, int k) { return kth(roots[lver], roots[rver], 1, n, k); }
};
```

---

## ⑦ 常用技巧

| 技巧 | 说明 |
|------|------|
| **离散化** | 值域大时先压缩坐标再建树（主席树 / 权值线段树必用） |
| **动态开点** | 不必分配 4n 空间，按需创建节点（线段树合并、值域大时用） |
| **二分查询** | 树上二分：查前缀和 ≥ k 的第一个位置（`if (t[ls].sum < k) k -= ...`） |
| **最大子段和** | Node 存 `sum / lmx / rmx / mx`，见模板 ① 的改造示例 |
| **扫描线** | 用线段树维护 y 轴覆盖长度，按 x 排序扫矩形（面积并、周长并） |
| **树链剖分** | 树上路径 / 子树问题，剖分后映射到 dfs 序，直接用模板 ①② |
| **线段树优化建图** | 建入树、出树两棵线段树，区间向点 / 点向区间连边，边数 O(n log n) |
| **线段树分治** | 把操作插入线段树对应时间区间，DFS 遍历时加入 / 回退 |
| **标记永久化** | 懒标记不下传，查询时累加路径上的标记，常数小，适合可持久化 |

---

## ⑧ 注意事项

| 要点 | 说明 |
|------|------|
| 下标 | 通常从 1 开始，`n` 为序列长度 |
| 空间 | 静态线段树开 `4 * n`；动态开点按 `n log n` 或更多预留 |
| 懒标记 | 递归进子区间前先 `push`，回溯后 `pull` |
| 爆 int | 区间和用 `long long`；乘法、加法标记注意溢出 |
| 负数最值 | 最值初值用 `-1e18` / `1e18`，不要用 `0` |
| 多测试 | 每轮重新构造结构体即可；动态开点结构记得重置 `idx = 0` |

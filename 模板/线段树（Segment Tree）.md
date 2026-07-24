# 线段树 (Segment Tree)

## 概念

线段树是一种基于分治的二叉树结构，用于维护序列的区间信息（和、最值、gcd 等）。

- **建树** O(n)，**单点/区间操作** O(log n)
- 每个节点代表一个区间 `[l, r]`，左右孩子各管一半
- 支持懒标记实现区间修改的 O(log n) 更新

---

## 基础模板（区间和，单点修改 / 区间查询）

```cpp
struct SegTree {
    int n;
    vector<long long> t;

    SegTree(int _n) : n(_n), t(4 * _n) {}

    void pull(int p) {
        t[p] = t[p << 1] + t[p << 1 | 1];
    }

    void build(int p, int l, int r, const vector<int> &a) {
        if (l == r) { t[p] = a[l]; return; }
        int m = (l + r) / 2;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void upd(int p, int l, int r, int idx, int val) {
        if (l == r) { t[p] = val; return; }
        int m = (l + r) / 2;
        if (idx <= m) upd(p << 1, l, m, idx, val);
        else upd(p << 1 | 1, m + 1, r, idx, val);
        pull(p);
    }

    long long qry(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p];
        int m = (l + r) / 2;
        long long res = 0;
        if (ql <= m) res += qry(p << 1, l, m, ql, qr);
        if (qr > m) res += qry(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }

    // ---------- 对外接口 ----------
    void init(const vector<int> &a) { build(1, 1, n, a); }
    void set(int idx, int val) { upd(1, 1, n, idx, val); }
    long long sum(int l, int r) { return qry(1, 1, n, l, r); }
};
```

使用 `SegTree seg(n)` 创建，下标从 1 开始。

---

## 懒标记线段树（区间加 + 区间求和）

```cpp
struct LazySeg {
    int n;
    vector<long long> t, lz;

    LazySeg(int _n) : n(_n), t(4 * _n), lz(4 * _n) {}

    void pull(int p) { t[p] = t[p << 1] + t[p << 1 | 1]; }

    void apply(int p, int l, int r, long long v) {
        t[p] += v * (r - l + 1);
        lz[p] += v;
    }

    void push(int p, int l, int r) {
        if (lz[p] == 0) return;
        int m = (l + r) / 2;
        apply(p << 1, l, m, lz[p]);
        apply(p << 1 | 1, m + 1, r, lz[p]);
        lz[p] = 0;
    }

    void build(int p, int l, int r, const vector<int> &a) {
        if (l == r) { t[p] = a[l]; return; }
        int m = (l + r) / 2;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void add(int p, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) { apply(p, l, r, v); return; }
        push(p, l, r);
        int m = (l + r) / 2;
        if (ql <= m) add(p << 1, l, m, ql, qr, v);
        if (qr > m) add(p << 1 | 1, m + 1, r, ql, qr, v);
        pull(p);
    }

    long long sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p];
        push(p, l, r);
        int m = (l + r) / 2;
        long long res = 0;
        if (ql <= m) res += sum(p << 1, l, m, ql, qr);
        if (qr > m) res += sum(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }

    // ---------- 对外接口 ----------
    void init(const vector<int> &a) { build(1, 1, n, a); }
    void range_add(int l, int r, long long v) { add(1, 1, n, l, r, v); }
    long long range_sum(int l, int r) { return sum(1, 1, n, l, r); }
};
```

---

## 懒标记线段树（区间加 + 区间取 min / 区间求和，支持两种操作）

以下模板同时支持 **区间加** 和 **区间赋值** 两种懒标记，适合复杂的混合操作。

```cpp
struct SegTreeBeats {
    // 以区间加 + 区间 chmin（取 min） + 区间和为例
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
        if (t[p].se != -1e18) t[p].se += v;
        t[p].sum += v * (r - l + 1);
        t[p].add += v;
    }

    void apply_chmin(int p, long long v) {
        if (t[p].mx <= v) return;
        t[p].sum -= t[p].cnt * (t[p].mx - v);
        t[p].mx = v;
    }

    void push(int p, int l, int r) {
        if (t[p].add != 0) {
            apply_add(p << 1, l, (l + r) / 2, t[p].add);
            apply_add(p << 1 | 1, (l + r) / 2 + 1, r, t[p].add);
            t[p].add = 0;
        }
        apply_chmin(p << 1, t[p].mx);
        apply_chmin(p << 1 | 1, t[p].mx);
    }

    void build(int p, int l, int r, const vector<int> &a) {
        t[p].add = 0;
        if (l == r) {
            t[p].mx = t[p].sum = a[l];
            t[p].cnt = 1;
            t[p].se = -1e18;
            return;
        }
        int m = (l + r) / 2;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void range_add(int p, int l, int r, int ql, int qr, long long v) {
        if (ql <= l && r <= qr) { apply_add(p, l, r, v); return; }
        push(p, l, r);
        int m = (l + r) / 2;
        if (ql <= m) range_add(p << 1, l, m, ql, qr, v);
        if (qr > m) range_add(p << 1 | 1, m + 1, r, ql, qr, v);
        pull(p);
    }

    void range_chmin(int p, int l, int r, int ql, int qr, long long v) {
        if (t[p].mx <= v) return;
        if (ql <= l && r <= qr && t[p].se < v) { apply_chmin(p, v); return; }
        push(p, l, r);
        int m = (l + r) / 2;
        if (ql <= m) range_chmin(p << 1, l, m, ql, qr, v);
        if (qr > m) range_chmin(p << 1 | 1, m + 1, r, ql, qr, v);
        pull(p);
    }

    long long range_sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p].sum;
        push(p, l, r);
        int m = (l + r) / 2;
        long long res = 0;
        if (ql <= m) res += range_sum(p << 1, l, m, ql, qr);
        if (qr > m) res += range_sum(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }

    long long range_max(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p].mx;
        push(p, l, r);
        int m = (l + r) / 2;
        long long res = -1e18;
        if (ql <= m) res = max(res, range_max(p << 1, l, m, ql, qr));
        if (qr > m) res = max(res, range_max(p << 1 | 1, m + 1, r, ql, qr));
        return res;
    }
};
```

---

## 区间取模 / 区间开方 线段树

利用性质：一个数被取模或开方后在有限次内变为 0 / 1，可暴力递归到叶子。

```cpp
struct ModSqrtSeg {
    int n;
    vector<long long> t, mx;

    ModSqrtSeg(int _n) : n(_n), t(4 * _n), mx(4 * _n) {}

    void pull(int p) {
        t[p] = t[p << 1] + t[p << 1 | 1];
        mx[p] = max(mx[p << 1], mx[p << 1 | 1]);
    }

    void build(int p, int l, int r, const vector<long long> &a) {
        if (l == r) { t[p] = mx[p] = a[l]; return; }
        int m = (l + r) / 2;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    // 区间取模：若最大值 < mod 则跳过
    void range_mod(int p, int l, int r, int ql, int qr, long long mod) {
        if (mx[p] < mod) return;
        if (l == r) {
            t[p] %= mod;
            mx[p] = t[p];
            return;
        }
        int m = (l + r) / 2;
        if (ql <= m) range_mod(p << 1, l, m, ql, qr, mod);
        if (qr > m) range_mod(p << 1 | 1, m + 1, r, ql, qr, mod);
        pull(p);
    }

    // 区间开方：若最小值为 1 或 0 则跳过（因为已经不会变了）
    void range_sqrt(int p, int l, int r, int ql, int qr) {
        if (mx[p] <= 1) return;
        if (l == r) {
            t[p] = (long long)sqrt(t[p]);
            mx[p] = t[p];
            return;
        }
        int m = (l + r) / 2;
        if (ql <= m) range_sqrt(p << 1, l, m, ql, qr);
        if (qr > m) range_sqrt(p << 1 | 1, m + 1, r, ql, qr);
        pull(p);
    }

    long long range_sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return t[p];
        int m = (l + r) / 2;
        long long res = 0;
        if (ql <= m) res += range_sum(p << 1, l, m, ql, qr);
        if (qr > m) res += range_sum(p << 1 | 1, m + 1, r, ql, qr);
        return res;
    }
};
```

---

## 线段树合并（权值线段树合并）

常用于树上问题（如 dsu on tree 替代方案），将两棵线段树合并到一棵。

```cpp
struct MergeSegTree {
    struct Node {
        long long sum;
        int lc, rc;
        Node() : sum(0), lc(0), rc(0) {}
    };
    vector<Node> t;
    int idx;  // 当前节点总数

    MergeSegTree(int cap = 4'000'000) {
        t.resize(cap);
        idx = 0;
    }

    int new_node() { return ++idx; }

    void add(int &p, int l, int r, int pos, long long val) {
        if (!p) p = new_node();
        t[p].sum += val;
        if (l == r) return;
        int m = (l + r) / 2;
        if (pos <= m) add(t[p].lc, l, m, pos, val);
        else add(t[p].rc, m + 1, r, pos, val);
    }

    long long qry(int p, int l, int r, int ql, int qr) {
        if (!p || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return t[p].sum;
        int m = (l + r) / 2;
        return qry(t[p].lc, l, m, ql, qr) + qry(t[p].rc, m + 1, r, ql, qr);
    }

    // 合并两棵树（将 b 合并到 a 并返回新根）
    int merge(int a, int b, int l, int r) {
        if (!a || !b) return a | b;
        if (l == r) { t[a].sum += t[b].sum; return a; }
        int m = (l + r) / 2;
        t[a].lc = merge(t[a].lc, t[b].lc, l, m);
        t[a].rc = merge(t[a].rc, t[b].rc, m + 1, r);
        t[a].sum = t[a].sum + t[b].sum;
        return a;
    }

    // 按出现次数分裂（前 k 小到左树，其余到右树）
    void split(int p, int l, int r, long long k, int &lch, int &rch) {
        if (!p) { lch = rch = 0; return; }
        if (l == r) {
            lch = p; rch = 0;
            t[lch].sum = k;
            return;
        }
        lch = new_node(); rch = new_node();
        int m = (l + r) / 2;
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

## 可持久化线段树（主席树）

```cpp
struct PST {
    struct Node {
        long long sum;
        int lc, rc;
        Node() : sum(0), lc(0), rc(0) {}
    };
    vector<Node> t;
    vector<int> roots;  // 每版本根节点
    int idx;

    PST(int cap = 4'000'000) {
        t.resize(cap);
        roots.reserve(cap / 10);
        idx = 0;
        roots.push_back(newnode());
    }

    int newnode() {
        t[++idx] = Node();
        return idx;
    }

    // 从 pre 版本复制一份，在 pos 上加 val
    void add(int &cur, int pre, int l, int r, int pos, long long val) {
        cur = newnode();
        t[cur] = t[pre];
        t[cur].sum += val;
        if (l == r) return;
        int m = (l + r) / 2;
        if (pos <= m) add(t[cur].lc, t[pre].lc, l, m, pos, val);
        else add(t[cur].rc, t[pre].rc, m + 1, r, pos, val);
    }

    long long qry(int p, int l, int r, int ql, int qr) {
        if (!p || ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return t[p].sum;
        int m = (l + r) / 2;
        return qry(t[p].lc, l, m, ql, qr) + qry(t[p].rc, m + 1, r, ql, qr);
    }

    // 区间 kth（权值线段树）
    int kth(int L, int R, int l, int r, int k) {
        if (l == r) return l;
        int m = (l + r) / 2;
        int left_cnt = t[t[R].lc].sum - t[t[L].lc].sum;
        if (k <= left_cnt) return kth(t[L].lc, t[R].lc, l, m, k);
        else return kth(t[L].rc, t[R].rc, m + 1, r, k - left_cnt);
    }

    // ---------- 对外接口 ----------
    int version() { return (int)roots.size() - 1; }
    void add_version(int pos, long long val) {
        int nxt;
        add(nxt, roots.back(), 1, n, pos, val);
        roots.push_back(nxt);
    }
    long long range_sum(int ver, int l, int r) { return qry(roots[ver], 1, n, l, r); }
    int range_kth(int lver, int rver, int k) { return kth(roots[lver], roots[rver], 1, n, k); }
    int n;  // 值域大小
};
```

---

## 线段树常用技巧

| 技巧 | 说明 |
|------|------|
| **离散化** | 当值域较大时先压缩坐标，再建线段树 |
| **动态开点** | 不必预先分配 4n 空间，按需创建节点（用于值域大或线段树合并） |
| **二分查询** | 在树上二分：如查询前缀和 ≥ k 的第一个位置（`if (t[ls].sum < k) k -= ...`） |
| **扫描线** | 用线段树维护 y 轴覆盖长度，按 x 排序扫描矩形（求面积并、周长并） |
| **线段树优化建图** | 建两棵线段树（入树、出树），区间向点 / 点向区间连边，边数 O(n log n) |
| **线段树分治** | 将操作插入到线段树上对应时间区间，DFS 遍历线段树时加入 / 回退操作 |
| **标记永久化** | 懒标记不下传，查询时累加路径标记。常数小，适合可持久化 |

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 下标 | 通常从 1 开始，`n` 为序列长度 |
| 空间 | 静态线段树开 `4 * n`；动态开点根据需要开 `n log n` 或更大 |
| 懒标记 | 每次递归子区间前先 `push`，回溯后 `pull` |
| 爆 int | 区间和用 `long long`，加法 / 乘法标记注意溢出 |
| 多测试 | 每轮重新构造结构体即可，或定义 `clear()` 清空 vector |

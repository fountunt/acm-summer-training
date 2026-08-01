#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree {
    struct Node {
        ll val;
        int h;
        Node() : val(0),h(0){}
                      // ① 需要维护的信息（按题目增减字段）
        Node(ll v,int _h) : val(v),h(_h) {}
    };
    static Node merge(const Node &a, const Node &b) {
        int h = a.h+1;   // ② 区间合并规则
        if(h & 1) return Node(a.val | b.val,h);
        return Node(a.val ^ b.val,h);
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
        if (l == r) { t[p] = Node(a[l],0); return; }
        int m = (l + r) >> 1;
        build(p << 1, l, m, a);
        build(p << 1 | 1, m + 1, r, a);
        pull(p);
    }

    void upd(int p, int l, int r, int idx, long long v) {
        if (l == r) { t[p] = Node(v,0); return; }
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
    ll query() { return t[1].val; }
};


int main(){
    int n,m;
    cin >> n >> m;
    ll len = 1 << n;
    vector<ll> a(len+1);
    for(int i = 1;i <= len;i++) cin >> a[i];

    SegTree seg(len);
    seg.init(a);

    for(int i = 0;i < m;i++){
        ll p,b;
        cin >> p >> b;
        seg.set(p,b);
        cout << seg.query() << "\n";
    }
}

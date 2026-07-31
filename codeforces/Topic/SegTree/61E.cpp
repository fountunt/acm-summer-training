#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 1e6 + 5;

int n;
int a[MAXN], rk[MAXN], L[MAXN];

struct LazySeg {
    int n;
    vector<long long> t, lz;

    LazySeg(int _n) : n(_n), t(4 * _n + 5), lz(4 * _n + 5) {}

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
        if (ql > qr) return;  // 也加上保护（虽然本题不会用到）
        if (ql <= l && r <= qr) { apply(p, l, r, v); return; }
        push(p, l, r);
        int m = (l + r) / 2;
        if (ql <= m) add(p << 1, l, m, ql, qr, v);
        if (qr > m) add(p << 1 | 1, m + 1, r, ql, qr, v);
        pull(p);
    }

    long long sum(int p, int l, int r, int ql, int qr) {
        if (ql > qr) return 0;   // ✅ 关键修复：处理空区间
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
    long long range_sum(int l, int r) {
        if (l > r) return 0;   // 接口层也保护
        return sum(1, 1, n, l, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<int> b(a + 1, a + n + 1);
    sort(b.begin(), b.end());
    for (int i = 1; i <= n; i++) {
        rk[i] = lower_bound(b.begin(), b.end(), a[i]) - b.begin() + 1;
    }

    vector<int> base(n + 1, 0);

    LazySeg segL(n);
    segL.init(base);

    for (int i = 1; i <= n; i++) {
        int leq = (int)segL.range_sum(1, rk[i]);
        L[i] = (i - 1) - leq;
        segL.range_add(rk[i], rk[i], 1);
    }

    LazySeg segR(n);
    segR.init(base);

    ll ans = 0;
    for (int i = n; i >= 1; i--) {
        int R = (int)segR.range_sum(1, rk[i] - 1);  // 现在 rk[i]=1 时返回 0，不会递归
        ans += 1LL * L[i] * R;
        segR.range_add(rk[i], rk[i], 1);
    }

    cout << ans << "\n";
    return 0;
}

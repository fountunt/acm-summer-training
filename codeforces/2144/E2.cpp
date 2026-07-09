#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;

struct SegTree
{
    int n;
    vector<int> val, lazy;

    SegTree(int _n) : n(_n), val(4 * _n, 0), lazy(4 * _n, 1) {}

    void apply(int p, int v)
    {
        val[p] = (ll)val[p] * v % MOD;
        lazy[p] = (ll)lazy[p] * v % MOD;
    }

    void push(int p)
    {
        if(lazy[p] != 1)
        {
            apply(p * 2, lazy[p]);
            apply(p * 2 + 1, lazy[p]);
            lazy[p] = 1;
        }
    }

    void range_mul(int l, int r, int v, int p, int L, int R)
    {
        if(l > R || r < L) return;
        if(l <= L && R <= r) { apply(p, v); return; }
        push(p);
        int mid = (L + R) / 2;
        range_mul(l, r, v, p * 2, L, mid);
        range_mul(l, r, v, p * 2 + 1, mid + 1, R);
    }

    void point_add(int pos, int v, int p, int L, int R)
    {
        if(L == R) { val[p] = (val[p] + v) % MOD; return; }
        push(p);
        int mid = (L + R) / 2;
        if(pos <= mid) point_add(pos, v, p * 2, L, mid);
        else point_add(pos, v, p * 2 + 1, mid + 1, R);
    }

    int point_query(int pos, int p, int L, int R)
    {
        if(L == R) return val[p];
        push(p);
        int mid = (L + R) / 2;
        if(pos <= mid) return point_query(pos, p * 2, L, mid);
        else return point_query(pos, p * 2 + 1, mid + 1, R);
    }

    void range_mul(int l, int r, int v) { if(l <= r) range_mul(l, r, v, 1, 0, n); }
    void point_add(int pos, int v) { point_add(pos, v, 1, 0, n); }
    int point_query(int pos) { return point_query(pos, 1, 0, n); }
};

void solve()
{
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for(int i = 1;i <= n;i++) cin >> a[i];

    // ---- 提取 L (从左看) 和 R (从右看) 的值序列 ----
    vector<int> L(n + 1), R(n + 1);
    int cntL = 0, cntR = 0;

    int cur = 0;
    for(int i = 1;i <= n;i++)
        if(a[i] > cur) L[++cntL] = a[i], cur = a[i];

    cur = 0;
    for(int i = n;i >= 1;i--)
        if(a[i] > cur) R[++cntR] = a[i], cur = a[i];

    int M = L[cntL];

    // ---- Lw[i]：左 DP 中，位置 i 作为最后一个左标记的方案数 ----
    vector<ll> Lw(n + 1, 0), Rw(n + 1, 0);

    // ---- 左 DP：用线段树维护 dp[j] ----
    {
        SegTree seg(cntL);
        seg.point_add(0, 1);

        for(int i = 1;i <= n;i++)
        {
            if(cntL > 0) Lw[i] = seg.point_query(cntL - 1);

            int pos = lower_bound(L.begin() + 1, L.begin() + cntL + 1, a[i]) - L.begin();

            if(pos <= cntL)
            {
                seg.range_mul(pos, cntL, 2);

                if(L[pos] == a[i])
                {
                    ll val = seg.point_query(pos - 1);
                    seg.point_add(pos, val);
                }
            }
        }
    }

    // ---- 右 DP：对称，从右往左扫 ----
    {
        SegTree seg(cntR);
        seg.point_add(0, 1);

        for(int i = n;i >= 1;i--)
        {
            if(cntR > 0) Rw[i] = seg.point_query(cntR - 1);

            int pos = lower_bound(R.begin() + 1, R.begin() + cntR + 1, a[i]) - R.begin();

            if(pos <= cntR)
            {
                seg.range_mul(pos, cntR, 2);

                if(R[pos] == a[i])
                {
                    ll val = seg.point_query(pos - 1);
                    seg.point_add(pos, val);
                }
            }
        }
    }

    // ---- 在 M 处合并 ----
    ll ans = 0, sum = 0;
    for(int i = 1;i <= n;i++)
    {
        if(a[i] == M)
        {
            ans = (ans + (sum + Lw[i]) % MOD * Rw[i]) % MOD;
            sum = (sum * 2 + Lw[i]) % MOD;
        }
        else
        {
            sum = sum * 2 % MOD;
        }
    }

    cout << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}

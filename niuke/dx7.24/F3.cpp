#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Vec = vector<ll>;
const int MOD = 998244353;

ll modpow(ll a, ll e) {
    ll r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

// ----------求出递推公式----------
vector<ll> berlekamp_massey(const vector<ll>& s) {
    vector<ll> C(1, 1), B(1, 1);
    ll L = 0, m = 1, b = 1;
    for (int n = 0; n < (int)s.size(); ++n) {
        ll d = 0;
        for (int i = 0; i <= L; ++i)
            d = (d + C[i] * s[n - i]) % MOD;
        if (d == 0) {
            ++m;
        } else {
            vector<ll> T = C;
            ll coef = d * modpow(b, MOD - 2) % MOD;
            if (C.size() < B.size() + m) C.resize(B.size() + m, 0);
            for (int j = 0; j < (int)B.size(); ++j)
                C[j + m] = (C[j + m] - coef * B[j]) % MOD;
            if (2 * L <= n) {
                L = n - L + 1;
                B = T;
                b = d;
                m = 1;
            } else {
                ++m;
            }
        }
    }
    // C[0] = 1, 递推式: s[n] + C[1]*s[n-1] + ... + C[L]*s[n-L] = 0
    vector<ll> rec(L);
    for (int i = 1; i <= L; ++i)
        rec[i - 1] = (MOD - C[i]) % MOD;
    return rec;
}

// ---------- 多项式乘法（模特征多项式） ----------
// rec 为递推系数，特征多项式: x^d = rec[0]*x^{d-1} + rec[1]*x^{d-2} + ... + rec[d-1]
vector<ll> poly_mul(const vector<ll>& a, const vector<ll>& b, const vector<ll>& rec) {
    int d = rec.size();
    vector<ll> tmp(2 * d - 1, 0);
    for (int i = 0; i < d; ++i) if (a[i])
        for (int j = 0; j < d; ++j) if (b[j])
            tmp[i + j] = (tmp[i + j] + a[i] * b[j]) % MOD;
    // 约化：从高次到低次
    for (int i = 2 * d - 2; i >= d; --i) {
        if (tmp[i] == 0) continue;
        ll coef = tmp[i];
        // x^i = rec[0]*x^{i-1} + rec[1]*x^{i-2} + ... + rec[d-1]*x^{i-d}
        for (int k = 1; k <= d; ++k)
            tmp[i - k] = (tmp[i - k] + coef * rec[k - 1]) % MOD;
    }
    vector<ll> res(d);
    for (int i = 0; i < d; ++i) res[i] = tmp[i];
    return res;
}

// ---------- 线性递推求第 n 项 ----------
// init[0..d-1] 为初始值，rec 为递推系数，求 a[n]
ll linear_nth(const vector<ll>& init, const vector<ll>& rec, ll n) {
    int d = rec.size();
    if (n < (ll)init.size()) return init[n];
    if (d == 0) return 0;          // 不会出现
    if (d == 1) {                  // 一阶递推，直接快速幂
        return init[0] * modpow(rec[0], n) % MOD;
    }
    // 计算 x^n 模特征多项式
    vector<ll> res(d, 0), base(d, 0);
    res[0] = 1;          // x^0
    base[1] = 1;         // x^1 (d>1)
    while (n) {
        if (n & 1) res = poly_mul(res, base, rec);
        base = poly_mul(base, base, rec);
        n >>= 1;
    }
    // 答案 = sum_{i=0}^{d-1} res[i] * init[i]
    ll ans = 0;
    for (int i = 0; i < d; ++i)
        ans = (ans + res[i] * init[i]) % MOD;
    return ans;
}

int n;
vector<vector<int>> states;

void dfs(int pos, vector<int>& cur)
{
    if (pos == n)
    {
        states.push_back(cur);
        return;
    }
    for (int i = 0; i <= 2; ++i)
    {
        if (pos > 0 && i == cur[pos - 1]) continue;
        cur.push_back(i);
        dfs(pos + 1, cur);
        cur.pop_back();
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> n >> m;

    vector<int> cur;
    dfs(0, cur);
    int S = states.size();

    vector<vector<int>> succ(S);
    for (int u = 0; u < S; ++u)
    {
        for (int v = 0; v < S; ++v)
        {
            bool ok = true;
            for (int i = 0; i < n; ++i)
            {
                if (states[u][i] == states[v][i])
                {
                    ok = false;
                    break;
                }
            }
            if (ok) succ[u].push_back(v);
        }
    }

    int need = 2 * S + 5;
    vector<ll> seq(need);
    Vec dp(S, 1);
    ll sum = 0;
    for (int i = 0; i < S; ++i) sum = (sum + dp[i]) % MOD;
    seq[0] = sum;

    auto vec_mul_sparse = [&](const Vec& v)
    {
        Vec res(S, 0);
        for (int u = 0; u < S; ++u)
        {
            if (v[u] == 0) continue;
            for (int v_idx : succ[u])
            {
                res[v_idx] += v[u];
                res[v_idx] %= MOD;
            }
        }
        return res;
    };

    for (int k = 1; k < need; ++k)
    {
        dp = vec_mul_sparse(dp);
        sum = 0;
        for (int i = 0; i < S; ++i) sum = (sum + dp[i]) % MOD;
        seq[k] = sum;
    }

    ll target = m - 1;
    if (target < need)
    {
        cout << seq[target] << '\n';
        return 0;
    }

    vector<ll> rec = berlekamp_massey(seq);
    int d = rec.size();
    vector<ll> init(seq.begin(), seq.begin() + d);
    ll ans = linear_nth(init, rec, target);
    cout << ans << '\n';

    return 0;
}

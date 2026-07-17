#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 998244353;

// 快速幂
ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

// 模逆元
ll modInverse(ll n) {
    return power(n, MOD - 2);
}

void solve()
{
    int m;
    if (!(cin >> m)) return;

    // 1-based indexing, s[1] 到 s[m], s[m+1] 默认为 0
    vector<ll> s(m + 2, 0);
    for (int i = 1; i <= m; i++) {
        cin >> s[i];
    }

    // 降序排序
    sort(s.begin() + 1, s.begin() + m + 1, greater<ll>());

    ll C = 0;
    ll Y = 1; // 维护后半部分的乘积

    for (int k = m; k >= 2; --k) {
        ll C_k = 0;
        if (k % 2 == 0) {
            ll num = k;
            ll den = (2 * (k - 1)) % MOD;
            C_k = (num * modInverse(den)) % MOD;
        } else {
            C_k = modInverse(2);
        }

        ll term1 = power(s[k] % MOD, k - 1);
        ll term2 = power(s[k + 1] % MOD, k - 1);
        ll diff = (term1 - term2 + MOD) % MOD;

        ll current_term = (C_k * Y) % MOD;
        current_term = (current_term * diff) % MOD;

        C = (C + current_term) % MOD;

        // 更新 Y 为下一个阶段做准备
        Y = (Y * (s[k] % MOD)) % MOD;
    }

    cout << C << "\n";
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    // 本题只有单组数据
    solve();

    return 0;
}

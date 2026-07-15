#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
const int MOD = 998244353;
const int G = 3;

int modpow(int a, int b)
{
    int res = 1;
    a %= MOD;
    while (b)
    {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void ntt(vector<int>& a, bool invert)
{
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1)
    {
        int wlen = modpow(G, (MOD - 1) / len);
        if (invert) wlen = modpow(wlen, MOD - 2);
        for (int i = 0; i < n; i += len)
        {
            int w = 1;
            for (int j = 0; j < len / 2; j++)
            {
                int u = a[i + j];
                int v = a[i + j + len / 2] * w % MOD;
                a[i + j] = (u + v) % MOD;
                a[i + j + len / 2] = (u - v + MOD) % MOD;
                w = w * wlen % MOD;
            }
        }
    }
    if (invert)
    {
        int inv_n = modpow(n, MOD - 2);
        for (int &x : a) x = x * inv_n % MOD;
    }
}

vector<int> convolution(vector<int> a, vector<int> b)
{
    int n = 1;
    while (n < (int)(a.size() + b.size() - 1)) n <<= 1;
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
    ntt(a, true);
    return a;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    int M = n / 3;

    vector<int> fact(n + 1), invFact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i % MOD;
    invFact[n] = modpow(fact[n], MOD - 2);
    for (int i = n - 1; i >= 0; i--) invFact[i] = invFact[i+1] * (i+1) % MOD;

    vector<int> pow26(n + 1);
    pow26[0] = 1;
    for (int i = 1; i <= n; i++) pow26[i] = pow26[i-1] * 26 % MOD;

    auto C = [&](int N, int K) -> int
    {
        if (K < 0 || K > N) return 0;
        return fact[N] * invFact[K] % MOD * invFact[N - K] % MOD;
    };

    vector<int> g(M + 1);
    for (int k = 0; k <= M; k++)
    {
        g[k] = C(n - 2 * k, k) * pow26[n - 3 * k] % MOD;
    }

    vector<int> A(M + 1), B(M + 1);
    for (int i = 0; i <= M; i++)
    {
        A[i] = g[i] * fact[i] % MOD;
        B[i] = (i % 2 == 0) ? invFact[i] : (MOD - invFact[i]) % MOD;
    }

    reverse(A.begin(), A.end());

    vector<int> conv = convolution(A, B);

    vector<int> ans(n + 1, 0);
    for (int k = 0; k <= M; k++)
    {
        ans[k] = conv[M - k] * invFact[k] % MOD;
    }

    for (int i = 0; i <= n; i++)
    {
        cout << ans[i] << (i == n ? '\n' : ' ');
    }

    return 0;
}

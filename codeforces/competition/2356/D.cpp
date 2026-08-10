#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;
const int MAXN = 1000005;
long long fact[MAXN], invfact[MAXN];

long long modpow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

long long C(int nn, int kk) {
    if (kk < 0 || kk > nn) return 0;
    return fact[nn] * invfact[kk] % MOD * invfact[nn - kk] % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    fact[0] = 1;
    for (int i = 1; i < MAXN; ++i) fact[i] = fact[i - 1] * i % MOD;
    invfact[MAXN - 1] = modpow(fact[MAXN - 1], MOD - 2);
    for (int i = MAXN - 1; i >= 1; --i) invfact[i - 1] = invfact[i] * i % MOD;

    int t;
    cin >> t;
    while (t--) {
        int n;
        string s;
        cin >> n >> s;
        bool has0 = false, has1 = false;
        for (char ch : s) {
            if (ch == '0') has0 = true;
            else has1 = true;
        }
        if (!has0 || !has1) {
            cout << 1 << '\n';
            continue;
        }
        long long x = 0, y = 0, z = 0, w = 0;
        for (int i = 0; i + 1 < n; ++i) {
            if (s[i] == s[i + 1]) {
                if (s[i] == '0') ++x;
                else ++y;
            } else {
                if (s[i] == '0') ++z;
                else ++w;
            }
        }
        long long ans;
        if (s[0] == '0' && s[n - 1] == '0') {
            ans = C((int)(x + z), (int)z) * C((int)(y + z - 1), (int)(z - 1)) % MOD;
        } else if (s[0] == '1' && s[n - 1] == '1') {
            ans = C((int)(x + z - 1), (int)(z - 1)) * C((int)(y + z), (int)z) % MOD;
        } else if (s[0] == '0' && s[n - 1] == '1') {
            ans = C((int)(x + z - 1), (int)(z - 1)) * C((int)(y + z - 1), (int)(z - 1)) % MOD;
        } else {
            ans = C((int)(x + w - 1), (int)(w - 1)) * C((int)(y + w - 1), (int)(w - 1)) % MOD;
        }
        cout << ans << '\n';
    }
    return 0;
}

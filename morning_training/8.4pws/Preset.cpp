#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 998244353;

ll powmod(ll b, ll e) {
    b %= MOD;
    ll r = 1;
    while (e > 0) {
        if (e & 1) r = r * b % MOD;
        b = b * b % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M;
    int Q;
    cin >> N >> M >> Q;
    for (int j = 0; j < Q; j++) {
        ll X, Y;
        cin >> X >> Y;   // 与答案无关，读掉即可
    }
    cout << powmod(M, N - Q) << '\n';
    return 0;
}

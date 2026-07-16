#include <bits/stdc++.h>
using namespace std;
#define int long long
const int p = 998244353;

int mod_pow(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b >>= 1;
    }
    return res;
}
int inv(int x) { return mod_pow(x, p - 2); }

void solve() {
    int n, m;
    cin >> n >> m;
    // 读入图结构（拓扑信息已知但不知道边权）
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
    }

    if (n == 1) {
        cout << "!" << endl;
        int k; cin >> k;
        cout << 1 << endl;  // f(1,1,k) = 1
        return;
    }

    // y[i] = f(1,n,i)  for i = 0,1,...,n-1
    vector<int> y(n);
    y[0] = 0;  // f(1,n,0) = 0  (1 ≠ n)

    for (int c = 1; c < n; c++) {
        cout << "? 1 " << n << " " << c << endl;
        cin >> y[c];
    }

    // 告知交互器已准备好
    cout << "!" << endl;
    int k;
    cin >> k;

    // 如果 k 已经在查询范围内，直接返回
    if (k < n) {
        cout << y[k] << endl;
        return;
    }

    // --- 拉格朗日插值（等距点 0,1,...,n-1）---
    int d = n - 1;

    // 阶乘与逆元
    vector<int> fact(d + 1), inv_fact(d + 1);
    fact[0] = 1;
    for (int i = 1; i <= d; i++) fact[i] = fact[i - 1] * i % p;
    inv_fact[d] = inv(fact[d]);
    for (int i = d; i >= 1; i--) inv_fact[i - 1] = inv_fact[i] * i % p;

    // pref[i] = Π_{j=0}^{i-1} (k - j)  前缀积
    // suff[i] = Π_{j=i+1}^{d} (k - j)  后缀积
    vector<int> pref(d + 1), suff(d + 1);
    pref[0] = 1;
    for (int i = 1; i <= d; i++)
        pref[i] = pref[i - 1] * ((k - (i - 1) + p) % p) % p;

    suff[d] = 1;
    for (int i = d - 1; i >= 0; i--)
        suff[i] = suff[i + 1] * ((k - (i + 1) + p) % p) % p;

    int ans = 0;
    for (int i = 0; i <= d; i++) {
        int num = pref[i] * suff[i] % p;           // Π_{j≠i} (k - j)
        int den = fact[i] * fact[d - i] % p;       // i! * (d-i)!
        if ((d - i) & 1) den = p - den;            // (-1)^{d-i}
        ans = (ans + y[i] * num % p * inv(den)) % p;
    }

    cout << ans << endl;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

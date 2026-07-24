# 线性递推（BM + 多项式模幂）

## 概念

对于 **k 阶线性递推** $a_n = c_1 a_{n-1} + \cdots + c_k a_{n-k}$：

1. **Berlekamp–Massey**：给定序列前若干项，自动求出最短递推系数
2. **多项式模幂**：用特征多项式约化 $x^n \bmod P(x)$，再点乘初项得到 $a_n$

复杂度 $O(k^2 \log n)$，$k \le 100$ 时比矩阵快速幂（$O(k^3 \log n)$）更优。

---

## 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

constexpr int MOD = 1e9 + 7;

ll modpow(ll a, ll b) {
    ll r = 1;
    while (b) {
        if (b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

// ====== Berlekamp–Massey ======
// 输入 s[0..n-1]，返回递推系数 rec[0..d-1]
// a[n] = rec[0]*a[n-1] + rec[1]*a[n-2] + ... + rec[d-1]*a[n-d]
vector<ll> berlekamp_massey(const vector<ll> &s) {
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
            if (C.size() < B.size() + m)
                C.resize(B.size() + m, 0);
            for (int j = 0; j < (int)B.size(); ++j)
                C[j + m] = (C[j + m] - coef * B[j]) % MOD;
            if (2 * L <= n) {
                L = n + 1 - L;
                B = T;
                b = d;
                m = 1;
            } else {
                ++m;
            }
        }
    }
    vector<ll> rec(L);
    for (int i = 0; i < L; ++i)
        rec[i] = (MOD - C[i + 1]) % MOD;
    return rec;
}

// ====== 多项式模特征多项式 ======
vector<ll> poly_mul(const vector<ll> &a, const vector<ll> &b, const vector<ll> &rec) {
    int d = rec.size();
    vector<ll> tmp(2 * d - 1, 0);
    for (int i = 0; i < d; ++i) if (a[i])
        for (int j = 0; j < d; ++j) if (b[j])
            tmp[i + j] = (tmp[i + j] + a[i] * b[j]) % MOD;
    for (int i = 2 * d - 2; i >= d; --i) {
        if (tmp[i] == 0) continue;
        ll coef = tmp[i];
        for (int k = 1; k <= d; ++k)
            tmp[i - k] = (tmp[i - k] + coef * rec[k - 1]) % MOD;
    }
    vector<ll> res(d);
    for (int i = 0; i < d; ++i) res[i] = tmp[i];
    return res;
}

// ====== 求递推第 n 项 ======
ll linear_nth(const vector<ll> &init, const vector<ll> &rec, ll n) {
    int d = rec.size();
    if (n < (ll)init.size()) return (init[n] % MOD + MOD) % MOD;
    if (d == 0) return 0;
    if (d == 1) return init[0] * modpow(rec[0], n) % MOD;  // 等比数列

    vector<ll> res(d, 0), base(d, 0);
    res[0] = 1;         // res = x^0
    base[1] = 1;        // base = x^1
    while (n) {
        if (n & 1) res = poly_mul(res, base, rec);
        base = poly_mul(base, base, rec);
        n >>= 1;
    }
    ll ans = 0;
    for (int i = 0; i < d; ++i)
        ans = (ans + res[i] * init[i]) % MOD;
    return (ans + MOD) % MOD;
}

// ====== 便捷版：已知序列，自动找递推 ======
ll nth_from_sequence(const vector<ll> &seq, ll n) {
    auto rec = berlekamp_massey(seq);
    return linear_nth(seq, rec, n);
}
```

---

## 使用方式

| 场景 | 代码 |
|------|------|
| **已知递推系数** | `linear_nth({init...}, {rec...}, n)` |
| **未知递推，打表能出** | `nth_from_sequence({序列前 2k 项}, n)` |
| **BM 单独用** | `auto rec = berlekamp_massey(seq);` |

### 示例

```cpp
// 斐波那契: 打表 → 自动递推
vector<ll> seq = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
cout << nth_from_sequence(seq, (ll)1e18) << "\n";

// 或手动给系数
cout << linear_nth({0, 1}, {1, 1}, (ll)1e18) << "\n";

// 三阶: a[n] = 2*a[n-1] + 3*a[n-2] + a[n-3]
cout << linear_nth({1, 2, 5}, {2, 3, 1}, (ll)1e15) << "\n";
```

---

## 要点

- 提供序列给 BM 时，长度至少 $\ge 2d$ 才稳定
- $d = 1$ 时特判为等比数列
- 模数需为质数（BM 需要求逆元）
- 复杂度 $O(d^2 \log n)$，$d \le 2000$ 时完全可用

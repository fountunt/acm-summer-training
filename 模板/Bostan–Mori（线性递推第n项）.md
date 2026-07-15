# Bostan–Mori（线性递推第 n 项）

## 概念

给定 **k 阶线性递推**：$a_n = c_1 a_{n-1} + c_2 a_{n-2} + \cdots + c_k a_{n-k}$（$n \ge k$），以及前 $k$ 项 $a_0 \sim a_{k-1}$，求第 $n$ 项（$n$ 可高达 $10^{18}$）。

**核心思想：** 生成函数 $A(x) = \sum_{i\ge 0} a_i x^i = \dfrac{P(x)}{Q(x)}$，其中

- $Q(x) = 1 - c_1 x - c_2 x^2 - \cdots - c_k x^k$
- $P(x) = (A(x) \cdot Q(x)) \bmod x^k$（由前 $k$ 项计算）

用 Bostan–Mori 算法可在 $O(k^2 \log n)$（朴素乘法）或 $O(k \log k \log n)$（NTT）内求出 $a_n = [x^n] \dfrac{P(x)}{Q(x)}$。

---

## 算法步骤

1. 由递推系数 $c[1..k]$ 构造 $Q(x)$
2. 由初项 $a[0..k-1]$ 与 $Q$ 计算 $P(x)$（卷积截断）
3. 分治：

```
while n > 0:
    Q_minus(x) := Q 中奇数次项取反
    U(x) := P(x) * Q_minus(x)
    V(x) := Q(x) * Q_minus(x)
    if n % 2 == 0:
        P(x) := U 的偶次项
    else:
        P(x) := U 的奇次项
    Q(x) := V 的偶次项
    n /= 2
return P(0) / Q(0)
```

---

## 代码（二合一版）

可通过宏 `BM_USE_NTT` 切换：注释掉则用朴素版，取消注释则用 NTT 加速版。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

constexpr int MOD = 998244353;

ll modpow(ll a, ll b) {
    ll r = 1;
    while (b) {
        if (b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

// ---------- NTT（仅在 BM_USE_NTT 下编译）----------
#ifdef BM_USE_NTT

constexpr int G = 3;

void ntt(vector<ll> &a, bool inv) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll wlen = modpow(G, (MOD - 1) / len);
        if (inv) wlen = modpow(wlen, MOD - 2);
        for (int i = 0; i < n; i += len) {
            ll w = 1;
            for (int j = 0; j < len / 2; ++j) {
                ll u = a[i + j];
                ll v = a[i + j + len / 2] * w % MOD;
                a[i + j] = (u + v) % MOD;
                a[i + j + len / 2] = (u - v + MOD) % MOD;
                w = w * wlen % MOD;
            }
        }
    }
    if (inv) {
        ll inv_n = modpow(n, MOD - 2);
        for (auto &x : a) x = x * inv_n % MOD;
    }
}

vector<ll> mul(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    int sz = 1;
    while (sz < (int)(a.size() + b.size() - 1)) sz <<= 1;
    vector<ll> fa(sz, 0), fb(sz, 0);
    copy(a.begin(), a.end(), fa.begin());
    copy(b.begin(), b.end(), fb.begin());
    ntt(fa, false); ntt(fb, false);
    for (int i = 0; i < sz; ++i) fa[i] = fa[i] * fb[i] % MOD;
    ntt(fa, true);
    fa.resize(a.size() + b.size() - 1);
    return fa;
}

#else
// ---------- 朴素乘法 ----------

vector<ll> mul(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    vector<ll> c(a.size() + b.size() - 1, 0);
    for (int i = 0; i < (int)a.size(); ++i)
        for (int j = 0; j < (int)b.size(); ++j)
            c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
    return c;
}

#endif
// ---------- 公共辅助函数 ----------

vector<ll> trunc(const vector<ll> &a, int lim) {
    if ((int)a.size() <= lim) return a;
    return vector<ll>(a.begin(), a.begin() + lim);
}

vector<ll> Q_minus(const vector<ll> &Q) {
    auto res = Q;
    for (int i = 1; i < (int)res.size(); i += 2)
        res[i] = (MOD - res[i]) % MOD;
    return res;
}

vector<ll> take_parity(const vector<ll> &a, int parity) {
    vector<ll> res;
    for (int i = parity; i < (int)a.size(); i += 2)
        res.push_back(a[i]);
    return res;
}

// ---------- Bostan–Mori ----------

/**
 * @brief 返回线性递推的第 n 项 a_n
 *
 * @param a  初项 a[0..k-1]
 * @param c  递推系数 c[1..k]（a_n = c1*a_{n-1} + ... + ck*a_{n-k}）
 * @param n  下标（0-based）
 * @return a_n % MOD
 */
ll linear_recurrence(const vector<ll> &a, const vector<ll> &c, ll n) {
    int k = (int)c.size();
    if (n < (int)a.size()) return a[n] % MOD;

    // Q(x) = 1 - c1*x - c2*x^2 - ... - ck*x^k
    vector<ll> Q(k + 1, 0);
    Q[0] = 1;
    for (int i = 1; i <= k; ++i)
        Q[i] = (MOD - c[i - 1]) % MOD;

    // P(x) = (A(x) * Q(x)) mod x^k
    vector<ll> A = trunc(a, k);
    vector<ll> P = trunc(mul(A, Q), k);

    // Bostan–Mori 二分
    while (n > 0) {
        vector<ll> Qm = Q_minus(Q);
        vector<ll> U = mul(P, Qm);
        vector<ll> V = mul(Q, Qm);

        if (n & 1)      P = take_parity(U, 1);
        else            P = take_parity(U, 0);
        Q = take_parity(V, 0);
        n >>= 1;
    }

    return P.empty() ? 0 : P[0] * modpow(Q[0], MOD - 2) % MOD;
}
```

---

## 使用示例

```cpp
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    // 斐波那契：F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)
    vector<ll> a = {0, 1};
    vector<ll> c = {1, 1};
    cout << linear_recurrence(a, c, (ll)1e18) << "\n";

    // 三阶递推：a_n = 2*a_{n-1} + 3*a_{n-2} + a_{n-3}, 初项 {1,2,5}
    vector<ll> a2 = {1, 2, 5};
    vector<ll> c2 = {2, 3, 1};
    cout << linear_recurrence(a2, c2, (ll)1e15) << "\n";
}
```

---

## 性能参考

| 递推阶数 $k$ | 朴素版 $O(k^2 \log n)$ | NTT 版 $O(k \log k \log n)$ |
|-------------|-----------------------|---------------------------|
| $10$        | $\approx$ 0.1 ms     | $\approx$ 0.2 ms          |
| $100$       | $\approx$ 0.3 ms     | $\approx$ 0.3 ms          |
| $1000$      | $\approx$ 15 ms      | $\approx$ 1 ms            |
| $10^5$      | 不可用               | $\approx$ 150 ms          |

---

## 切换方式

```cpp
// #define BM_USE_NTT          // ← 取消注释启用 NTT 加速版（需 k 较大时再用）
#include "Bostan–Mori（线性递推第n项）.md"
```

- **$k \le 500$** → 默认朴素版（常数小，速度快）
- **$k > 500$** → 定义 `BM_USE_NTT` 启用 NTT 版

---

## 注意

1. 模数 `998244353`，原根 `3`，NTT 最大长度 $2^{23}$。
2. `mul` 在 NTT 版中自动补齐到 2 的幂次；朴素版为 $O(nm)$ 直接卷积。
3. 两个版本共用同一接口 `linear_recurrence(a, c, n)`，切换只需一个宏。

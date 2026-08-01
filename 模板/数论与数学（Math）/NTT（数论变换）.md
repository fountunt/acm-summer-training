# NTT（数论变换）$O(n \log n)$

## 概念

NTT 是 FFT 在有限域 $\mathbb{Z}_p$ 上的模拟，用原根的整数次幂代替单位复根，避免浮点误差。

### 常用模数与原根

| 模数 | 二进制形式 | 最大长度 | 原根 |
|------|-----------|---------|------|
| 998244353 | $119 \cdot 2^{23} + 1$ | $2^{23}$ | 3 |
| 1004535809 | $479 \cdot 2^{21} + 1$ | $2^{21}$ | 3 |
| 469762049 | $7 \cdot 2^{26} + 1$ | $2^{26}$ | 3 |

---

## 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------- 配置 ----------
constexpr int MOD = 998244353;   // 模数（质数）
constexpr int G = 3;              // 原根

ll modpow(ll a, ll b) {
    ll r = 1;
    while (b) {
        if (b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

// ---------- NTT ----------
/**
 * @brief 数论变换（迭代版，Cooley–Tukey）
 *
 * @param a  输入/输出多项式系数（长度必须为 2 的幂）
 * @param inv  false → 正变换, true → 逆变换
 */
void ntt(vector<ll> &a, bool inv) {
    int n = (int)a.size();

    // bit-reversal 重排
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    // 蝶形运算
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

// ---------- 多项式运算 ----------

/** 多项式乘法 */
vector<ll> mul(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    int sz = 1;
    while (sz < (int)(a.size() + b.size() - 1)) sz <<= 1;
    vector<ll> fa(sz, 0), fb(sz, 0);
    copy(a.begin(), a.end(), fa.begin());
    copy(b.begin(), b.end(), fb.begin());
    ntt(fa, false);
    ntt(fb, false);
    for (int i = 0; i < sz; ++i) fa[i] = fa[i] * fb[i] % MOD;
    ntt(fa, true);
    fa.resize(a.size() + b.size() - 1);
    return fa;
}

/** 多项式乘法（三点合并优化，用于拆系数） */
vector<ll> mul_3mod(const vector<ll> &a, const vector<ll> &b,
                    int mod1 = 998244353, int mod2 = 1004535809, int mod3 = 469762049) {
    // 略：用于任意模数 NTT（MTT），需要三个模数 + CRT 合并
    // 适用于模数不是 NTT-friendly 的场景
    return mul(a, b);  // 默认走当前 MOD
}

/** 多项式求逆：B(x) = 1/A(x) (mod x^n) */
vector<ll> poly_inv(const vector<ll> &a, int n) {
    assert(a[0] != 0);
    vector<ll> b = {modpow(a[0], MOD - 2)};
    int cur = 1;
    while (cur < n) {
        cur <<= 1;
        vector<ll> a_cut(a.begin(), a.begin() + min((int)a.size(), cur));
        vector<ll> t = mul(mul(b, b), a_cut);
        b.resize(cur);
        for (int i = 0; i < cur; ++i) {
            b[i] = (2 * b[i] - (i < (int)t.size() ? t[i] : 0) + MOD) % MOD;
        }
    }
    b.resize(n);
    return b;
}

/** 多项式除法：A / B，返回 {quotient, remainder} */
pair<vector<ll>, vector<ll>> poly_div(const vector<ll> &a, const vector<ll> &b) {
    int n = (int)a.size() - 1, m = (int)b.size() - 1;
    if (n < m) return {{}, a};
    auto ra = a, rb = b;
    reverse(ra.begin(), ra.end());
    reverse(rb.begin(), rb.end());
    auto inv_rb = poly_inv(rb, n - m + 1);
    auto q = mul(ra, inv_rb);
    q.resize(n - m + 1);
    reverse(q.begin(), q.end());
    vector<ll> r = mul(b, q);
    r.resize(m);
    for (int i = 0; i < m; ++i)
        r[i] = (a[i] - r[i] + MOD) % MOD;
    while (!r.empty() && r.back() == 0) r.pop_back();
    return {q, r};
}

/** 多项式求导 */
vector<ll> poly_deriv(const vector<ll> &a) {
    if (a.empty()) return {};
    vector<ll> res(a.size() - 1);
    for (int i = 1; i < (int)a.size(); ++i)
        res[i - 1] = a[i] * i % MOD;
    return res;
}

/** 多项式积分 */
vector<ll> poly_integ(const vector<ll> &a) {
    vector<ll> res(a.size() + 1, 0);
    for (int i = 0; i < (int)a.size(); ++i)
        res[i + 1] = a[i] * modpow(i + 1, MOD - 2) % MOD;
    return res;
}

/** 多项式 ln：ln(A(x)) (a[0] = 1) */
vector<ll> poly_ln(const vector<ll> &a, int n) {
    assert(a[0] == 1);
    auto inv_a = poly_inv(a, n);
    auto deriv_a = poly_deriv(a);
    auto res = mul(deriv_a, inv_a);
    res.resize(n);
    res = poly_integ(res);
    res.resize(n);
    return res;
}

/** 多项式 exp：exp(A(x)) (a[0] = 0) */
vector<ll> poly_exp(const vector<ll> &a, int n) {
    assert(a[0] == 0);
    vector<ll> b = {1};
    int cur = 1;
    while (cur < n) {
        cur <<= 1;
        vector<ll> ln_b = poly_ln(b, cur);
        vector<ll> a_cut(a.begin(), a.begin() + min((int)a.size(), cur));
        vector<ll> delta(cur);
        for (int i = 0; i < cur; ++i) {
            ll rhs = (i < (int)a_cut.size() ? a_cut[i] : 0);
            delta[i] = (rhs - (i < (int)ln_b.size() ? ln_b[i] : 0) + MOD) % MOD;
        }
        delta[0] = (delta[0] + 1) % MOD;
        b = mul(b, delta);
        b.resize(cur);
    }
    b.resize(n);
    return b;
}

/** 多项式快速幂：A(x)^k (mod x^n) */
vector<ll> poly_pow(const vector<ll> &a, ll k, int n) {
    int shift = 0;
    while (shift < (int)a.size() && a[shift] == 0) ++shift;
    if (shift == (int)a.size()) {   // A(x) == 0
        vector<ll> res(n, 0);
        if (k == 0) res[0] = 1;     // 0^0 = 1
        return res;
    }
    ll a0 = a[shift];
    ll inv_a0 = modpow(a0, MOD - 2);
    ll pow_a0 = modpow(a0, k);

    // 提取 A'(x) = a[shift..] / a0
    vector<ll> b(a.size() - shift);
    for (int i = 0; i < (int)b.size(); ++i)
        b[i] = a[i + shift] * inv_a0 % MOD;

    // ln(A') * k → exp
    b = poly_ln(b, n);
    for (auto &x : b) x = x * (k % MOD) % MOD;
    b = poly_exp(b, n);

    // 平移 x^{shift*k}
    vector<ll> res(n, 0);
    ll offset = shift * k;
    if (offset >= n) return res;    // 超出截断范围
    for (int i = 0; i + offset < n && i < (int)b.size(); ++i)
        res[i + offset] = b[i] * pow_a0 % MOD;
    return res;
}

// ---------- 辅助函数 ----------

/** 截断到前 n 项 */
vector<ll> trunc(const vector<ll> &a, int n) {
    if ((int)a.size() <= n) return a;
    return vector<ll>(a.begin(), a.begin() + n);
}

/** 带模加法 */
ll add(ll a, ll b) { return (a += b) >= MOD ? a - MOD : a; }
ll sub(ll a, ll b) { return (a -= b) < 0 ? a + MOD : a; }
```

---

## 使用示例

```cpp
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    // ---- 1. 基本乘法 ----
    vector<ll> A = {1, 2, 1};       // 1 + 2x + x^2
    vector<ll> B = {1, 1};          // 1 + x
    auto C = mul(A, B);             // 1 + 3x + 3x^2 + x^3
    for (ll x : C) cout << x << " ";  // 1 3 3 1
    cout << "\n";

    // ---- 2. 多项式求逆 ----
    vector<ll> F = {1, 1, 0, 0, 0}; // 1 + x
    auto invF = poly_inv(F, 5);     // 1 - x + x^2 - x^3 + x^4 (mod x^5)
    for (ll x : invF) cout << x << " ";
    cout << "\n";

    // ---- 3. 多项式 ln ----
    vector<ll> L = {1, 1, 0, 0, 0};
    auto lnL = poly_ln(L, 5);
    for (ll x : lnL) cout << x << " ";  // x - x^2/2 + x^3/3 - x^4/4
    cout << "\n";

    // ---- 4. 多项式 exp ----
    vector<ll> E = {0, 1, 0, 0, 0};
    auto expE = poly_exp(E, 5);     // 1 + x + x^2/2 + x^3/6 + x^4/24
    for (ll x : expE) cout << x << " ";
    cout << "\n";

    // ---- 5. 多项式快速幂 ----
    vector<ll> P = {0, 1, 0, 0, 0};
    auto powP = poly_pow(P, 3, 5);  // (x)^3 = x^3
    for (ll x : powP) cout << x << " ";
    cout << "\n";
}
```

---

## 复杂度

| 操作 | 时间 |
|------|------|
| `ntt` | $O(n \log n)$ |
| `mul` | $O(n \log n)$ |
| `poly_inv` | $O(n \log n)$ |
| `poly_ln` | $O(n \log n)$ |
| `poly_exp` | $O(n \log n)$ |
| `poly_pow` | $O(n \log n)$ |
| `poly_div` | $O(n \log n)$ |

---

## 注意

1. **长度：** NTT 输入长度必须为 2 的幂，`mul` 自动处理补齐。
2. **模数限制：** 本模板使用 `998244353`（$119 \cdot 2^{23} + 1$），最大变换长度 $2^{23}$。若需要更大长度或不同模数，换用表中原根。
3. **poly_ln 要求：** `a[0] = 1`；**poly_exp 要求：** `a[0] = 0`。
4. **poly_pow：** 自动处理前导零（提取最低非零项），支持 $k$ 为 `long long`。
5. 若需**任意模数 NTT**（模数不是 NTT-friendly），使用拆系数 FFT（MTT），此处未实现。

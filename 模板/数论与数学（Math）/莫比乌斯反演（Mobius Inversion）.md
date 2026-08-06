# 莫比乌斯反演（Mobius Inversion）$O(n\log n)$

## 概念

**莫比乌斯函数 $\mu(n)$：**
$$\mu(n)=\begin{cases}1 & n=1\ (-1)^k & n=\text{k 个不同质数之积}\ 0 & n\text{ 含平方因子}\end{cases}$$

**关键恒等式**（反演的基础）：
$$\sum_{d\mid n}\mu(d)=\begin{cases}1 & n=1\ 0 & n>1\end{cases}=[n=1]$$

**反演形式 1（按倍数求和）**：若 $g(x)=\sum_{x\mid k}f(k)$，则
$$f(k)=\sum_{k\mid x}\mu\!\left(\tfrac{x}{k}\right)g(x)$$

**反演形式 2（按约数求和）**：若 $g(n)=\sum_{d\mid n}f(d)$，则
$$f(n)=\sum_{d\mid n}\mu(d)\,g\!\left(\tfrac{n}{d}\right)$$

> 用途：把"对 $x$ 的倍数/约数求和"这种耦合条件，变换成"每个坐标独立"的形式，从而转化为简单的计数。

---

## 代码

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 线性筛莫比乌斯函数 μ[1..n]
 * @param n 上界
 * @return vector<int> mu，mu[1]=1，mu[i] = μ(i)
 */
vector<int> mobius_sieve(int n) {
    vector<int> primes, minp(n + 1, 0), mu(n + 1, 0);
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!minp[i]) { minp[i] = i; primes.push_back(i); mu[i] = -1; }
        for (int p : primes) {
            if (i * p > n) break;
            minp[i * p] = p;
            if (i % p == 0) { mu[i * p] = 0; break; }   // 含平方因子 → 0
            mu[i * p] = -mu[i];                          // 乘新质因子 → 符号翻转
        }
    }
    return mu;
}

/**
 * @brief 倍数求和变换：g[x] = Σ_{x | k} f[k]
 *        枚举每个 x 的倍数 k，把 f[k] 累加到 g[x]。
 * @param f f[1..n]
 * @param n 上界
 * @return vector<long long> g[1..n]
 */
vector<long long> sum_over_multiples(const vector<long long>& f, int n) {
    vector<long long> g(n + 1, 0);
    for (int x = 1; x <= n; ++x)
        for (int k = x; k <= n; k += x)
            g[x] += f[k];                 // 需要取模则在此处 % MOD
    return g;
}

/**
 * @brief 莫比乌斯反演（倍数版）：f[k] = Σ_{k | x} μ(x/k) g[x]
 *        是 sum_over_multiples 的逆变换。
 * @param g g[1..n]，由 sum_over_multiples 得到
 * @param mu mobius_sieve 的结果
 * @param n 上界
 * @return vector<long long> f[1..n]
 */
vector<long long> mobius_invert(const vector<long long>& g, const vector<int>& mu, int n) {
    vector<long long> f(n + 1, 0);
    for (int k = 1; k <= n; ++k)
        for (int x = k; x <= n; x += k)
            f[k] += mu[x / k] * g[x];
    return f;
}

/**
 * @brief 约数求和变换：g[n] = Σ_{d | n} f[d]
 * @param f f[1..n]
 * @param n 上界
 * @return vector<long long> g[1..n]
 */
vector<long long> sum_over_divisors(const vector<long long>& f, int n) {
    vector<long long> g(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            g[m] += f[d];
    return g;
}

/**
 * @brief 莫比乌斯反演（约数版）：f[n] = Σ_{d | n} μ(d) g[n/d]
 *        是 sum_over_divisors 的逆变换。
 * @param g g[1..n]，由 sum_over_divisors 得到
 * @param mu mobius_sieve 的结果
 * @param n 上界
 * @return vector<long long> f[1..n]
 */
vector<long long> divisor_invert(const vector<long long>& g, const vector<int>& mu, int n) {
    vector<long long> f(n + 1, 0);
    for (int m = 1; m <= n; ++m)
        for (int d = 1; d <= m; ++d)
            if (m % d == 0) f[m] += mu[d] * g[m / d];
    return f;
}
```

---

## 使用示例

```cpp
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    int n = 10;
    auto mu = mobius_sieve(n);

    // 例1：倍数变换 + 反演 互逆（往返测试）
    vector<long long> f(n + 1);
    for (int i = 1; i <= n; ++i) f[i] = i;        // 随便给一个 f
    auto g  = sum_over_multiples(f, n);            // g[x] = Σ_{x|k} f[k]
    auto f2 = mobius_invert(g, mu, n);             // 应还原出 f
    for (int i = 1; i <= n; ++i) assert(f2[i] == f[i]);

    // 例2：计数 1 ≤ i,j ≤ n 且 gcd(i,j)=1 的对数（经典约数反演）
    // [gcd(i,j)=1] = Σ_{d | gcd(i,j)} μ(d)，故答案 = Σ_{d=1}^n μ(d) ⌊n/d⌋²
    long long ans = 0;
    for (int d = 1; d <= n; ++d)
        ans += mu[d] * 1LL * (n / d) * (n / d);
    cout << ans << "\n";   // n=10 → 63
}
```

---

## 复杂度

- **线性筛 μ：** $O(n)$ 时间，$O(n)$ 空间。
- **倍数/约数求和变换及反演：** 每层循环是调和级数，$O(n\log n)$ 时间，$O(n)$ 空间。

---

## 注意

1. 下标从 1 开始（数组开 $n+1$，下标 0 不用）。
2. 变换与反演互为逆运算：`mobius_invert(sum_over_multiples(f), mu) == f`。
3. 倍数版和约数版对应两种求和方向，别混用（一个枚举倍数、一个枚举约数）。
4. 需要取模时，在累加处 `% MOD`；`mu` 有负值，注意用 `long long` 再取模。
5. 若 $n$ 很大（如 $10^9$）无法开数组，可改用"整除分块 + 杜教筛"求 $\mu$ 的前缀和，属于进阶内容。

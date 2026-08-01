#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MOD = 998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, c;
    cin >> N >> c;

    // 最大 Ω = floor(log2(N))
    int maxO = 0;
    while ((1LL << (maxO + 1)) <= N) ++maxO;

    // 递推 h[m]：DFS 枚举 m 的整数拆分（至少 2 部分），收集各部分 h 值之积后取 mex
    vector<ll> h(maxO + 1, 0);
    h[0] = h[1] = 1;
    for (int m = 2; m <= maxO; ++m) {
        set<ll> st;
        function<void(int, int, ll, int)> gen = [&](int rem, int last, ll prod, int cnt) {
            if (rem == 0) {
                if (cnt >= 2) st.insert(prod);
                return;
            }
            for (int x = min(last, rem); x >= 1; --x)
                gen(rem - x, x, prod * h[x], cnt + 1);
        };
        gen(m, m - 1, 1, 0);
        int mex = 1;                    // 0 已被显式加入集合，mex 必 ≥ 1
        while (st.count(mex)) ++mex;
        h[m] = mex;
    }

    // 线性筛最小质因子 lp，递推 Ω(i) = Ω(i / lp(i)) + 1
    vector<int> lp(N + 1, 0), primes;
    vector<unsigned char> om(N + 1, 0);
    for (int i = 2; i <= N; ++i) {
        if (lp[i] == 0) { lp[i] = i; primes.push_back(i); }
        for (int p : primes) {
            long long v = 1LL * i * p;
            if (p > lp[i] || v > N) break;
            lp[v] = p;
        }
    }
    for (int i = 2; i <= N; ++i) om[i] = om[i / lp[i]] + 1;

    // 累加 Σ h[Ω(i)] * c^i (mod MOD)
    ll ans = 0, pw = c % MOD;
    for (int i = 1; i <= N; ++i) {
        ans = (ans + h[om[i]] * pw) % MOD;
        pw = pw * (c % MOD) % MOD;
    }
    cout << ans << '\n';
    return 0;
}

/* ============================================================================
 * 完整思路整理
 * ============================================================================
 *
 * 【目标】给定 N、c，求 ans = (Σ_{i=1}^{N} g(i)·c^i) mod 998244353，其中
 *           g(1) = 1，
 *           g(n) = mex({∏g(w_i) | n = ∏w_i, 1 < w_i < n} ∪ {0})，n ≥ 2。
 *
 * 一、g(n) 只依赖质因子总数 Ω(n)
 *     设 Ω(n) = m（计重数）。n 的任意非平凡分解 n = ∏w_i 中，每个 w_i 的质因子
 *     数 x_i = Ω(w_i) ≥ 1，且 Σx_i = m、项数 ≥ 2。
 *     反之，把 n 的 m 个质因子任意分成 ≥2 组（每组大小 x_i），就能得到该拆分
 *     对应的分解。由归纳假设 g(w_i) = h_{x_i}，故
 *         g(n) = h_m = mex({0} ∪ {∏h_{x_j} : Σx_j = m, ≥2 项})，
 *     与 n 的具体结构无关。初值 h_0 = g(1) = 1，h_1 = g(质数) = 1。
 *
 * 二、预处理小状态 h_m
 *     n ≤ 1e7 时 Ω(n) ≤ ⌊log2 n⌋ ≤ 23，状态极小。
 *     对每个 m，DFS 枚举 m 的整数拆分（按非增序枚举，避免重复），
 *     收集各部分 h 值之积，取 mex 即得 h_m。
 *     注意 mex 集合显式包含 0，故 mex 从 1 开始查。
 *     （实测规律：m ≥ 2 时 h_m = 第 m-1 个素数，即 2,3,5,7,...）
 *
 * 三、线性筛统计答案
 *     线性筛维护最小质因子 lp(i)，递推 Ω(i) = Ω(i / lp(i)) + 1。
 *     顺序维护 c^i，累加 Σ h_{Ω(i)} · c^i (mod 998244353) 即为答案。
 *
 * 【复杂度】时间 O(N)，空间 O(N)。N ≤ 1e7 可轻松通过。
 * ========================================================================== */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll N, D, S;
    cin >> N >> D >> S;

    // 答案 = max_{g ∈ G} T(g)
    //   G    = {S, 2S, ..., S·Mmax}  ：从 S 一步可达的 S 的倍数
    //   T(g) = g·min(⌊N/g⌋, ⌊D/g⌋+1) ：从 g 一跳能到的最高楼层（封顶 N）
    ll Mmax = min(N / S, D / S + 1);
    ll Gmax = S * Mmax;

    ll best = 0;
    // 整除分块：⌊N/g⌋ 与 ⌊D/g⌋ 都恒定的区间内，T(g) 随 g 单调递增，
    // 只需取该区间内最大的 S 的倍数作为候选 g。区间总数 O(√N)。
    for (ll g = 1; g <= Gmax; ) {
        ll qN = N / g, qD = D / g;
        ll hi = Gmax;                       // 本区间右端点
        if (qN > 0) hi = min(hi, N / qN);
        if (qD > 0) hi = min(hi, D / qD);
        ll cand = (hi / S) * S;             // 区间内最大的 S 的倍数
        if (cand >= g) {
            ll k = min(qN, qD + 1);
            best = max(best, cand * k);     // T(cand) = cand·k
        }
        g = hi + 1;
    }
    cout << best << '\n';
    return 0;
}

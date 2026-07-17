/**
 * Problem E - 排列和
 *
 * 题意：给定 0..n-1 的一个排列 P，定义
 *   f(P) = Σ_{0≤i<j<n} (P_j - P_i)
 * 求 f(P) 的值。
 *
 * 思路：
 * 对于每个位置 i，P_i 在 f(P) 中的系数：
 *   - 作为被减数出现在 i<j 的项中，贡献 +P_i，共 n-1-i 次
 *   - 作为减数出现在 j<i 的项中，贡献 -P_i，共 i 次
 * 总系数 = (n-1-i) - i = n-1-2i
 * 所以 f(P) = Σ P_i * (n-1-2i)
 * O(n) 计算即可。
 *
 * 复杂度：O(n)
 */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<ll> p(n);
    for (int i = 0; i < n; i++) cin >> p[i];

    ll ans = 0;
    for (int i = 0; i < n; i++) {
        ans += p[i] * (n - 1 - 2 * i);
    }
    cout << ans << '\n';

    return 0;
}

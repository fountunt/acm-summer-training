#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        long long x, y;
        cin >> x >> y;

        long long sxy = x + y;
        long long n = (long long)((sqrt(1.0 + 8.0 * sxy) - 1) / 2);
        while ((n + 1) * (n + 2) / 2 <= sxy) n++;
        while (n * (n + 1) / 2 > sxy) n--;

        long long S = n * (n + 1) / 2;
        long long lo = max(0LL, S - y), hi = min(x, S);

        // 凸函数,最优 p 为 (S+x-y)/2 在 [lo,hi] 上的投影
        long long p1 = (S + x - y) / 2;           // floor
        long long p2 = p1 + 1;                    // ceil
        auto clamp = [&](long long v) { return max(lo, min(hi, v)); };
        long long p = clamp(p1);
        auto dist = [&](long long pp) {
            return (pp - x) * (pp - x) + (S - pp - y) * (S - pp - y);
        };
        if (dist(clamp(p2)) < dist(p)) p = clamp(p2);

        // 找 X 的个数 u 使 p 在可达区间 [u(u+1)/2, u(2n+1-u)/2] 内
        long long u = -1;
        for (long long uu = 0; uu <= n; uu++) {
            long long pmin = uu * (uu + 1) / 2;
            long long pmax = uu * (2 * n + 1 - uu) / 2;
            if (pmin <= p && p <= pmax) { u = uu; break; }
        }

        // 构造 w(第 i 个 X 的活跃步数):严格递减,和 = p
        vector<long long> w(u);
        for (long long i = 0; i < u; i++) w[i] = u - i;
        long long R = p - u * (u + 1) / 2;
        for (long long i = 0; i < u && R > 0; i++) {
            long long maxw = n - i;
            long long upper = (i == 0) ? maxw : min(maxw, w[i - 1] - 1);
            long long add = min(R, upper - w[i]);
            w[i] += add;
            R -= add;
        }

        string s(n, 'Y');
        for (long long i = 0; i < u; i++) {
            long long pos = n + 1 - w[i];   // X 的位置(1-based)
            s[pos - 1] = 'X';
        }
        cout << s << '\n';
    }
    return 0;
}

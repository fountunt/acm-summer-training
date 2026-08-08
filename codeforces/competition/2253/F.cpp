#include <bits/stdc++.h>
using namespace std;

const long long INF = (1LL << 62);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> m(n + 1);
    for (int i = 1; i <= n; i++) cin >> m[i];

    long long ans = 0;

    // 每个与 6 互素的部分 c 独立:数字 2^a * 3^b * c <= n
    for (long long c = 1; c * 4 <= n; c++) {
        long long cc = c;
        while (cc % 2 == 0) cc /= 2;
        while (cc % 3 == 0) cc /= 3;
        if (cc != c) continue;  // 只处理类的代表(与 6 互素)

        // 行 b:3^b * c <= n;列 a:2^a * 3^b * c <= n
        vector<int> Ab;  // A(b):每行列数-1
        long long p3 = 1;
        while (p3 * c <= n) {
            int a = 0;
            while ((1LL << a) * p3 * c <= n) a++;
            Ab.push_back(a - 1);
            p3 *= 3;
        }
        int B = (int)Ab.size() - 1;
        int A0 = Ab[0];
        int full = 1 << (A0 + 1);

        vector<long long> dp_prev(full, 0);  // dp[-1] = 0

        long long p3b = 1;
        for (int b = 0; b <= B; b++, p3b *= 3) {
            int aB = Ab[b];
            int size = 1 << (aB + 1);
            if (aB >= 2) {
                int R = aB - 1;  // 约束列 0..aB-2
                int hsize = 1 << R;
                vector<long long> H(hsize, INF);
                for (int mask = 0; mask < size; mask++) {
                    long long val = 0;
                    for (int a = 0; a <= aB; a++) {
                        if (mask >> a & 1) {
                            val += m[(1LL << a) * p3b * c];
                        }
                    }
                    val += dp_prev[mask];
                    int req = 0;
                    for (int a = 0; a < R; a++) {
                        bool covered = (mask >> a & 1) || (mask >> (a + 1) & 1) || (mask >> (a + 2) & 1);
                        if (!covered) req |= 1 << a;
                    }
                    if (val < H[req]) H[req] = val;
                }
                // SOS 子集 min:H[mask] = min over req subset of mask
                for (int i = 0; i < R; i++) {
                    for (int mask = 0; mask < hsize; mask++) {
                        if (mask >> i & 1) {
                            if (H[mask ^ (1 << i)] < H[mask]) H[mask] = H[mask ^ (1 << i)];
                        }
                    }
                }
                int lowmask = hsize - 1;
                vector<long long> dp_cur(full);
                for (int mask_next = 0; mask_next < full; mask_next++) {
                    dp_cur[mask_next] = H[mask_next & lowmask];
                }
                dp_prev.swap(dp_cur);
            } else {
                long long best = INF;
                for (int mask = 0; mask < size; mask++) {
                    long long val = 0;
                    for (int a = 0; a <= aB; a++) {
                        if (mask >> a & 1) val += m[(1LL << a) * p3b * c];
                    }
                    val += dp_prev[mask];
                    if (val < best) best = val;
                }
                fill(dp_prev.begin(), dp_prev.end(), best);
            }
        }
        ans += dp_prev[0];
    }

    cout << ans << '\n';
    return 0;
}

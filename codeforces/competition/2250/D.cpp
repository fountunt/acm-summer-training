#include <bits/stdc++.h>
using namespace std;

const int N = 1000000;
const int mod = 998244353;

int n;
int a[N + 10], vis[N + 10];
bool pre[N + 10], suf[N + 10];

void work() {
    cin >> n;
    for (int i = 1; i < n; ++i) cin >> a[i];
    a[n] = 0; // 用于 suf 计算

    // 如果存在 a[i] == n，直接无解
    for (int i = 1; i < n; ++i) {
        if (a[i] == n) {
            cout << 0 << '\n';
            return;
        }
    }

    // pre[i] : a[1..i] 是否非递减
    pre[0] = true;
    for (int i = 1; i < n; ++i) {
        pre[i] = pre[i - 1] && (a[i] >= a[i - 1]);
    }

    // suf[i] : a[i..n-1] 是否非递增
    suf[n] = true;
    for (int i = n - 1; i >= 1; --i) {
        suf[i] = suf[i + 1] && (a[i] >= a[i + 1]);
    }

    long long ans = 0;

    for (int cut = 0; cut < n; ++cut) {
        // 检查前后缀条件
        if (!pre[cut] || !suf[cut + 1]) continue;
        // 中间相邻相等不能作为分界
        if (cut > 0 && cut < n - 1 && a[cut] == a[cut + 1]) continue;

        vector<int> l, r;
        for (int i = 1; i <= cut; ++i) l.push_back(a[i]);
        for (int i = cut + 1; i < n; ++i) r.push_back(a[i]);
        reverse(r.begin(), r.end());

        memset(vis, 0, sizeof(int) * (n + 1));

        int x = 0, y = 0, used = 0;
        long long ways = 1;

        while (x < (int)l.size() || y < (int)r.size()) {
            int w;
            if (x == (int)l.size()) {
                w = r[y++];
            } else if (y == (int)r.size()) {
                w = l[x++];
            } else if (l[x] < r[y]) {
                w = l[x++];
            } else if (l[x] > r[y]) {
                w = r[y++];
            } else {
                // 两个序列当前最小值相等，冲突，无解
                cout << 0 << '\n';
                return;
            }

            if (!vis[w]) {
                vis[w] = 1;
            } else {
                // 重复出现的值：有 w - used 种选择
                if (w < used) {
                    ways = 0;
                } else {
                    ways = ways * (w - used) % mod;
                }
            }
            ++used;
        }

        ans = (ans + ways) % mod;
    }

    cout << ans % mod << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        work();
    }

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;

        vector<int> c, sz;
        for (int x : a) {
            if (c.empty() || c.back() != x) {
                c.push_back(x);
                sz.push_back(1);
            } else {
                sz.back()++;
            }
        }
        int k = (int)c.size();

        int ans = k;
        bool ok = false;
        for (int i = 0; i + 1 < k; i++) {
            if (sz[i] >= 2 && sz[i + 1] >= 2) { ans = k + 2; ok = true; break; }
        }

        if (!ok) {
            for (int i = 0; i < k; i++) {
                if (sz[i] < 2) continue;
                bool L = (i >= 1 && (i <= 1 || c[i - 2] != c[i]));
                bool R = (i <= k - 2 && (i >= k - 2 || c[i] != c[i + 2]));
                if (L || R) { ans = k + 1; break; }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        string s;
        cin >> n >> s;
        int ans = 0;
        for (int x = 0; x < 2; ++x) {
            for (int y = 0; y < 2; ++y) {
                vector<int> v(n);
                v[0] = x;
                v[1] = y;
                for (int i = 2; i < n; ++i) {
                    v[i] = 1 - v[i - 2];
                }
                bool ok = true;
                for (int i = 0; i < n; ++i) {
                    if (s[i] != '?' && s[i] - '0' != v[i]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    ++ans;
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}

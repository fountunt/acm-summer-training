#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n;
        long long k;
        cin >> n >> k;

        int L = 0;
        while ((1LL << L) <= n) L++;
        long long lim = 1LL << L;

        bool ok = true;

        if (k >= lim) ok = false;

        if (ok && (n & (n - 1)) == 0) {
            if (k < n) ok = false;
        }

        if (!ok) {
            cout << "NO\n";
            continue;
        }

        long long need = k ^ n;

        vector<int> special;
        for (int b = 0; b < L; b++) {
            if ((need >> b) & 1LL) {
                int val = 1 << b;
                if (val >= n) {
                    ok = false;
                    break;
                }
                special.push_back(val);
            }
        }

        if (!ok) {
            cout << "NO\n";
            continue;
        }

        cout << "YES\n";

        vector<int> ans;
        ans.reserve(n);

        for (int x = 1; x < n; x++) {
            bool isSpecial = false;
            for (int s : special) {
                if (s == x) {
                    isSpecial = true;
                    break;
                }
            }
            if (!isSpecial) ans.push_back(x);
        }

        ans.push_back(0);

        for (int s : special) ans.push_back(s);

        for (int i = 0; i < n; i++) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }

    return 0;
}
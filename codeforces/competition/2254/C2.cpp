#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n;
    cin >> n;
    string a, b;
    cin >> a >> b;

    ll ans = 0;

    for (int parity = 0; parity <= 1; ++parity) {
        vector<int> posA, posB;

        for (int i = parity; i < n; i += 2) {
            if (a[i] == '0') posA.push_back(i / 2);
            if (b[i] == '0') posB.push_back(i / 2);
        }

        if (posA.size() != posB.size()) {
            cout << -1 << "\n";
            return;
        }

        for (int i = 0; i < (int)posA.size(); ++i) {
            ans += abs(posA[i] - posB[i]);
        }
    }

    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}

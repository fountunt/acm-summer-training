#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    for (cin >> T; T--; ) {
        int n; cin >> n;
        for (int i = 2; i <= n; i++) { int x; cin >> x; }
        int m; cin >> m;
        vector<int> a(m);
        int skip = n + 1;
        for (int &x : a) { cin >> x; skip = min(skip, x); }
        if (m == 1) { cout << "0\n"; continue; }
        cout << m - 1;
        for (int x : a) if (x != skip) cout << ' ' << x;
        cout << '\n';
    }
}

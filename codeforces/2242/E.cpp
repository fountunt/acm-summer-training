#include <bits/stdc++.h>
using namespace std;

int l, r, n;

vector<int> getBinary(int v) {
    vector<int> rep;
    while (v > 0) {
        rep.push_back(v & 1);
        v >>= 1;
    }
    reverse(rep.begin(), rep.end());
    return rep;
}

string getAnswer(int x, int y, int n) {
    vector<int> x_bits = getBinary(x);
    vector<int> y_bits = getBinary(y);

    string ans(n, '0');
    for (int i = 0; i < n; i++) {
        ans[i] = '0' + (x_bits[i % x_bits.size()] & y_bits[i % y_bits.size()]);
    }
    return ans;
}

void solve() {
    cin >> l >> r >> n;

    int k = 63 - __builtin_clzll(r);
    int x = -1, y = -1;

    if (l >= (1ll << k)) {
        int p = 30;
        while (((l >> p) & 1) == ((r >> p) & 1))
            p--;
        x = l;
        y = l - (l & ((1 << p) - 1)) + (1 << p);
    } else {
        x = max(l, (1 << (k - 1)));
        y = 1ll << k;
    }

    string ans = getAnswer(x, y, n);
    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}

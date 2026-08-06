#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int P(ll r, ll c) {
    if (r == c) return 1;
    if (r > c) {
        ll q = r / c, s = r % c;
        if (s == 0) return (int)((q - 1) & 1) ^ 1;
        return (int)(q & 1) ^ P(s, c);
    } else {
        ll q = c / r, s = c % r;
        if (s == 0) return (int)((q - 1) & 1) ^ 1;
        return (int)(q & 1) ^ P(r, s);
    }
}

int gval(ll r, ll c) {
    int res = P(r, c);
    if (r > 1) res ^= P(r - 1, c);
    if (c > 1) res ^= P(r, c - 1);
    if (r > 1 && c > 1) res ^= P(r - 1, c - 1);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    cin >> N;
    int x = 0;
    for (int i = 0; i < N; i++) {
        ll r, c;
        cin >> r >> c;
        x ^= gval(r, c);
    }
    cout << (x ? "FIRST" : "SECOND") << '\n';
    return 0;
}

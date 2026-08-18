#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<ll> b(n);
    ll sum = 0;
    for (int i = 0; i < n; i++) {
        cin >> b[i];
        sum += b[i];
    }

    // 总和必须大于 0，因为 a_n = sum
    if (sum <= 0) {
        cout << -1 << "\n";
        return;
    }

    multiset<ll> ms(b.begin(), b.end());

    // 第一个元素必须是最小的正数
    auto it = ms.lower_bound(1);
    if (it == ms.end()) {
        cout << -1 << "\n";
        return;
    }

    vector<ll> a;
    ll cur = *it;
    a.push_back(cur);
    ms.erase(it);

    // 贪心构建剩余元素
    while (!ms.empty()) {
        ll need = 1 - cur;  // 下一个元素 x 必须满足 cur + x >= 1
        auto nxt = ms.lower_bound(need);
        if (nxt == ms.end()) {
            cout << -1 << "\n";
            return;
        }
        ll x = *nxt;
        ms.erase(nxt);

        cur += x;
        a.push_back(cur);
    }

    // 输出结果
    for (int i = 0; i < n; i++) {
        cout << a[i] << " \n"[i == n - 1];
    }
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

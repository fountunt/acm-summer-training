#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve() {
    int n;
    cin >> n;

    vector<ll> b(n);
    vector<ll> sortedB(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
        sortedB[i] = b[i];
    }

    // 排序并统计每个唯一值的出现次数
    sort(sortedB.begin(), sortedB.end());

    if (sortedB[0] != 0) {
        cout << -1 << "\n";
        return;
    }

    vector<pair<ll, ll>> groups; // {value, count}
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && sortedB[j] == sortedB[i]) ++j;
        groups.push_back({sortedB[i], j - i});
        i = j;
    }

    int k = groups.size();
    vector<ll> aVal(k);

    bool ok = true;
    for (int i = 0; i + 1 < k; ++i) {
        ll diff = groups[i + 1].first - groups[i].first;
        ll cnt = groups[i].second;

        if (diff <= 0 || diff % cnt != 0) {
            ok = false;
            break;
        }

        ll x = diff / cnt;
        if (x < 1) {
            ok = false;
            break;
        }
        if (i > 0 && x <= aVal[i - 1]) {
            ok = false;
            break;
        }
        aVal[i] = x;
    }

    if (!ok) {
        cout << -1 << "\n";
        return;
    }

    // 最后一组
    if (k == 1) {
        aVal[0] = 1;
    } else {
        aVal[k - 1] = aVal[k - 2] + 1;
    }

    // 检查 1e18 限制
    for (ll x : aVal) {
        if (x > (ll)1e18) {
            ok = false;
            break;
        }
    }
    if (!ok) {
        cout << -1 << "\n";
        return;
    }

    // 建立映射：每个唯一 b 值 -> 对应的 a 值
    vector<pair<ll, ll>> mp; // {b_value, a_value}
    for (int i = 0; i < k; ++i) {
        mp.push_back({groups[i].first, aVal[i]});
    }

    // 对原顺序的每个 b[i] 二分查找
    for (int i = 0; i < n; ++i) {
        auto it = lower_bound(mp.begin(), mp.end(), make_pair(b[i], 0LL));
        cout << it->second << " \n"[i == n - 1];
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

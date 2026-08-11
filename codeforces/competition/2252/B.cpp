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

        int cnt0 = 0, cnt1 = 0;
        for (char c : s) {
            if (c == '0') cnt0++;
            else cnt1++;
        }

        int ans = 0;
        if (abs(cnt0 - cnt1) > 1) {
            ans = (abs(cnt0 - cnt1) - 1) * 2;
        }

        cout << ans << '\n';
    }
    return 0;
}

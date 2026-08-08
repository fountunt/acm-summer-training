#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m, x, y;
        cin >> n >> m >> x >> y;
        vector<int> a(x), b(y);
        for (int &v : a) cin >> v;
        for (int &v : b) cin >> v;

        int lim = n + m;
        vector<char> inA(lim + 1, 0), inB(lim + 1, 0);
        for (int v : a) inA[v] = 1;
        for (int v : b) inB[v] = 1;

        int p = min(x, n), q = min(y, m);
        // 行、列都操作满时恰有一个操作值不出现,总配额减 1
        int total = (x >= n && y >= m) ? p + q - 1 : p + q;

        long long ans = 0;
        int cntA = 0, cntB = 0, cnt = 0;
        for (int v = lim; v >= 1 && cnt < total; v--) {
            if (inA[v] && inB[v]) {
                ans += v; cnt++;
            } else if (inA[v]) {
                if (cntA < p) { ans += v; cntA++; cnt++; }
            } else if (inB[v]) {
                if (cntB < q) { ans += v; cntB++; cnt++; }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}

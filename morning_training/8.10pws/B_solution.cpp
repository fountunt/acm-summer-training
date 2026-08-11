#include <bits/stdc++.h>
using namespace std;
const int MX = 1005;
int n, m, a[MX];
int res[MX][MX];
bool add(int x, int y) {
    if (res[x][y] || res[x + 1][y] || res[x - 1][y] || res[x][y + 1] || res[x][y - 1])
        return false;
    res[x][y] = 1;
    return true;
}
int main() {
    cin.tie(0) -> sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) {
        if (a[i] > (m + 1) / 2) {
            cout << -1 << endl;
            return 0;
        }
    }
    if (m % 2 == 0) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= a[i]; j++) {
                res[i][2 * j - (i & 1)] = 1;
            }
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++)
                cout << res[i][j];
            cout << "\n";
        }
        return 0;
    }
    for (int i = 1, lst = 0; i <= n; i++) {
        if (a[i] == (m + 1) / 2) {
            for (int j = 1; j <= a[i]; j++) {
                if (!add(i, 2 * j - 1)) {
                    cout << -1 << endl;
                    return 0;
                }
            }
            if (lst && lst + 1 == i) {
                cout << -1 << endl;
                return 0;
            }
            int st = 2;
            if (res[i][1] == 1 && (i - lst) % 2 == 1) st = 1;
            for (int k = lst + 1; k <= i - 1; k++) {
                for (int j = st; j <= m && a[k]; j += 2) {
                    if (add(k, j)) a[k]--;
                }
                for (int j = 1; j <= m && a[k]; j++) {
                    if (add(k, j)) a[k]--;
                }
                if (a[k]) {
                    cout << -1 << endl;
                    return 0;
                }
                st = 3 - st;
            }
            lst = i;
            continue;
        }
        if (i == n) {
            int st = 2;
            if (res[i + 1][1] == 1 && (i + 1 - lst) % 2 == 1) st = 1;
            for (int k = lst + 1; k <= i; k++) {
                for (int j = st; j <= m && a[k]; j += 2) {
                    if (add(k, j)) a[k]--;
                }

                for (int j = 1; j <= m && a[k]; j++) {
                    if (add(k, j)) a[k]--;
                }
                if (a[k]) {
                    cout << -1 << endl;
                    return 0;
                }
                st = 3 - st;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++)
            cout << res[i][j];
        cout << "\n";
    }
    return 0;
}

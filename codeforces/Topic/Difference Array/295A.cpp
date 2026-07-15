#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

struct OP
{
    int l, r, d;
};

signed main()
{
    int n = 0, m = 0, k = 0;
    cin >> n >> m >> k;
    vector<int> a(n + 2), diff(n + 3, 0);
    for (int i = 1; i <= n; i++) cin >> a[i];

    for (int i = 1; i <= n; i++) diff[i] = a[i] - a[i - 1];

    vector<OP> op(m + 2);
    for (int i = 1; i <= m; i++)
        cin >> op[i].l >> op[i].r >> op[i].d;

    vector<int> cnt(m + 3, 0);
    while (k--)
    {
        int x, y;
        cin >> x >> y;
        cnt[x] += 1;
        cnt[y + 1] -= 1;
    }

    for (int i = 1; i <= m; i++)
    {
        cnt[i] += cnt[i - 1];
    }

    for (int i = 1; i <= m; i++)
    {
        diff[op[i].l] += op[i].d * cnt[i];
        diff[op[i].r + 1] -= op[i].d * cnt[i];
    }

    for (int i = 1; i <= n; i++)
    {
        a[i] = diff[i] + a[i - 1];
    }

    for (int i = 1; i <= n; i++)
        cout << a[i] << " \n"[i == n];

    return 0;
}

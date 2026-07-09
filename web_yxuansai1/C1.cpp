#include <bits/stdc++.h>
using namespace std;

const int N = 200005;
map<int,int> f;

int find(int u)
{
    if (f[u] == 0) return u;
    return f[u] = find(f[u]);
}

struct Node{int l, r;} a[N];

void solve()
{
    int n, m;
    cin >> n >> m;

    f.clear();

    for (int i = 1; i <= n; i++) cin >> a[i].l >> a[i].r;

    sort(a + 1, a + n + 1, [](Node x, Node y){return x.r < y.r;});

    int ans = m;
    for (int i = 1; i <= n; i++)
    {
        if (find(a[i].l) != a[i].r)
        {
            ans--;
            f[find(a[i].l)] = find(a[i].l) + 1;
        }
    }

    cout << ans << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}

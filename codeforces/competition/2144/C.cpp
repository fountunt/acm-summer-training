#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

void solve()
{
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for(int i = 0;i < n;i++) cin >> a[i];
    for(int i = 0;i < n;i++) cin >> b[i];

    int ans = 1;
    for(int i = 0;i < n;i++)
    {
        if(a[i] > b[i]) swap(a[i], b[i]);
        if(i == 0 || a[i] >= b[i-1])
            ans = ans * 2 % MOD;
    }
    cout << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}

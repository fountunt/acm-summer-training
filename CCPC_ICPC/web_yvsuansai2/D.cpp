#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
#define endl "\n"
const int MOD = 998244353;

void solve()
{
    int N;
    cin >> N;
    vector<int> a(N);
    for(int i = 0;i < N;i++) cin >> a[i];
    sort(a.rbegin(),a.rend());
    int ans = 0;
    for(int i = 0;i < N;i++)
    {
        ans += (i+1+1)*i/2*a[i]%MOD;
    }
    cout << ans << endl;
}

signed main()
{
    int T;
    cin >> T;
    while(T--) solve();
    return 0;
}
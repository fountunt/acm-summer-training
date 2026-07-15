#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"
const int N = 2e5+5,p = 998244353;
int a[N];

void solve()
{
    int n;
    cin >> n;
    for(int i = 1;i <= n;i++)cin >> a[i];
    sort(a+1,a+n+1);
    int ans = 0,pw = 1;
    for(int i = 1;i <= n;i++) ans = (ans*2+(pw+1)*a[i])%p,pw = pw*3%p;
    cout << ans*499122177%p << endl;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--) solve();
    return 0;
}

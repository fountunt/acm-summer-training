//a,b 在第i天看同一集，或者什么都不看
//dp[i] 第i天的右端点
//a[x] == b[x] dp[i] = x;
//a[x] != b[x] dp[i] = min(a[x],b[x])-1;
//ans = 求和dp[i][N]

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--)
    {
        int n;
        cin >> n;
        vector<int> a(n),b(n);
        for(int i = 0;i < n;i++) cin >> a[i];
        for(int i = 0;i < n;i++) cin >> b[i];

        vector<int> pa(n+1,n),pb(n+1,n),dp(n+1,n);
        ll ans = 0;
        for(int i = n-1;i>=0;i--)
        {
            pa[a[i]] = i;
            pb[b[i]] = i;
            if(a[i] == b[i])
            {
                int x = a[i] + 1;
                if(pa[x] == pb[x]) dp[i] = dp[pa[x]];
                else dp[i] = min(pa[x],pb[x]);
            }
            if(pa[0] != pb[0]) ans += min(pa[0],pb[0]) - i;
            else ans += dp[pa[0]] - i;
        }
        cout << ans << "\n";
    }
}

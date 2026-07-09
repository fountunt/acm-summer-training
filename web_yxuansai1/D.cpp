#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"
typedef long long ll;
const ll N = 1e6+5;

ll a[N];
vector<ll> g[N];
ll dp[N][3];

void dfs(ll u,ll fa)
{
    ll sum = 0;
    for(auto v : g[u])
    {
        if(v == fa) continue;
        dfs(v,u);
        sum += dp[v][0];
    }
    ll up = sum;
    ll down = sum;
    for(auto it : g[u])
    {
        if(it == fa) continue;
        up = max(up,sum-dp[it][0]+dp[it][1]+a[it]-a[u]);
        down = max(down,sum-dp[it][0]+dp[it][2]+a[u]-a[it]);
    }
    dp[u][0] = up+down-sum;
    dp[u][1] = up;
    dp[u][2] = down;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n;
    cin >> n;
    for(ll i = 1;i <= n;i++) cin >> a[i];
    for(ll i = 1;i <= n-1;i++)
    {
        ll x,y;
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs(1,0);
    cout << dp[1][0] << endl;
    return 0;
}
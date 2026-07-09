#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"
const int N = 5005;

vector<pair<int,int>> G[N];
vector<int> g[N];
int ans,d[N],f[N];
void dfs1(int u,int x)
{
    for(auto v : g[u]) d[u] = min(d[u],f[v]);
    for(auto[v,w] : G[u]) if(v != x) dfs1(v,u),d[u] = min(d[u],d[v]+w);
}
void dfs2(int u,int x)
{
    for(auto[v,w] : G[u])if(v != x)d[v] = min(d[v],d[u] + w),dfs2(v,u);
    ans += d[u];
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m;
    cin >> n >> m;
    for(int i = 1;i < n;i++)
    {
        int u,v,w;
        cin >> u >> v >> w;
        G[u].push_back({v,w});
        G[v].push_back({u,w});
    }
    for(int i = 1;i <= m;i++)
    {
        int p,q;
        cin >> p >> q;
        g[p].push_back(q);
        g[q].push_back(p);
    }
    memset(d,0x3f,sizeof(d));
    memset(f,0x3f,sizeof(f));
    d[1] = 0;
    for(int i = 0;i <= n;i++)
    {
        ans = 0;
        dfs1(1,0);
        dfs2(1,0);
        memcpy(f,d,sizeof(f));
        cout << ans << endl;
    }

    return 0;
}
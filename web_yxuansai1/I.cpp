#include <bits/stdc++.h>
using namespace std;
#define int long long

const int N = 100010,inf = 2e18+7;
int n,m,V,st;
int vis[N];
int d[N];
vector<pair<int,int>> g[N];

void dij()
{
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;

    q.push({0,st});
    d[st] = 0;
    while(q.size())
    {
        auto [vol,u] = q.top();
        q.pop();
        if(vis[u]) continue;
        vis[u] = 1;
        for(auto [v,w]:g[u])
        {
            if(w > V) continue;
            int nw = vol + w;
            if(vol % V + w > V) nw = (vol/V)*V+V+w;
            if(nw < d[v])
            {
                d[v] = nw;
                if(!vis[v]) q.push({d[v],v});
            }
        }
    }
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> V >> st;
    for(int i = 1;i <= n;i++) d[i] = inf;
    for(int i = 1,u,v,w;i <= m;i++)
    {
        cin >> u >> v >> w;
        g[u].push_back({v,w});
        g[v].push_back({u,w});
    }

    dij();
    for(int i = 1;i <= n;i++) cout << (d[i] == inf ? -1 : (d[i]-1) / V + 1) << " ";
    return 0;
}

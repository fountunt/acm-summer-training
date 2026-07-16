//树上求两点路径并标记走过路径
//树上倍增查询公共祖先并标记走过的路线
//累加
//得出路过数组

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 1e5 + 5;
const int LOG = 17;

vector<int> adj[MAXN];
int depth[MAXN],parent[LOG][MAXN];
int cnt[MAXN];
vector<pair<int,int>> edges;
int n,k;

void dfs_lca(int u,int p)
{
    parent[0][u] = p;
    for(int v : adj[u])
    {
        if(v == p) continue;
        depth[v] = depth[u] + 1;
        dfs_lca(v,u);
    }
}

int lca(int a,int b)
{
    if(depth[a] < depth[b]) swap(a,b);
    int diff = depth[a] - depth[b];
    for(int i = 0;i < LOG;i++)
    {
        if(diff & (1 << i)) a = parent[i][a];
    }
    if(a == b) return a;
    for(int i = LOG - 1;i >= 0;i--)
    {
        if(parent[i][a] != parent[i][b])
        {
            a = parent[i][a];
            b = parent[i][b];
        }
    }
    return parent[0][a];
}

void dfs_sum(int u,int p)
{
    for(int v : adj[u])
    {
        if(v == p) continue;
        dfs_sum(v,u);
        cnt[u] += cnt[v];
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    edges.resize(n-1);
    for(int i = 0;i < n-1;i++)
    {
        int u,v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges[i] = {u,v};
    }

    dfs_lca(1,0);
    for(int i = 1;i < LOG;i++)
    {
        for(int u = 1;u <= n;u++)
        {
            parent[i][u] = parent[i-1][parent[i-1][u]];
        }
    }

    cin >> k;
    for(int i = 0;i < k;i++)
    {
        int a,b;
        cin >> a >> b;
        int w = lca(a,b);
        cnt[a]++;
        cnt[b]++;
        cnt[w] -= 2;
    }

    dfs_sum(1,0);

    for(int i = 0;i < n-1;i++)
    {
        int u = edges[i].first,v = edges[i].second;
        if(depth[u] > depth[v]) cout << cnt[u];
        else cout << cnt[v];
        if(i != n - 2) cout << " ";
    }
    cout << "\n";
}

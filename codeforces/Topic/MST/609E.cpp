#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge{int u,v,w,id;};

struct DSU
{
    vector<int> fa,sz;
    DSU(int n)
    {
        fa.resize(n + 1);
        sz.resize(n + 1,1);
        for(int i = 1;i <= n;i++) fa[i] = i;
    }

    int find(int x)
    {
        if(fa[x] == x) return x;
        return fa[x] = find(fa[x]);
    }

    bool unite(int x,int y)
    {
        x = find(x),y = find(y);
        if(x == y) return false;
        if(sz[x] < sz[y]) swap(x,y);
        fa[y] = x;sz[x] += sz[y];
        return true;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for(int i = 0;i < m;i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].id = i;
    }

    auto sorted = edges;
    sort(sorted.begin(),sorted.end(),[](const Edge& a,const Edge& b){
        return a.w < b.w;
    });

    DSU dsu(n);
    vector<bool> in_mst(m,false);
    vector<vector<pair<int,int>>> tree(n+1);
    ll sum = 0;

    for(auto& e : sorted)
    {
        if(dsu.unite(e.u,e.v))
        {
            in_mst[e.id] = true;
            sum += e.w;
            tree[e.u].emplace_back(e.v,e.w);
            tree[e.v].emplace_back(e.u,e.w);
        }
    }

    int LOG = __lg(n) + 1;
    vector<int> dep(n+1);
    vector<vector<int>> up(n + 1,vector<int>(LOG));
    vector<vector<int>> mx(n + 1,vector<int>(LOG));

    auto dfs = [&](auto&& self,int u,int p,int w) -> void
    {
        up[u][0] = p;
        mx[u][0] = w;
        for(int j = 1;j < LOG;j++)
        {
            up[u][j] = up[up[u][j-1]][j-1];
            mx[u][j] = max(mx[u][j-1],mx[up[u][j-1]][j-1]);
        }
        for(auto& [v,w2] : tree[u])
        {
            if(v == p) continue;
            dep[v] = dep[u] + 1;
            self(self,v,u,w2);
        }
    };
    dfs(dfs, 1, 0, 0);

    auto path_max = [&](int u, int v) -> int
    {
        int res = 0;
        if (dep[u] < dep[v]) swap(u, v);
        int diff = dep[u] - dep[v];
        for (int j = 0; j < LOG; ++j)
            if (diff >> j & 1) res = max(res, mx[u][j]), u = up[u][j];
        if (u == v) return res;
        for (int j = LOG - 1; j >= 0; --j)
            if (up[u][j] != up[v][j])
            {
                res = max({res, mx[u][j], mx[v][j]});
                u = up[u][j], v = up[v][j];
            }
        res = max({res, mx[u][0], mx[v][0]});
        return res;
    };

    vector<ll> ans(m);
    for (auto& e : sorted)
    {
        if (in_mst[e.id]) ans[e.id] = sum;
        else ans[e.id] = sum - path_max(e.u, e.v) + e.w;
    }

    for (int i = 0; i < m; ++i) cout << ans[i] << '\n';
}

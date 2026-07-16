#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge
{
    int u,v;
    ll w;
    bool operator < (const Edge& other) const{return w < other.w;}
};

struct DSU
{
    vector<int> parent,rank;
    DSU(int n)
    {
        parent.resize(n+1);
        rank.resize(n+1,0);
        for(int i = 1;i <= n;i++) parent[i] = i;
    }

    int find(int x)
    {
        if(parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x,int y)
    {
        int rx = find(x),ry = find(y);
        if(rx == ry) return false;
        if(rank[rx] < rank[ry]) swap(rx,ry);
        parent[ry] = rx;
        if(rank[rx] == rank[ry]) rank[rx]++;
        return true;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m;
    cin >> n >> m;

    vector<ll> a(n+1);
    int min_idx = 1;
    for(int i = 1;i <= n;i++)
    {
        cin >> a[i];
        if(a[i] < a[min_idx]) min_idx = i;
    }

    vector<Edge> edges;
    edges.reserve(n + m);

    for(int i = 0;i < m;i++)
    {
        int x,y;
        ll w;
        cin >> x >> y >> w;
        edges.push_back({x,y,w});
    }

    for(int i = 1;i <= n;i++)
    {
        if(i == min_idx) continue;
        edges.push_back({i,min_idx,a[i] + a[min_idx]});
    }

    sort(edges.begin(),edges.end());

    DSU dsu(n);
    ll ans = 0;
    int cnt = 0;

    for(const auto& e : edges)
    {
        if(dsu.unite(e.u,e.v))
        {
            ans += e.w;
            cnt++;
            if(cnt == n - 1) break;
        }
    }

    cout << ans << "\n";
    return 0;
}

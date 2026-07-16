#include <bits/stdc++.h>
using namespace std;

struct Edge
{
    int u,v,w;
};

struct DSU
{
    vector<int> parent,sz;

    DSU(int n)
    {
        parent.resize(n + 1);
        sz.assign(n + 1,1);
        for(int i = 1;i <= n;i++) parent[i] = i;
    }

    int find(int x)
    {
        if(parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int a,int b)
    {
        a = find(a);
        b = find(b);
        if(a == b) return false;
        if(sz[a] < sz[b]) swap(a,b);
        parent[b] = a;
        sz[a] += sz[b];
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
    }

    sort(edges.begin(),edges.end(),[](const Edge& a,const Edge& b){
        return a.w < b.w;
    });//排序取边Kruskal

    DSU dsu(n);
    int ans = 0;
    int i = 0;

    while(i < m)
    {
        int j = i;
        while(j < m && edges[j].w == edges[i].w) j++;//选取相同权值的边

        int good = 0;
        for(int k = i;k < j;k++)
        {
            int ru = dsu.find(edges[k].u);
            int rv = dsu.find(edges[k].v);
            if(ru != rv) good++;
        }//统计可加边

        int merged = 0;//统计实际加的边
        for(int k = i;k < j;k++)
        {
            if(dsu.unite(edges[k].u,edges[k].v)) merged++;
        }//未连接就连接

        ans += good - merged;

        i = j;
    }

    cout << ans << "\n";
}

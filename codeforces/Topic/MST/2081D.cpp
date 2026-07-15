#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DSU
{
    vector<int> fa,sz;
    DSU(int n)
    {
        fa.resize(n);
        sz.resize(n,1);
        for(int i = 0;i < n;i++) fa[i] = i;
    }

    int find(int x) {return fa[x] == x ? x : fa[x] = find(fa[x]);}

    bool unite(int x,int y)
    {
        x = find(x),y = find(y);
        if(x == y) return false;
        if(sz[x] < sz[y]) swap(x,y);
        fa[y] = x;sz[x] += sz[y];
        return true;
    }
};

void solve()
{
    int n;
    cin >> n;
    vector<int> p(n);
    for(int i = 0;i < n;i++) cin >> p[i];

    sort(p.begin(),p.end());

    vector<int> a;

    for(int i = 0;i < n;i++)
    {
        if(i == 0 || p[i] != p[i-1])
        {
            a.push_back(p[i]);
        }
    }//去重

    int k = a.size();
    if(k == 1)
    {
        cout << 0 << "\n";
        return;
    }

    int maxn = a.back();

    vector<int> val_to_idx(maxn + 2,-1);
    for(int i = 0;i < k;i++)
    {
        val_to_idx[a[i]] = i;
    }

    vector<int> next_exist(maxn + 2,maxn + 1);
    for(int x = maxn;x >= 0;x--)
    {
        if(val_to_idx[x] != -1) next_exist[x] = x;
        else next_exist[x] = next_exist[x + 1];
    }//预处理 查找大于x的最小的数

    vector<tuple<int,int,int>> edges;

    for(int i = 0;i < k;i++)
    {
        int v = a[i];
        for(int j = v;j <= maxn;j += v)
        {
            int start = (j == v) ? (v + 1) : j;
            if(start > maxn) break;

            int u = next_exist[start];
            if(u < j + v)
            {
                edges.emplace_back(u % v,i,val_to_idx[u]);
            }
        }
    }//筛查法找第一个数
    sort(edges.begin(),edges.end());
    DSU dsu(k);
    ll ans = 0;
    int cnt = 0;
    for(auto &e :edges)
    {
        int w = get<0>(e);
        int u = get<1>(e);
        int v = get<2>(e);
        if(dsu.unite(u,v))
        {
            ans += w;
            if(++cnt == k-1) break;
        }
    }//加入边 标准化kruskal

    cout << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) solve();
}

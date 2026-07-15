#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 2e5 + 5;
const int LOG = 20;

int n;
ll a[MAXN];
vector<pair<int,ll>> tree[MAXN];

int up[LOG][MAXN];
ll dist[MAXN];
int depth[MAXN];

void dfs_init(int u,int p,ll d)
{
    up[0][u] = p;
    depth[u] = (p == 0 ? 0 : depth[p] + 1);
    dist[u] = d;
    for(auto [v,w] : tree[u])
    {
        if(v == p) continue;
        dfs_init(v,u,d + w);
    }
}

void build_up()
{
    for(int k = 1;k < LOG;k++)
    {
        for(int u = 1;u <= n;u++)
        {
            int mid = up[k-1][u];
            if(mid != 0) up[k][u] = up[k-1][mid];
            else up[k][u] = 0;
        }
    }
}

int find_ancestor(int u,ll limit)
{
    if(limit <= 0) return 1;
    if(dist[u] < limit) return 0;
    int cur = u;
    for(int k = LOG-1;k >= 0;k--)
    {
        int nxt = up[k][cur];
        if(nxt != 0 && dist[nxt] >= limit)
        {
            cur = nxt;
        }
    }
    return cur;
}

ll diff[MAXN],ans[MAXN];

void dfs_solve(int u,int p)
{
    ll limit = dist[u] - a[u];
    int v = find_ancestor(u,limit);
    if(v != 0)
    {
        if(p != 0) diff[p]++;
        if(v != 1 && up[0][v] != 0)
        {
            diff[up[0][v]]--;
        }
    }

    for(auto [nxt,w] : tree[u])
    {
        if(nxt == p) continue;
        dfs_solve(nxt,u);
    }

    ans[u] = diff[u];
    for(auto [nxt,w] : tree[u])
    {
        if(nxt == p) continue;
        ans[u] += ans[nxt];
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for(int i = 1;i <= n;i++) cin >> a[i];
    for(int i = 2;i <= n;i++)
    {
        int p;ll w;
        cin >> p >> w;
        tree[p].push_back({i,w});
        tree[i].push_back({p,w});
    }

    dfs_init(1,0,0);
    build_up();
    dfs_solve(1,0);

    for(int i = 1;i <= n;i++)
    {
        cout << ans[i] << " ";
    }
    cout << "\n";
}

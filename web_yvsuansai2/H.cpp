#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"
const int N = 3005,p = 998244353;
int d[N],f[N];//d[i]距离为i的有序对
vector<int> g[N];
void dfs(int u,int fa,int x)
{
    d[x]++;
    for(int v : g[u])
    {
        if(v != fa)
        {
            dfs(v,u,x+1);
        }
    }
}//深搜统计有序对

void solve()
{
    int n;
    cin >> n;
    for(int i = 1;i <= n;i++) g[i].clear(),d[i] = 0; //初始化
    for(int i = 1;i < n;i++)
    {
        int u,v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }//建图
    for(int i = 1;i <= n;i++) dfs(i,0,1);
    int ans = 1;
    for(int i = 2;i <= n;i++) ans = (ans + (d[i]/2)*f[i])%p;
    cout << (ans+p)%p << endl;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    f[0] = 1;
    for(int i = 1;i <= 3000;i++) f[i] = f[i-1]*i%p;
    for(int i = 3000; i > 1;i--) f[i] = (f[i]-2*f[i-1]+f[i-2])%p;
    int T;
    cin >> T;
    while(T--) solve();
}

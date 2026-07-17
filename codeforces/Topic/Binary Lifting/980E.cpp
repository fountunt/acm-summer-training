#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 1e6 + 5;
const int LOG = 20;

vector<int> adj[N];
int dep[N];
int up[N][LOG];
int val[N][LOG];
int kept[N];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,k;
    cin >> n >> k;

    for(int i = 0;i < n-1;i++)
    {
        int a,b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> q;
    q.push_back(n);
    dep[n] = 0;
    up[n][0] = 0;
    for(int idx = 0;idx < (int)q.size();idx++)
    {
        int u = q[idx];
        for(int v : adj[u])
        {
            if(v == up[u][0]) continue;
            dep[v] = dep[u] + 1;
            up[v][0] = u;
            q.push_back(v);
        }
    }//dep + root 建树 深度和根节点

    for(int j = 1;j <= LOG;j++)
    {
        for(int i = 1;i <= n;i++)
        {
            int p = up[i][j-1];
            up[i][j] = p ? up[p][j-1] : 0;
        }
    }//倍增表

    kept[n] = true;//第n个节点要选
    int remain = n - k - 1;
    for(int i = n-1;i >= 1;i--)
    {
        if(kept[i]) continue;
        int v = i;
        for(int j = LOG;j >= 0;j--)
        {
            int anc = up[v][j];
            if(anc != 0 && !kept[anc]) v = anc;
        }//倍增查找公共祖先

        int anc = up[v][0];
        int need = dep[i] - dep[anc];
        if(need <= remain)
        {
            remain -= need;
            for(int cur = i;cur != anc;cur = up[cur][0])
            {
                kept[cur] = true;
            }
        }//保留整条路线 计算长度是否够
    }//贪心查找

    vector<int> removed;
    for(int i = 1;i <= n;i++)
    {
        if(!kept[i])
        {
            removed.push_back(i);
        }
    }

    for(int i = 0;i < k;i++)
    {
        cout << removed[i] << " ";
    }
    cout << endl;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int zo_mst(int n,const vector<vector<int>>& bad)
{
    set<int> uvis;
    for(int i = 1;i <= n;i++) uvis.insert(i);

    int comps = 0;
    while(!uvis.empty())
    {
        comps++;
        int s = *uvis.begin();
        uvis.erase(uvis.begin());
        queue<int> q;
        q.push(s);

        while(!q.empty())
        {
            int u = q.front();q.pop();
            vector<int> removed;
            for(int v : uvis)
            {
                if(!binary_search(bad[u].begin(),bad[u].end(),v))
                {
                    removed.push_back(v);
                }
            }
            for(int v : removed)
            {
                uvis.erase(v);
                q.push(v);
            }
        }
    }
    return comps - 1;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m;
    cin >> n >> m;

    vector<vector<int>> g(n+1);
    for(int i = 0;i < m;i++)
    {
        int a,b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    for(int i = 1;i <= n;i++)
    {
        sort(g[i].begin(),g[i].end());
    }

    int ans = zo_mst(n,g);

    cout << ans << "\n";
}

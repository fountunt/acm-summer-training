#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int n,m;

struct Edge
{
    int u,v;
    double base;
};

vector<Edge> edges;

bool SPFA(double w)
{
    vector<double> dist(n+1,0.0);
    for(int i = 1;i <= n;i++)
    {
        bool update = false;
        for(const auto &e : edges)
        {
            double nd = dist[e.u] + e.base + w;
            if(nd > dist[e.v])
            {
                dist[e.v] = nd;
                update = true;
                if(i == n) return true;
            }
        }
        if(!update) break;
    }
    return false;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    edges.reserve(m);

    for(int i = 0;i < m;i++)
    {
        int a,b,c,d;
        cin >> a >> b >> c >> d;
        edges.push_back({b,d,log((double)c/a)});
    }

    double l = 0.0,r = 1.0;
    for(int i = 0;i < 60;i++)
    {
        double mid = (l + r) / 2.0;
        if(SPFA(log(mid))) r = mid;
        else l = mid;
    }

    cout << fixed << setprecision(10) << l << "\n";
}

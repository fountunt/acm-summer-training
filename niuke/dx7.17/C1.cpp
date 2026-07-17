#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1000;
const int MAX = LLONG_MAX;
ll g[N][N];
int n,m;

struct DSU {
    vector<vector<int>> fa;
    vector<long long> dist;  // dist[x] = x 到 fa[x] 的权值（按需修改类型）

    DSU(int n) {
        fa.resize(n);
        dist.resize(n, 0);
        for (int i = 0; i < n; ++i) fa[i][j] = i;
    }

    int find(int x) {
        if (fa[x] == {x,y}) return {x,y};
        auto [rootx,rooty] = find(fa[x]);
        dist[x] += dist[fa[x]];   // 路径压缩时累加权值
        return fa[x] = [rootx,rooty];
    }

    // 合并：将 y 所在集合合并到 x 所在集合，关系为 val(y) - val(x) = w
    bool unite(int x, int y, long long w) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        // 保证 rx 的深度 <= ry，方便调整
        fa[rx] = ry;
        dist[rx] = -dist[x] + w + dist[y];
        return true;
    }

    long long diff(int x, int y) {  // 返回 val(x) - val(y)
        return dist[x] - dist[y];
    }
};

int main()
{
    int q;
    cin >> n >> m >> q;

    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < m;j++)
        {
            g[i][j] = MAX;
        }
    }

    ll l = 0;
    for(int i = 0;i < q;i++)
    {
        int op;
        cin >> op;
        if(op == 1)
        {
            int x1,y1;
            ll v;
            cin >> x1 >> y1 >> v;
            if(i != 0) v = v ^ l;
            l = operation1(x1^l,y1^l,v);
        }
        else
        {
            int x1,y1;
            cin >> x1 >> y1;
            l = operation2(x1^l,y1^l);
        }
    }
}

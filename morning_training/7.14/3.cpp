#include <bits/stdc++.h>
using namespace std;

struct DSU
{
    vector<int> fa;
    vector<int> sz;
    vector<vector<int>> members;

    DSU(int n)
    {
        fa.resize(n + 1);
        sz.resize(n + 1, 1);
        members.resize(n + 1);
        for (int i = 1; i <= n; i++)
        {
            fa[i] = i;
            members[i].push_back(i);
        }
    }

    int find(int x)
    {
        if (fa[x] != x) fa[x] = find(fa[x]);
        return fa[x];
    }

    // 将小集合合并到大集合，返回新的根
    int unite(int x, int y,
              vector<vector<int>>& adj,
              vector<unordered_map<int, int>>& cnt,
              vector<int>& distinct,
              vector<bool>& in_queue,
              queue<int>& q,
              vector<int>& k)
    {
        x = find(x);
        y = find(y);
        if (x == y) return y;
        if (sz[x] > sz[y]) swap(x, y); // 保证 x 是小集合

        fa[x] = y;
        sz[y] += sz[x];

        // 收集 x 中所有节点的后继（去重）
        unordered_set<int> affected;
        for (int u : members[x])
        {
            for (int v : adj[u])
            {
                affected.insert(v);
            }
        }

        // 更新每个受影响后继的计数器
        for (int v : affected)
        {
            auto& mp = cnt[v];
            if (!mp.count(x)) continue;
            int t = mp[x];
            mp.erase(x);

            int old_y = 0;
            if (mp.count(y)) old_y = mp[y];
            mp[y] += t;

            if (old_y > 0)
            {
                // 两个根合并为一个，distinct 减 1
                distinct[v]--;
            }
            // old_y == 0 时：x 消失，y 新增，distinct 不变

            if (distinct[v] == 1 && !in_queue[v] && k[v] > 0)
            {
                in_queue[v] = true;
                q.push(v);
            }
        }

        // 合并成员列表
        for (int u : members[x])
        {
            members[y].push_back(u);
        }
        members[x].clear();

        return y;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int case_num = 1; case_num <= T; case_num++)
    {
        int n;
        cin >> n;

        vector<int> k(n + 1);
        vector<vector<int>> pred(n + 1); // pred[i]: i 的依赖
        vector<vector<int>> adj(n + 1);  // adj[u]: u 的后继（依赖 u 的节点）

        for (int i = 1; i <= n; i++)
        {
            cin >> k[i];
            pred[i].resize(k[i]);
            for (int j = 0; j < k[i]; j++)
            {
                cin >> pred[i][j];
                adj[pred[i][j]].push_back(i);
            }
        }//输入前驱

        DSU dsu(n);
        vector<unordered_map<int, int>> cnt(n + 1);
        vector<int> distinct(n + 1, 0);
        vector<bool> in_queue(n + 1, false);
        queue<int> q;

        for (int i = 1; i <= n; i++)
        {
            if (k[i] == 0)
            {
                distinct[i] = 0;
                continue;
            }
            unordered_map<int, int> mp;
            for (int u : pred[i])
            {
                mp[u]++;
            }
            cnt[i] = move(mp);
            distinct[i] = (int)cnt[i].size();//后继节点数量
            if (distinct[i] == 1)
            {
                in_queue[i] = true;
                q.push(i);
            }
        }//初始化

        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            if (k[v] == 0) continue;

            int rv = dsu.find(v);
            // 找公共根 r
            int r = dsu.find(pred[v][0]);
            if (rv == r) continue;

            dsu.unite(rv, r, adj, cnt, distinct, in_queue, q, k);
        }//查找合并

        int ans = 0;
        for (int i = 1; i <= n; i++)
        {
            if (dsu.find(i) == i)
            {
                ans = max(ans, dsu.sz[i]);
            }
        }//取最大值

        cout << "Case #" << case_num << ": " << ans << "\n";
    }
    return 0;
}

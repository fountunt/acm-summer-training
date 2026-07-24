//An,n
//3 1 2
//[1,2,3] -> [3,1,2] g = 2
//[1,3,2] -> [2,1,3] g = 0
//[2,1,3] -> [3,2,1] g = 2
//[2,3,1] -> [1,2,3] g = 2
//[3,1,2] -> [2,3,1] g = 0
//[3,2,1] -> [1,3,2] g = 2
//每次做idx交换映射都能增加逆序的排列
//朴素做法:直接遍历所有的A 并求g(A,A`); O(nlgn*n!)
//g() 映射变换前后逆序的差值
//3 2 1
//[3,2,1] -> [1,2,3]
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 22;
const int MOD = 998244353;

ll dp[1 << MAXN];  // 全局，静态存储区，无栈溢出
int pre_mask[MAXN];
vector<int> g[MAXN];
int in[MAXN];

// flag = true : 最大化Delta的约束
// flag = false: 最小化Delta的约束
ll solve(int n, const vector<int>& pos, bool flag)
{
    fill(pre_mask, pre_mask + n, 0);
    for(int i = 0; i < n; i++)
    {
        g[i].clear();
        in[i] = 0;
    }

    for(int u = 0; u < n; u++)
    {
        for(int v = u + 1; v < n; v++)
        {
            int pu = pos[u];
            int pv = pos[v];
            if(flag)
            {
                // Max Δ约束
                if(pu < pv)
                {
                    // A_u > A_v  => v排在u前面  v→u
                    pre_mask[u] |= (1 << v);
                    g[v].push_back(u);
                    in[u]++;
                }
                else
                {
                    // A_u < A_v  => u排在v前面  u→v
                    pre_mask[v] |= (1 << u);
                    g[u].push_back(v);
                    in[v]++;
                }
            }
            else
            {
                // Min Δ约束（全部取反）
                if(pu < pv)
                {
                    // A_u < A_v  => u排在v前面 u→v
                    pre_mask[v] |= (1 << u);
                    g[u].push_back(v);
                    in[v]++;
                }
                else
                {
                    // A_u > A_v  => v排在u前面 v→u
                    pre_mask[u] |= (1 << v);
                    g[v].push_back(u);
                    in[u]++;
                }
            }
        }
    }

    // 拓扑排序判环
    queue<int> q;
    for(int i = 0; i < n; i++)
        if(in[i] == 0) q.push(i);
    int cnt = 0;
    while(!q.empty())
    {
        int u = q.front(); q.pop();
        cnt++;
        for(int v : g[u])
        {
            if(--in[v] == 0) q.push(v);
        }
    }
    if(cnt != n) return 0;

    fill(dp, dp + (1 << n), 0);
    dp[0] = 1;
    int full = (1 << n) - 1;
    for(int mask = 0; mask <= full; mask++)
    {
        if(dp[mask] == 0) continue;
        for(int x = 0; x < n; x++)
        {
            if(mask & (1 << x)) continue;
            if((mask & pre_mask[x]) == pre_mask[x])
            {
                dp[mask | (1 << x)] = (dp[mask | (1 << x)] + dp[mask]) % MOD;
            }
        }
    }
    return dp[full];
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> p(n), pos(n);
    for(int i = 0; i < n; i++)
    {
        int x; cin >> x;
        x--; // 转0-based
        p[i] = x;
        pos[x] = i;
    }

    ll cnt_max = solve(n, pos, true);
    ll cnt_min = solve(n, pos, false);
    ll ans = (cnt_max + cnt_min) % MOD;
    cout << ans << "\n";
    return 0;
}

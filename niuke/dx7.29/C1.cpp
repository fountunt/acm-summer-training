#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> A(n, 0);
    vector<vector<ll>> d(n, vector<ll>(m));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++) cin >> d[i][j];
        string s;
        cin >> s;
        for (int j = 0; j < m; j++)
        {
            if (s[j] == 'A') A[i] |= (1 << j);
        }//预处理转化为掩码
    }//输入数据

    vector<ll> cost((1 << m) * m, 0);//二维转一维数组存储时间
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cost[A[i] * m + j] += d[i][j];
        }
    }

    // SOS DP预处理求每个掩码下的贡献
    for (int b = 0; b < m; b++)
    {
        for (int mask = 0; mask < (1 << m); mask++)
        {
            if (mask & (1 << b))
            {
                int sub = mask ^ (1 << b);
                ll *src = &cost[mask * m];
                ll *dst = &cost[sub * m];
                for (int j = 0; j < m; j++) dst[j] += src[j];
            }
        }
    }

    vector<ll> dp(1 << m, INF);
    dp[0] = 0;
    for (int mask = 0; mask < (1 << m); mask++)
    {
        if (dp[mask] == INF) continue;

        int rem = ((1 << m) - 1) ^ mask;
        while (rem)
        {
            int lsb = rem & -rem;
            int j = __builtin_ctz(lsb);
            int new_mask = mask | lsb;
            ll val = dp[mask] + cost[mask * m + j];//取得改掩码下的需要加入的贡献
            dp[new_mask] = min(val,dp[new_mask]);
            rem ^= lsb;
        }
    }

    cout << dp[(1 << m) - 1] << "\n";
    return 0;
}

//R挑选一个测试 不会有多个R被测试
//A在R前面被测试则加入总时长
//找到一个测试顺序 使得A在R后面的总时长最长
//m = 20 考虑状压dp
//考虑测试点加入

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Dating
{
    vector<ll> times;
    string zt;
};

int main()
{
    int n,m;
    cin >> n >> m;
    vector<ll> dp(1 << m,-1);
    dp[0] = 0;

    vector<Dating> dating(n);
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < m;j++)
        {
            int ttime;
            cin >> ttime;
            dating[i].times.push_back(ttime);
        }
        cin >> dating[i].zt;
    }//读入数据

    //预处理把每种情况是否需要加入给列出来



    for(int mask = 0;mask < (1 << m);mask++) //2^m
    {
        for(int i = 0;i < m;i++)//m
        {
            int new_mask = mask | (1 << i);
            //选取第i个测试点
            ll new_time = dp[mask] + 1;
            //把第i个测试点时间加入

            dp[new_mask] = min(dp[new_mask],new_time);
        }
    }

    cout << dp[1 << m - 1] << "\n";
}

//2*n 分成n块
//dp[i] 设为第i块需要的代价
//dp[i] = dp[i-1] + (a[i] != b[i]);
//dp[i] = min(dp[i],dp[i-2] + (a[i-1] != a[i]) + (b[i-1] != b[i]))

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    int t;
    cin >> t;
    while(t--)
    {
        int n;
        cin >> n;
        string a,b;
        vector<int> dp(n+1,0);
        cin >> a >> b;
        a = ' ' + a;
        b = ' ' + b;

        for(int i = 1;i <= n;i++)
        {
            int cost = (a[i] != b[i]);
            dp[i] = dp[i-1] + (cost);

            if(i >= 2)
            {
                dp[i] = min(dp[i],dp[i-2] + (a[i-1] != a[i]) + (b[i-1] != b[i]));
            }
        }

        cout << dp[n] << "\n";
    }
}

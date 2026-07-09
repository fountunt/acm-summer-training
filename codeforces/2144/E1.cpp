#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;

void solve()
{
    int n;
    cin >> n;
    vector<int> a(n+1);
    for(int i = 1;i <= n;i++) cin >> a[i];

    vector<int> L(n+1),R(n+1);
    int cntL = 0,cntR = 0;

    ll cur = 0;
    for(int i = 1;i <= n;i++)
        if(a[i] > cur) {L[++cntL] = a[i];cur = a[i];}
    
    cur = 0;
    for(int i = n;i >= 1;i--)
        if(a[i] > cur) {R[++cntR] = a[i];cur = a[i];}

    int M = L[cntL];

    vector<vector<int>> dp1(n+1,vector<int>(cntL+1,0));
    dp1[0][0] = 1;
    for(int i = 1;i <= n;i++)
    {
        for(int j = 0;j <= cntL;j++)
        {
            dp1[i][j] = dp1[i-1][j];

            if(j > 0)
            {
                if(a[i] <= L[j])
                    dp1[i][j] = (dp1[i][j] + dp1[i-1][j]) % MOD;
                
                if(a[i] == L[j])
                    dp1[i][j] = (dp1[i][j] + dp1[i-1][j-1])%MOD;
            }
        }
    }

    vector<vector<int>> dp2(n + 2, vector<int>(cntR + 1, 0));
    dp2[n + 1][0] = 1;
    for(int i = n;i >= 1;i--)
    {
        for(int j = 0;j <= cntR;j++)
        {
            dp2[i][j] = dp2[i + 1][j];

            if(j > 0)
            {
                if(a[i] <= R[j])
                    dp2[i][j] = (dp2[i][j] + dp2[i+1][j])%MOD;

                if(a[i] == R[j])
                    dp2[i][j] = (dp2[i][j] + dp2[i+1][j-1])%MOD;
            }
        }
    }

    ll ans = 0;
    cur = 0;
    for(int i = 1;i <= n;i++)
    {
        if(a[i] == M)
        {
            ll Lw = dp1[i-1][cntL-1];
            ll Rw = dp2[i+1][cntR-1];
            ans = (ans + (cur + Lw) % MOD * Rw) % MOD;
            cur = (cur * 2 + Lw) % MOD;
        }
        else
        {
            cur = cur * 2 % MOD;
        }
    }
    cout << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}
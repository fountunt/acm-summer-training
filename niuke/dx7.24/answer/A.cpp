#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0;i < n;i++) cin >> a[i];

    static int cnt[29][2][2];
    memset(cnt,0,sizeof(cnt));
    int cnt1_29 = 0;

    for(int val : a)
    {
        for(int i = 0;i < 28;i++)
        {
            int hi = (val >> (i + 1)) & 1;
            int lo = (val >> i) & 1;
            cnt[i][hi][lo]++;
        }

        int hi = (val >> 29) & 1;
        int lo = (val >> 28) & 1;
        cnt[28][hi][lo]++;
        cnt1_29 += (val >> 29) & 1;
    }

    int m;
    cin >> m;
    while(m--)
    {
        int type,x;
        cin >> type >> x;

        for(int i = 0;i < 29;i++)
        {
            int x_hi = (x >> (i + 1)) & 1;
            int x_lo = (x >> i) & 1;

            int newcnt[2][2] = {0};
            for(int hi = 0;hi < 2;hi++)
            {
                for(int lo = 0;lo < 2;lo++)
                {
                    int val = cnt[i][hi][lo];
                    if(!val) continue;
                    int n_hi,n_lo;
                    if(type == 1)
                    {
                        n_hi = hi & x_hi;
                        n_lo = lo & x_lo;
                    }
                    else if(type == 2)
                    {
                        n_hi = hi | x_hi;
                        n_lo = lo | x_lo;
                    }
                    else
                    {
                        n_hi = hi ^ x_hi;
                        n_lo = lo ^ x_lo;
                    }
                    newcnt[n_hi][n_lo] += val;
                }
            }
            for(int hi = 0;hi < 2;hi++)
            {
                for(int lo = 0;lo < 2;lo++)
                {
                    cnt[i][hi][lo] = newcnt[hi][lo];
                }
            }
        }

        int x_29 = (x >> 29) & 1;
        if(type == 1) cnt1_29 = (x_29 ? cnt1_29 : 0);
        else if(type == 2) cnt1_29 = (x_29 ? n : cnt1_29);
        else cnt1_29 = (x_29 ? n - cnt1_29 : cnt1_29);

        ll sum_pop = 0,sum_adj = 0;
        for(int i = 0;i < 29;i++)
        {
            sum_pop += cnt[i][0][1] + cnt[i][1][1];
            sum_adj += cnt[i][1][1];
        }
        sum_pop += cnt1_29;
        ll ans = sum_pop - sum_adj;
        cout << ans << "\n";
    }
}
//位运算的桶性质

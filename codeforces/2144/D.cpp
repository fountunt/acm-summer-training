//选择折扣x(x直接遍历)
//计算可复用标签
//得到盈利

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    ll n,y;
    cin >> n >> y;
    vector<ll> c(n);
    ll max_c = 0;
    for(int i = 0;i < n;i++)
    {
        cin >> c[i];
        max_c = max(max_c,c[i]);
    }

    vector<ll> freq(max_c+1,0);
    for(int i = 0;i < n;i++) freq[c[i]]++;

    vector<ll> pref(max_c+1,0);
    for(int i = 1;i <= max_c;i++) pref[i] = pref[i-1] + freq[i];

    ll ans = LLONG_MIN;

    for(ll x = 2;x <= max_c;x++)
    {
        ll sum_new = 0,matches = 0;

        for(ll d = 1;(d-1)*x < max_c;d++)
        {
            ll l = (d-1) * x + 1;//左端点
            ll r = min(d*x,max_c);//右端点
            ll range = pref[r] - pref[l-1];//两端点内的商品
            if(range == 0) continue;//没有商品直接跳过

            sum_new += d * range;//计算价格
            if(d <= max_c) matches += min(range,freq[d]);//计算可以复用的标签
        }

        ll income = sum_new - y * (n-matches);
        ans = max(ans,income);
    }

    if(max_c > 1)
    {
        ll income = n - y * (n - freq[1]);
        ans = max(ans,income);
    }
    else
    {
        ans = n;
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
    //system("pause");
    return 0;
}

//本题用前缀和把区间内商品数量进行存储
//根据题意遍历折扣x
//直接计算区间内的商品的价格 和复用标签的数量进行计算总盈利
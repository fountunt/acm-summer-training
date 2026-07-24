//排序
//替换为中位数 即要让中位数尽可能大
//ai i < k/2 尽可能小
//把i>k/2 后面的数都作为一次中位数 直接取后面k/2个数来组成数组
//O(n)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int k,n;
    cin >> n >> k;
    vector<ll> a(n+1);
    for(int i = 1;i <= n;i++) cin >> a[i];
    sort(a.begin()+1,a.end());
    vector<ll> pre(n+1,0);
    for(int i = 1;i <= n;i++)  pre[i] = pre[i-1] + a[i];
    ll ans = 0;
    if(k & 1)
    {
        ll temp = 0;
        for(int i = (k+1)/2;i + k/2 <= n;i++)
        {
            temp += pre[i-1] - pre[k/2];
            temp += pre[n] - pre[i+k/2];
            temp += a[i]*k;
            ans = max(temp,ans);
            temp = 0;
        }
        cout << ans << "\n";
    }
    else
    {
        ll temp = 0;
        for(int i = k/2;i + k/2 <= n;i++)
        {
            temp += pre[i-1] - pre[k/2-1];
            temp += pre[n] - pre[i+k/2];
            temp += (a[i]+a[i+1])*k/2;
            ans = max(temp,ans);
            temp = 0;
        }
        cout << ans << "\n";
    }
}

int main()
{
    int T;
    cin >> T;
    while(T--) solve();
}

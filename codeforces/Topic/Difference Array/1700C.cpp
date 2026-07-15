#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

void solve()
{
    int n;
    cin >> n;

    vector<int> a(n);
    for(int i = 0;i < n;i++) cin >> a[i];

    vector<int> diff(n);
    diff[0] = a[0];
    for(int i = 1;i < n;i++)
    {
        diff[i] = a[i] - a[i-1];
    }

    int ans = 0;
    for(int i = 1;i < n;i++)
    {
        if(diff[i] > 0)
        {
            ans += diff[i];
        }//后面都减掉1
        else if(diff[i] < 0)
        {
            ans -= diff[i];
            diff[0] += diff[i];
        }//前面减掉1
    }

    ans += abs(diff[0]);
    cout << ans << endl;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}

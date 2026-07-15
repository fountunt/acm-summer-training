#include <bits/stdc++.h>
using namespace std;

void solve()
{
    int n,k;
    cin >> n >> k;
    vector<int> a(n);
    for(int i = 0;i < n;i++) cin >> a[i];
    vector<int> b(a[n-1]+1);
    for(int i = 0;i < n;i++) b[a[i]]++;

    sort(b.rbegin(),b.rend());
    
    int m = b.size();
    long long sum_f = 0;
    int ans = 0;

    for(int s = 1;s <= m;s++)
    {
        sum_f += b[s-1];
        int nxt = (s<m) ? b[s] : 0;

        if(b[s-1] <= nxt) continue;

        int min_f = b[s-1];
        if((sum_f - k) % s == 0)
        {
            long long P = (sum_f - k) / s;
            if(P < min_f) ++ans;
        }
    }

    cout << ans << "\n";
}

int main()
{
    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}
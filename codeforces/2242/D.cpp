#include <bits/stdc++.h>
using namespace std;

void solve()
{
    string a,b;
    cin >> a >> b;
    int n = a.size();
    int m = b.size();

    vector<int> pa(n+1,0),pb(m+1,0);
    for(int i = 0;i < n;i++)
    {
        pa[i+1] = (pa[i] + (a[i] - '0')) % 10;
    }
    for(int i = 0;i < m;i++)
    {
        pb[i+1] = (pb[i] + (b[i] - '0')) % 10;
    }

    if(pa[n] != pb[m])
    {
        cout << "-1\n";
        return;
    }

    vector<int> prev(m+1,0),cur(m+1,0);

    for(int i = 0;i <= n;i++)
    {
        for(int j = 0;j <= m;j++)
        {
            if(i == 0 && j == 0)
            {
                cur[j] = 0;
                continue;
            }

            int dp_val = 0;
            if(i > 0 && j > 0 && pa[i] == pb[j])
            {
                dp_val = 1 + prev[j - 1];
            }

            int best_val = dp_val;
            if(i > 0) best_val = max(best_val,prev[j]);
            if(j > 0) best_val = max(best_val,cur[j - 1]);

            cur[j] = best_val;
        }
        swap(prev,cur);
    }
    cout << prev[m] << "\n";
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
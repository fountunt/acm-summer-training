#include <bits/stdc++.h>
using namespace std;

int main()
{
    int T;
    cin >> T;
    while(T--)
    {
        int n,k;
        cin >> n >> k;
        string s;
        cin >> s;
        int ans = 0;
        if(k > n/2)
        {
            cout << -1 << "\n";
            continue;
        }
        for(int i = 0;i < k;i++)
        {
            if(s[i] == 'L') ans++;
            if(s[n-1-i] == 'R') ans++;
        }
        cout << ans << "\n";
    }
}

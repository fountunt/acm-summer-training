#include <bits/stdc++.h>
using namespace std;

void solve()
{
    int n;
    cin >> n;
    vector<int> a(n),b(n+1,0);
    for(int i = 0;i < n;i++) cin >> a[i];

    for(int i = 1;i <= n;i++) b[i] = a[i-1] + b[i-1];

    for(int i = 1;i < n;i++)
    {
        for(int j = i+1;j < n;j++)
        {
            if((b[i] % 3 != (b[j] - b[i])%3 && (b[n] - b[j])%3 != b[i] % 3 && (b[n] - b[j])%3 != (b[j] - b[i])%3) || (b[i] % 3 == (b[j] - b[i])%3 && (b[i] % 3 == (b[n]-b[j])%3)))
            {
                cout << i << " " << j << "\n";
                return;
            }
        }
    }
    cout << 0 << " " << 0 << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--) solve();
    //system("pause");
    return 0;
}
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
#define endl "\n"

void solve()
{
    int S;
    cin >> S;
    vector<int> F(8);
    for(int i = 1;i <= 7;i++) cin >> F[i];

    cout << min({
        S/3,
        F[1] + F[3] + F[5] + F[7],
        F[2] + F[3] + F[6] + F[7],
        F[4] + F[5] + F[6] + F[7],
        (S - F[4])/2,
        (S - F[2])/2,
        (S - F[1])/2
    }) << endl;
    
}

signed main()
{
    int T;
    cin >> T;
    while(T--) solve();
    return 0;
}
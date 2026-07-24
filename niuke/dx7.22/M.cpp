#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--)
    {
        long long n,m;
        cin >> n >> m;
        if(m >= n-1) cout << n*(n-1)/2-m << "\n";
        else cout << m*(m+1)/2-m << "\n";
    }
}

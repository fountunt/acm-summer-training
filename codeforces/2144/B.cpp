#include <bits/stdc++.h>
using namespace std;

void solve()
{
    int n;
    cin >> n;
    vector<int> a(n+1), used(n+1, 0), zero;
    for(int i = 1;i <= n;i++)
    {
        cin >> a[i];
        if(a[i] == 0) zero.push_back(i);
        else used[a[i]] = 1;
    }

    if(zero.size() == 1)
    {
        int unused = 1;
        while(used[unused]) unused++;
        a[zero[0]] = unused;
    }

    int l = 1,r = n;
    while(l <= n && a[l] == l) l++;
    while(r >= 1 && a[r] == r) r--;

    int result = r - l + 1;
    cout << max(0, result) << "\n";
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
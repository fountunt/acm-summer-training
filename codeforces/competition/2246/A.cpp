#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
#define endl "\n";

signed main()
{
    int t;
    cin >> t;
    while(t--)
    {
        int a = 1,b = 2;
        int n;
        cin >> n;
        for(int i = 1;i <= n;i++)
        {
            if(i & 1)
            {
                cout << b << " ";
                b += 2;
            }
            else
            {
                cout << a << " ";
                a += 2;
            }
        }
        cout << endl;
    }
}
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
#define endl "\n"

int a[200];

signed main()
{
    a[1] = 1;
    a[2] = 2;
    a[3] = 3;
    int t;
    cin >> t;
    while(t--)
    {
        int n;
        cin >> n;

        if (n == 2) 
        {
            cout << -1 << endl;
            continue;
        }

        if(n <= 3)
        {
            for(int i = 1;i <= n;i++) cout << a[i] << " ";
            cout << endl;
        }
        else 
        {
            cout << "1 2 3 ";
            int x = a[3]*2;
            for(int i = 4;i <= n;i++)
            {
                cout << x << " ";
                x *= 2;
            }
            cout << endl;
        }
    }
}
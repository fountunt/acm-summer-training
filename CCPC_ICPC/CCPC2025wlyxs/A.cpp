/**
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

int cal(int a,int b,int c)
{
    a = min(a,c);
    b = min(b,c);
    c = min(c,a+b);
    return (c-a)*a + (c-b)*b - (c-a)*(c-b) + (a+b-c)*(a+b-c-1)/2;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m;
    cin >> n >> m;
    for(int i = 0;i <= n;i++)
    {
        for(int j = 0;j <= m;j++)//遍历所有点进行计算
        {
            int a = n - i;
            int b = m - j;
            cout << cal(i,a,j)+cal(i,a,b)+cal(j,b,i)+cal(j,b,a)+min(i,j)+min(i,b)+min(a,j)+min(a,b);
            cout << " ";
        }
        cout << endl;
    }
}
**/


#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

signed main()
{
    int n,m;
    cin >> n >> m;
    n++,m++;
    vector<vector<int>> g(n+1,vector<int>(m+1,0));

    for(int a = 0;a < min(n,m);a++)
    {
        for(int b = 1;b + a < min(n,m);b++)
        {
            auto work = [&](int l,int u)
            {
                int r = m - a - b + l,d = n - b - a + u;
                g[u][l]++;
                g[u][r]--;
                g[d][l]--;
                g[d][r]++;
            };
            work(0,a);
            work(b,0);
            work(a,a+b);
            work(a+b,b);
        }
    }

    for(int i = 1;i < n;i++)
    {
        for(int j = 0;j < m;j++)
        {
            g[i][j] += g[i-1][j];
        }
    }

    for(int i = 0;i < n;i++)
    {
        for(int j = 1;j < m;j++)
        {
            g[i][j] += g[i][j-1];
        }
    }

    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < m;j++)
        {
            cout << g[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

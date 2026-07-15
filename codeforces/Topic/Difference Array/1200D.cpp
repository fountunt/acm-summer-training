#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

const int N = 2005;
char g[N][N];

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,k;
    cin >> n >> k;
    for(int i = 1;i <= n;i++)
    {
        for(int j = 1;j <= n;j++)
        {
            cin >> g[i][j];
        }
    }

    vector<int> Lrow(n+1,n+1),Rrow(n+1,0);
    vector<int> Ucol(n+1,n+1),Dcol(n+1,0);

    for(int i = 1;i <= n;i++)
    {
        for(int j = 1;j <= n;j++)
        {
            if(g[i][j] == 'B')
            {
                if(Lrow[i] == n + 1) Lrow[i] = j;
                Rrow[i] = j;
                if(Ucol[j] == n + 1) Ucol[j] = i;
                Dcol[j] = i;
            }
        }
    }

    int initial = 0;
    for(int i = 1;i <= n;i++)
    {
        if(Lrow[i] == n + 1) initial++;
        if(Ucol[i] == n + 1) initial++;
    }

    int rows = n - k + 1;
    int cols = n - k + 1;

    vector<vector<int>> diff(rows + 2,vector<int>(cols + 2, 0));

    auto addRect = [&](int r1,int c1,int r2,int c2)
    {
        if(r1 > r2 || c1 > c2) return;
        diff[r1][c1] += 1;
        diff[r2 + 1][c1] -= 1;
        diff[r1][c2 + 1] -= 1;
        diff[r2 + 1][c2 + 1] += 1;
    };

    for(int r = 1;r <= n;r++)
    {
        if(Lrow[r] == n+1) continue;

        int i1 = max(1ll,r-k+1);
        int i2 = min(r,rows);

        int j1 = max(1ll,Rrow[r] - k + 1);
        int j2 = min(Lrow[r], cols);

        addRect(i1,j1,i2,j2);
    }

    for(int c = 1; c <= n;c++)
    {
        if(Ucol[c] == n+1) continue;

        int j1 = max(1ll,c - k + 1);
        int j2 = min(c,cols);

        int i1 = max(1ll,Dcol[c] - k + 1);
        int i2 = min(Ucol[c], rows);

        addRect(i1,j1,i2,j2);
    }

    int ans = 0;
    for(int i = 1;i <= rows;i++)
    {
        for(int j = 1;j <= cols;j++)
        {
            diff[i][j] += diff[i-1][j] + diff[i][j-1] - diff[i-1][j-1];
            ans = max(ans,diff[i][j] + initial);
        }
    }

    cout << ans << endl;
    return 0;

}

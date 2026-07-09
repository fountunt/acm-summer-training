#include <bits/stdc++.h>
using namespace std;

const int N = 200005;

map<int,int> f;
struct node{int l,r;} a[N];

int find(int u){return f[u] == 0 ? u : (f[u] = find(f[u]));}

int main()
{
    int i,T,n,m,ans;
    cin >> T;
    while(T--)
    {
        cin >> n >> m;
        ans = m;
        f.clear();
        for(int i = 1;i <= n;i++) cin >> a[i].l >> a[i].r;
        sort(a+1,a+n+1,[](node a,node b){return a.r<b.r;});
        for(i = 1;i <= n;i++)
        {
            if(find(a[i].l) != a[i].r)
            {
                ans--;
                f[find(a[i].l)] = find(a[i].l)+1;
            }
        }
        cout << ans << endl;
    }
    return 0;
}
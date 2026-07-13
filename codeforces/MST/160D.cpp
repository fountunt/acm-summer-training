#include<bits/stdc++.h>
using namespace std;
#define int long long
using pii = pair<int, int>;
using ull = unsigned long long;
using lll=__int128;
#define debug(x) cout << #x << ":" << x << endl
constexpr int inf = 1e18;
constexpr int M =1e5+5;
int n,k;
int a[M];
int f[M];
int find(int x)
{
    if(f[x]!=x) f[x]=find(f[x]);
    return f[x];
}
void solve()
{
    cin>>n>>k;
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        a[i]%=k;
        f[i]=i;
    }
    sort(a+1,a+1+n);
    int ans=0;
    int ind=n+1;
    int last=n+1;
    int fs=n;
    for(int i=1;i<=n;i++)
    {
        while(ind-1>0&&a[ind-1]+a[i]>=k&&ind-1>i)
        {
            ind--;
        }
        if(ind<=i)
        {
            break;
        }
        for(int j=ind;j<=min(n,last);j++)
        {
            ans+=a[i]+a[j]-k;
        }
        if(ind<n+1&&fs==n)
        {
            fs=i;
        }
        last=ind;
    }
    for(int i=2;i<=fs;i++)
    {
        ans+=a[1]+a[i];
    }
    cout<<ans<<'\n';
}

signed main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    //freopen("ain.txt", "r", stdin),freopen("aout.txt", "w", stdout);
    int T=1;
    cin>>T;
    while(T--) solve();
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"
using ll = long long;
const int p = 998244353;

int qpow(int a,int b)
{
    int c = 1;
    while(b)
    {
        if(b & 1) c = c * a % p;
        a = a * a % p;
        b /= 2;
    }
    return c;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--)
    {
        int n,m;
        cin >> n >> m;
        if(n & 1) cout << qpow(qpow(2,m)-1,n-1) << endl;
        else cout << (qpow(qpow(2,m)-1,n-1) + qpow(1-qpow(2,m),n/2)+p)%p << endl;
    }
}

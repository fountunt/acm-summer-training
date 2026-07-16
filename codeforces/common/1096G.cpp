#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353,G = 3;

int modpow(int a,int e)
{
    int res = 1;
    for(;e;e >>= 1,a = (ll)a*a%MOD)
    {
        if(e&1) res = (ll)res * a %MOD;
    }
    return res;
}//快速幂

void ntt(vector<int>& a,bool invert)
{
    int n = a.size();
    for(int i = 1,j = 0;i < n;i++)
    {
        int bit = n >> 1;
        for(;j & bit;bit >>= 1) j^= bit;
        j ^= bit;
        if(i < j) swap(a[i],a[j]);
    }//数组重排得到FFT分治的底层顺序

    for(int len = 2;len <= n;len <<= 1)
    {
        int wlen = modpow(G,(MOD - 1)/len);
        if(invert) wlen = modpow(wlen,MOD-2);
        for(int i = 0;i < n;i += len)
        {
            int w = 1;
            for(int j = 0;j < len/2;j++)
            {
                int u = a[i+j];
                int v = (ll)a[i + j + len/2] * w % MOD;
                a[i + j] = (u + v) % MOD;
                a[i + j + len/2] = (u - v + MOD) % MOD;
                w = (ll)w * wlen % MOD;
            }
        }
    }//蝴蝶操作合并

    if(invert)
    {
        int n_inv = modpow(n,MOD-2);
        for(int &x : a) x = (ll)x * n_inv % MOD;
    }
}

vector<int> multiply(vector<int> a,vector<int> b)
{
    int n = 1;
    while(n < (int)a.size() + (int)b.size() - 1) n <<= 1;
    a.resize(n);b.resize(n);
    ntt(a,false);ntt(b,false);//FFT变换到点值
    for(int i = 0;i < n;i++) a[i] = (ll)a[i] * b[i] %MOD;//点乘
    ntt(a,true);//逆变换
    while(!a.empty() && a.back() == 0) a.pop_back();
    return a;
}//多项式相乘

vector<int> poly_pow(vector<int> base,int exp)
{
    vector<int> res = {1};
    while(exp)
    {
        if(exp & 1) res = multiply(res,base);
        base = multiply(base,base);
        exp >>= 1;
    }

    return res;
}//多项式乘法

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,k;cin >> n >> k;
    vector<int> base(10,0);
    for(int i = 0;i < k;i++)
    {
        int d;cin >> d;
        base[d] = 1;
    }

    auto poly = poly_pow(base,n/2);
    int ans = 0;
    for(int x : poly) ans = (ans + (ll)x*x)%MOD;
    cout << ans << "\n";
    return 0;
}
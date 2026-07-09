#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;
const int G = 3;

int modpow(int a,int e)
{
    int res = 1;
    while(e)
    {
        if(e & 1) res = (ll)res * a % MOD;
        a = (ll)a * a % MOD;
        e>>=1;
    }
    return res;
}

void ntt(vector<int>& a,bool invert)
{
    int n = a.size();
    for(int i = 1,j = 0;i < n;i++)
    {
        int bit = n >> 1;
        for(;j&bit;bit>>=1) j^=bit;
        j^=bit;
        if(i < j) swap(a[i],a[j]);
    }

    for(int len = 2;len <= n;len <<= 1)
    {
        int wlen = modpow(G,(MOD-1)/len);
        if(invert) wlen = modpow(wlen,MOD-2);
        for(int i = 0;i < n;i += len)
        {
            int w = 1;
            for(int j = 0;j < len / 2;j++)
            {
                int u = a[i+j];
                int v = (ll)a[i + j + len/2] * w % MOD;
                a[i + j] = (u + v) % MOD;
                a[i + j + len/2] = (u - v + MOD) % MOD;
                w = (ll)w * wlen % MOD;
            }
        }
    }
    if(invert)
    {
        int n_inv = modpow(n,MOD-2);
        for(int &x : a) x =(ll)x * n_inv % MOD;
    }
}//ntt

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s,t;
    cin >> s >> t;
    int n = s.size(),m = t.size();
    int total_pos = n - m + 1;

    int N = 1;
    while(N < n + m) N <<= 1;

    vector<ll> mask(total_pos,0);

    for(int x = 0;x < 6;x++)
    {
        for(int y = 0;y < 6;y++)
        {
            if(x == y) continue;

            vector<int> A(N,0);
            for(int i = 0;i < n;i++)
            {
                if(s[i] == 'a' + x)A[i] = 1;
            }

            vector<int> B(N,0);
            for(int i = 0;i < m;i++)
            {
                if(t[m-1-i] == 'a' + y) B[i] = 1;
            }//构造反转数组

            ntt(A,false);
            ntt(B,false);
            for(int i = 0;i < N;i++) A[i] = (ll)A[i] * B[i] % MOD;
            ntt(A,true);

            ll bit = 1LL << (x * 6 + y);
            for(int pos = 0;pos < total_pos;pos++)
            {
                if(A[pos + m - 1] > 0)
                {
                    mask[pos] |= bit;
                }
            }
        }
    }

    int parent[6];
    for(int pos = 0;pos < total_pos;pos++)
    {
        iota(parent,parent + 6,0);

        function<int(int)> find = [&](int x)
        {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };

        int ans = 0;
        ll msk = mask[pos];

        for(int x = 0;x < 6;x++)
        {
            for(int y = x + 1;y < 6;y++)
            {
                if((msk >> (x * 6 + y) & 1) || (msk >> (y*6 + x) & 1))
                {
                    int rx = find(x),ry = find(y);
                    if(rx != ry)
                    {
                        parent[rx] = ry;
                        ans++;
                    }
                }
            }
        }
        cout << ans << "\n"[pos == total_pos-1];
    }

    return 0;

}
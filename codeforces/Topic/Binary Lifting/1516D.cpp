#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXA = 1e5 + 5;
const int MAXN = 1e5 + 5;
const int LOG = 18;

int spf[MAXA + 5];
vector<int> primes;

void sieve()
{
    for(int i = 2;i <= MAXA;i++)
    {
        if(!spf[i])
        {
            spf[i] = i;
            primes.push_back(i);
        }
        for(int p : primes)
        {
            if(p > spf[i] || 1LL * i * p > MAXA) break;
            spf[i*p] = p;
        }
    }
}//欧拉筛

vector<int> getPrimeFactors(int x)
{
    vector<int> fac;
    while(x > 1)
    {
        int p = spf[x];
        fac.push_back(p);
        while(x % p == 0) x /= p;
    }
    return fac;
}//因子去重

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    sieve();

    int n,q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for(int i = 1;i <= n;i++) cin >> a[i];

    vector<int> R(n+2);
    vector<int> cnt(MAXA + 5,0);
    int right = 0;
    for(int left = 1;left <= n;left++)
    {
        while(right < n)
        {
            int x = a[right + 1];
            auto fac = getPrimeFactors(x);
            bool ok = true;
            for(int p : fac)
            {
                if(cnt[p] > 0){ok = false;break;}
            }
            if(!ok) break;
            right++;
            for(int p : fac) cnt[p]++;
        }
        R[left] = right;
        int x = a[left];
        auto fac = getPrimeFactors(x);
        for(int p : fac) cnt[p]--;
    }//双指针预处理

    int LOGN = 1;
    while((1 << LOGN) <= n) LOGN++;//动态标出上限
    LOGN+=2;//余量
    vector<vector<int>> up(LOGN ,vector<int>(n + 2,n + 1));//跳跃数组
    for(int i = 1;i <= n;i++) up[0][i] = R[i] + 1;
    up[0][n+1] = n+1;//初始化跳一步
    for(int k = 1;k < LOGN;k++)
    {
        for(int i = 1;i <= n + 1;i++)
        {
            up[k][i] = up[k-1][up[k-1][i]];
        }
    }//树上倍增数组初始化

    while(q--)
    {
        int l,r;
        cin >> l >> r;
        int cur = l;
        int ans = 0;
        for(int k = LOGN - 1;k >= 0;k--)
        {
            if(up[k][cur] <= r)
            {
                cur = up[k][cur];
                ans += (1 << k);
            }//树上倍增查询
        }
        cout << ans + 1 << "\n";
    }

    return 0;
}

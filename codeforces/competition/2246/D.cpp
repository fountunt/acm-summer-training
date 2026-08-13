//开始前任意+1
//bob 交换
//alice 找偶数前缀/2 或者a1 - 1
//-----------------------------
//bob 把奇数移动到第二位

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int bit_length(ll x)
{
    if(x == 0) return 0;
    return 64 - __builtin_clzll((unsigned long long)x);
}

ll calc(ll x)
{
    if(x == 0) return 0;
    return __builtin_popcountll((unsigned long long)x) + bit_length(x) - 1;
}//计算单个消去次数

void solve()
{
    int n;
    cin >> n;
    vector<ll> a(n);
    ll max_a = 0;
    for(int i = 0; i < n; i++)
    {
        cin >> a[i];
        max_a = max(max_a, a[i]);
    }

    ll ans = LLONG_MAX;

    // 计算最大需要的 k
    int max_k = 0;
    while((1LL << max_k) <= max_a + 32) max_k++;

    for(int k = 0; k <= max_k; k++)
    {
        ll total = k;  // k 次全局除以2
        ll pow2k = 1LL << k;

        for(int i = 0; i < n; i++)
        {
            // 找到 >= a[i] 的最小 2^k 倍数
            ll base = ((a[i] + pow2k - 1) / pow2k) * pow2k;
            ll best = LLONG_MAX;

            // 检查候选值
            for(ll cand = base; cand <= base + 32 * pow2k; cand += pow2k)
            {
                ll add_cost = cand - a[i];
                ll future_cost = calc(cand / pow2k);  // 关键修改：除以 2^k
                best = min(best, add_cost + future_cost);
            }

            total += best;
        }

        ans = min(ans, total);
    }

    cout << ans << "\n";
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while(T--) solve();
    return 0;
}

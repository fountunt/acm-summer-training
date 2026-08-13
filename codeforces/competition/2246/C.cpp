#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 1e9 + 7;

ll modpow(ll a, ll e)
{
    ll r = 1;
    while (e)
    {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;

        int neg = 0;
        map<ll, int> cnt;

        for (int i = 0; i < n; i++)
        {
            ll x;
            cin >> x;
            if (x == -1) neg++;
            else cnt[x]++;
        }

        int pos_cnt = 0;
        int distinct_cnt = 0;
        for (auto &p : cnt)
        {
            pos_cnt += p.second;
            distinct_cnt++;
        }

        ll ans0 = modpow(2, pos_cnt - distinct_cnt);

        ll pairs = 0;
        for (auto &p : cnt)
        {
            if (cnt.count(p.first + 1))
            {
                pairs++;
            }
        }
        ll ans1 = ans0 * pairs % MOD;

        ll even_neg, odd_neg;
        if (neg == 0)
        {
            even_neg = 1;
            odd_neg = 0;
        }
        else
        {
            ll p = modpow(2, neg - 1);
            even_neg = odd_neg = p;
        }

        ll ans = (even_neg * ans0 + odd_neg * ans1) % MOD;
        cout << ans << '\n';
    }
    return 0;
}

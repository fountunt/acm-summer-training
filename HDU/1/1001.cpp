#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--)
    {
        ll x, y;
        int n, m;
        cin >> x >> y >> n >> m;

        ll maxA = 0, minA = 0;
        for (int i = 0; i < n; ++i)
        {
            ll val;
            cin >> val;
            if (i == 0)
            {
                maxA = minA = val;
            }
            else
            {
                maxA = max(maxA, val);
                minA = min(minA, val);
            }
        }

        ll maxB = 0, minB = 0;
        for (int i = 0; i < m; ++i)
        {
            ll val;
            cin >> val;
            if (i == 0)
            {
                maxB = minB = val;
            }
            else
            {
                maxB = max(maxB, val);
                minB = min(minB, val);
            }
        }

        vector<ll> candidatesA = {0, maxA, minA};
        vector<ll> candidatesB = {0, maxB, minB};

        ll ans = LLONG_MIN;
        for (ll va : candidatesA)
        {
            for (ll vb : candidatesB)
            {
                ans = max(ans, (x + va) * (y + vb));
            }
        }

        cout << ans << '\n';
    }
    return 0;
}

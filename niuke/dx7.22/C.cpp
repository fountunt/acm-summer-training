#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll Lx(ll y, ll k)
{
    if (y < k - 1) return 0;
    if (y == k - 1) return 0;
    return y - 1;
}

ll Rx(ll y, ll k)
{
    if (y < k - 1) return k - 1;
    if (y == k - 1) return k;
    return y + 1;
}

bool valid(ll x, ll y, ll k)
{
    if (max(x, y) < k) return true;
    return abs(x - y) <= 1;
}

void solve()
{
    ll k, x1, y1, x2, y2;
    cin >> k >> x1 >> y1 >> x2 >> y2;

    ll dx = x2 - x1;
    ll dy = y2 - y1;

    ll min_len = (dx + dy) / (dy + 1);

    auto calc_max = [&]() -> ll
    {
        ll ans = 0;

        auto try_way = [&](bool first_x) -> ll
        {
            ll cx = x2, cy = y2;
            ll cur = 0;
            ll best = 0;

            if (!valid(cx, cy, k))
            {
                if (first_x)
                {
                    if (cx - 1 >= x1 && valid(cx - 1, cy, k))
                    {
                        cx--;
                        cur = 1;
                        best = 1;
                    }
                    else
                    {
                        return -1LL;
                    }
                }
                else
                {
                    if (cy - 1 >= y1 && valid(cx, cy - 1, k))
                    {
                        cy--;
                        cur = 0;
                    }
                    else
                    {
                        return -1LL;
                    }
                }
            }

            while (cx > x1 || cy > y1)
            {
                ll limit = max(x1, Lx(cy, k));
                if (cx > limit)
                {
                    ll steps = cx - limit;
                    cx = limit;
                    cur += steps;
                    best = max(best, cur);
                }

                if (cx == x1 && cy > y1)
                {
                    cy--;
                    cur = 0;
                    if (!valid(cx, cy, k)) return -1LL;
                }
                else if (cx > x1)
                {
                    if (cy > y1)
                    {
                        cy--;
                        cur = 0;
                        if (!valid(cx, cy, k)) return -1LL;
                    }
                    else
                    {
                        return -1LL;
                    }
                }
                else
                {
                    break;
                }
            }

            if (cx == x1 && cy == y1) return best;
            return -1LL;
        };

        ll res1 = try_way(true);
        if (res1 != -1) ans = max(ans, res1);
        ll res2 = try_way(false);
        if (res2 != -1) ans = max(ans, res2);

        return ans;
    };

    ll max_len = calc_max();
    cout << min_len << " " << max_len << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}

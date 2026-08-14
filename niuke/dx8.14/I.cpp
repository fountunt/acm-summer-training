#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128_t;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll x, a, b;
    cin >> x >> a >> b;

    ll ans = (x + b - 1) / b;
    if(a == 1){
        cout << ans << "\n";
        return 0;
    }

    auto S1 = [&](ll k)->i128{
        return (i128)(a - 1) * k * (k + 1) / 2 + k;
    };

    auto D = [&](ll k, ll t)->i128{
        i128 m = (i128)k * (a - 1);
        i128 res = (i128)b * t + S1(k);
        ll s = min(t, (ll)m);
        res += (i128)s * m - (i128)s * (s - 1) / 2;
        return res;
    };

    for(ll k = 0; ; ++k){
        if(k > ans) break;
        if(S1(k) >= x){ ans = min(ans, k); break; }
        ll hi = ans - k;
        if(D(k, hi) < x) continue;
        ll lo = 0;
        while(lo < hi){
            ll mid = lo + (hi - lo) / 2;
            if(D(k, mid) >= x) hi = mid;
            else lo = mid + 1;
        }
        ans = min(ans, k + lo);
    }
    cout << ans << "\n";
    return 0;
}

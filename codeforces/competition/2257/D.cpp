#include <bits/stdc++.h>
using namespace std;
using i128 = __int128;
using ll = long long;

void print_i128(i128 x){
    if(x == 0) {cout << '0'; return;}
    if(x < 0){cout << '-';x = -x;}
    string s;
    while(x > 0){
        int digit = (int)(x % 10);
        s.push_back(char('0' + digit));
        x /= 10;
    }
    reverse(s.begin(),s.end());
    cout << s;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while(t--){
        ll S;
        int q;
        cin >> S >> q;

        vector<ll> divs;
        for(ll i = 1;i * i <= S;i++){
            if(S % i == 0){
                divs.push_back(i);
                if(i != S / i) divs.push_back(S / i);
            }
        }

        sort(divs.begin(),divs.end());

        int m = (int)divs.size();
        vector<ll> pref(m);
        ll prevD = 0;

        for(int k = 0;k < m;k++){
            ll h = S / divs[k];
            ll len = divs[k] - prevD;
            i128 add = (i128)h * len;
            if(k == 0) pref[k] = (ll)add;
            else pref[k] = (ll)((i128)pref[k-1] + add);
            prevD = divs[k];
        }

        auto sumH = [&](ll x) -> ll{
            int idx = (int)(lower_bound(divs.begin(),divs.end(),x) - divs.begin());
            i128 res = (idx > 0 ? (i128)pref[idx-1] : 0);
            ll prev = (idx > 0 ? divs[idx - 1] : 0);
            ll h = S / divs[idx];
            res += (i128)h * (x - prev);
            return (ll)res;
        };

        while(q--){
            ll x,y;
            cin >> x >> y;

            ll M = S / y;
            int pos = (int)(upper_bound(divs.begin(),divs.end(),M) - divs.begin() - 1);
            ll i0 = divs[pos];
            ll sum_i0 = pref[pos];

            i128 ans;
            if(x <= i0){
                ans = (i128)y * x;
            } else {
                ll sum_x = sumH(x);
                ans = (i128)y * i0 + (i128)(sum_x - sum_i0);
            }

            print_i128(ans);
            cout << "\n";
        }
    }
    return 0;
}

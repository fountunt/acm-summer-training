#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll upb(ll k,ll b){
    return k*b*b + k*b;
}

ll lob(ll k,ll b){
    return k*b*b + k*b - b;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    ll ans = (n + 1) / 2;
    for(ll b = 2; lob(1, b) <= n; b++){
        ll D = b * (b + 1);
        ll full = n / D;
        ans += full * b;
        ll k = full + 1;
        if(lob(k, b) <= n)
            ans += (n - lob(k, b)) + 1;
    }
    cout << ans << "\n";
    return 0;
}

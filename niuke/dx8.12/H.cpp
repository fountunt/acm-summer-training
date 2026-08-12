#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using int128 = __int128;
const int MOD = 998244353;

void solve(){
    ll n,x;
    cin >> n >> x;
    vector<ll> a(n);
    for(ll i = 0;i < n;i++){
        cin >> a[i];
    }

    if(x == 1){
        ll ans = 0;
        for(int i = 0;i < n;i++){
            ans += a[i];
            ans %= MOD;
        }
        cout << ans << "\n";
        return;
    }

    int128 sy_energe = 0;
    for(auto& num : a){
        sy_energe += (num/x);
        num = num % x;
    }

    sort(a.begin(),a.end(),greater<ll>());

    int cur = 0;
    while(true){
        if(cur == n){//剩余能量过多时
            while(true){
                if(sy_energe < x-1) break;
                if(sy_energe == x-1) {sy_energe = 0;break;}
                int128 temp = sy_energe / x;
                sy_energe %= x;
                sy_energe += temp;
            }
            break;
        }

        if(a[cur] + sy_energe + 1 < x){//剩余能量不足以支持下一次释放
            break;
        }

        sy_energe -= (x - (a[cur] + 1));//若支持则释放
        a[cur] = 0;

        cur++;
    }

    ll ans = 0;
    for(int i = 0;i < n;i++){
        ans += a[i];
        ans %= MOD;
    }

    ans = (ans + sy_energe) % MOD;
    cout << ans << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) solve();
}

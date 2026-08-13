#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 998244353;
const int MAXN = 1e6 + 5;
ll fact[MAXN],invfact[MAXN];

ll modpow(ll a,ll b){
    ll r = 1;
    while(b){
        if(b & 1) r = r * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return r;
}

void init(){
    fact[0] = 1;
    for(int i = 1;i <= MAXN;i++) fact[i] = fact[i-1] * i % MOD;
    invfact[MAXN] = modpow(fact[MAXN],MOD-2);
    for(int i = MAXN;i >= 1;i--) invfact[i-1] = invfact[i] * i % MOD;
}

ll C(int n,int k){
    if(k < 0 || k > n) return 0;
    return fact[n] * invfact[k] % MOD * invfact[n-k] % MOD;
}

void solve(){
    int n;
    string s;
    cin >> n >> s;

    int cnt[2] = {0,0},runs[2] = {0,0};
    for(int i = 0;i < n;i++){
        cnt[s[i] - '0']++;
        if(i == 0 || s[i] != s[i-1]) runs[s[i] - '0']++;
    }

    ll ans = 1;
    for(int c = 0;c < 2;c++){
        if(runs[c] > 0) ans = ans * C(cnt[c]-1,runs[c]-1) % MOD;
    }
    cout << ans << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    init();
    int t;
    cin >> t;
    while(t--) solve();

}

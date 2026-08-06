#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
const long long MOD = 998244353;

int pc(ull x) {return __builtin_popcountll(x);}

ll blockSum(ull h,int k,ull d){
    ll a = pc(h) % MOD;
    ull H = h + d;
    ull H_high = H >> k;

    ll cnt[2] = {0},pct[2] = {0},pcs[2] = {0},prod[2] = {0};
    cnt[0] = 1;
    for(int p = 0;p < k;p++){
        ll db = (d >> p) & 1ull;
        ll nc[2] = {0},np[2] = {0},ns[2] = {0},npr[2] = {0};
        for(int c = 0;c < 2;c++) for(int tb = 0;tb < 2;tb++){
            ll s3 = tb + db + c;
            ll s = s3 & 1;
            int c2 = (s3 >= 2);
            nc[c2] = (nc[c2] + cnt[c]) % MOD;
            np[c2] = (np[c2] + pct[c] + tb*cnt[c]) % MOD;
            ns[c2] = (ns[c2] + pcs[c] + s*cnt[c]) % MOD;
            npr[c2] = (npr[c2] + prod[c] + tb*pcs[c] + s*pct[c] + (tb&s)*cnt[c]) % MOD;
        }
        memcpy(cnt,nc,sizeof cnt);memcpy(pct,np,sizeof pct);
        memcpy(pcs,ns,sizeof pcs);memcpy(prod,npr,sizeof npr);
    }
    ll res = 0;
    for(int c = 0;c < 2;c++){
        if(cnt[c] == 0 && pct[c] == 0 && pcs[c] == 0 && prod[c] == 0) continue;
        ll b = pc(H_high + c) % MOD;
        res = (res + a * pcs[c] + pct[c] * b + a*b*cnt[c] + prod[c]) % MOD;
    }
    return res;
}

ll solve(ull n,ull d){
    ll ans = (ll)((ull)pc(n) * pc(n+d)) % MOD;

    for(int k = 0;k < 60;k++){
        if((n >> k) & 1ull){
            ull h = (n >> (k+1)) << (k+1);
            ans = (ans + blockSum(h,k,d)) % MOD;
        }
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;cin >> t;
    while(t--){
        ull n,d;cin >> n >> d;
        cout << solve(n,d) << "\n";
    }
}

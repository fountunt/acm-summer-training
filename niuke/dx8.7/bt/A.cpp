#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll nxt(ll m,ll B,ll f){
    if(m < 0) return f;
    if((m & ~B) == f) return m;
    for(int i = 0;i <= 29;i++){
        if((m >> i & 1)) continue;
        if(((m >> (i + 1)) & ~(B >> (i+1))) != ((f >> (i + 1)) & ~(B >> (i + 1)))) continue;
        if(!(B >> i & 1) && !(f >> i & 1)) continue;
        ll v = (m & ~((1LL << (i + 1)) - 1));
        v |= (1LL << i);
        v |= (f & ~B & ((1LL << i) - 1));
        return v;
    }
    return -1;
}

bool feasible(ll B,const vector<int>& a){
    ll m = -1;
    for(int x : a){
        ll v = nxt(m,B,(ll)x & ~B);
        if(v < 0) return false;
        m = v;
    }
    return true;
}

void solve(){
    int n;cin >> n;
    vector<int> a(n);
    for(int i = 0;i < n;i++) cin >> a[i];

    ll M = (1LL << 30) - 1;
    for(int b = 29;b >= 0;b--){
        if(feasible(M & ~(1LL << b),a));
            M &= ~(1LL << b);
    }
    cout << M << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--) solve();
}

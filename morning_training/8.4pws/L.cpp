#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N,D,S;
    cin >> N >> D >> S;

    ll Mmax = min(N / S,D / S + 1);
    ll Gmax = S * Mmax;

    ll best = 0;

    for(ll g = 1;g <= Gmax;){
        ll qN = N / g,qD = D / g;
        ll hi = Gmax;
        if(qN > 0) hi = min(hi,N/qN);
        if(qD > 0) hi = min(hi,D/qD);
        ll cand = (hi/S)*S;
        if(cand >= g){
            ll k = min(qN,qD+1);
            best = max(best,cand * k);
        }
        g = hi + 1;
    }
    cout << best << "\n";
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Blk{
    ll c,s;
};

const ll NONE = LLONG_MIN / 4;

ll floordiv(ll a,ll b){
    ll q = a/b,r = a % b;
    if(r < 0) q--;
    return q;
}

array<ll,3> span1(const Blk& b){
    ll c = b.c,s = b.s;
    ll T = c*(c-1)/2;
    ll L = floordiv(s - T,c);
    ll r = s - (c*L + T);
    if(r == 0) return {L,L+c-1,NONE};
    else return {L,L+c,L+c-r};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while(T--){
        int n;
        cin >> n;
        vector<ll> a(n);
        for(int i = 0;i < n;i++) cin >> a[i];
        sort(a.begin(),a.end());

        vector<Blk> st;
        for(ll p : a){
            st.push_back({1,p});
            while(st.size() >= 2){
                auto sp1 = span1(st[st.size() - 2]);
                auto sp2 = span1(st.back());
                if(max(sp1[0],sp2[0]) <= min(sp1[1],sp2[1])){
                    Blk b1 = st[st.size() - 2], b2 = st.back();
                    st.pop_back(); st.pop_back();
                    st.push_back({b1.c + b2.c,b1.s + b2.s});
                } else break;
            }
        }
        vector<array<ll,3>> blks;
        for(auto &b : st) blks.push_back(span1(b));
        int idx = 0;
        ll k = 0;
        while(true){
            while(idx < (int)blks.size() && blks[idx][1] < k) idx++;
            if(idx >= (int)blks.size() || blks[idx][0] > k){
                cout << k << "\n"; break;
            }
            ll L = blks[idx][0],R = blks[idx][1],g = blks[idx][2];
            bool absent = false;
            if(k == g) absent = true;
            else if((L < k-1 && k-1 < R) || (L < k+1 && k+1 < R))
                absent = true;
            if(absent) {cout << k << "\n"; break;}
            k++;
        }
    }
    return 0;
}

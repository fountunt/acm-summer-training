#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll floordiv(ll a, ll b){ // b>0, floor(a/b)
    ll q = a / b, r = a % b;
    if (r < 0) q--;
    return q;
}

struct Blk { ll c, s; };
const ll NONE = LLONG_MIN / 4;   // 表示无缺口

// 稳定块的形态:返回 {L, R, gap};gap 在 [L,R] 内则为缺口,否则无缺口
// 块内只记点数 c 与位置和 s,稳定形态是连续区间挖去至多一个位置
array<ll,3> span(const Blk& b){
    ll c = b.c, s = b.s;
    ll T = c*(c-1)/2;
    ll L = floordiv(s - T, c);
    ll r = s - (c*L + T);          // 0 <= r < c
    if (r == 0) return {L, L + c - 1, NONE};
    else        return {L, L + c, L + c - r};
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
        for(int i=0;i<n;i++) cin >> a[i];
        sort(a.begin(), a.end());
        // 栈合并:逐点形成单点块,外包区间相交则合并,得到全局唯一稳定态
        vector<Blk> st;
        for(ll p : a){
            st.push_back({1, p});
            while(st.size() >= 2){
                auto sp1 = span(st[st.size()-2]);
                auto sp2 = span(st.back());
                if(max(sp1[0],sp2[0]) <= min(sp1[1],sp2[1])){   // 外包区间相交 -> 合并
                    Blk b1 = st[st.size()-2], b2 = st.back();
                    st.pop_back(); st.pop_back();
                    st.push_back({b1.c + b2.c, b1.s + b2.s});
                }else break;
            }
        }
        vector<array<ll,3>> blks;
        for(auto &b : st) blks.push_back(span(b));
        // 从 k=0 向上扫描,k 可缺失即为答案
        int idx = 0;
        ll k = 0;
        while(true){
            while(idx < (int)blks.size() && blks[idx][1] < k) idx++;
            if(idx >= (int)blks.size() || blks[idx][0] > k){   // k 不在任何块中
                cout << k << "\n"; break;
            }
            ll L = blks[idx][0], R = blks[idx][1], g = blks[idx][2];
            bool absent = false;
            if(k == g) absent = true;                          // 恰为块的缺口
            else if((L < k-1 && k-1 < R) || (L < k+1 && k+1 < R)) // 邻位在块的严格内部
                absent = true;                                 // 该位置稳定化中曾点火,可消去 k
            if(absent){ cout << k << "\n"; break; }
            k++;
        }
    }
    return 0;
}

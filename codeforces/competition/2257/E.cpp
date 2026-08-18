#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Seg{
    ll need,gain;
    int len,id;
    bool operator>(const Seg& o) const {
        return need > o.need;
    }
};

void solve(){
    int n; ll x;
    cin >> n >> x;
    vector<vector<ll>> a(n),b(n);
    vector<vector<Seg>> segs(n);

    for(int i = 0;i < n;i++){
        int m;cin >> m;
        a[i].resize(m);
        b[i].resize(m);
        for(int j = 0;j < m;j++) cin >> a[i][j];
        for(int j = 0;j < m;j++) cin >> b[i][j];

        ll sum = 0,need = 0;
        int len = 0;
        for(int j = 0;j < m;j++){
            need = max(need,a[i][j] - sum);
            sum += b[i][j] - a[i][j];
            len++;
            if(sum >= 0){
                segs[i].push_back({need,sum,len,i});
                sum = 0;
                need = 0;
                len = 0;
            }
        }
    }

    priority_queue<Seg,vector<Seg>,greater<Seg>> pq;
    vector<int> ptr(n,0),built(n,0);
    for(int i = 0;i < n;i++){
        if(!segs[i].empty()) pq.push(segs[i][0]);
    }

    ll cap = x;
    while(!pq.empty()){
        Seg s = pq.top();
        if(s.need > cap) break;
        pq.pop();
        cap += s.gain;
        built[s.id] += s.len;
        int id = s.id;
        ptr[id]++;
        if(ptr[id] < (int)segs[id].size()){
            pq.push(segs[id][ptr[id]]);
        }
    }

    ll bestH = -1;
    int bestId = 0;
    for(int i = 0;i < n;i++){
        ll cur = cap;
        int h = built[i];
        for(int j = built[i];j < (int)a[i].size();j++){
            if(cur >= a[i][j]){
                cur += b[i][j] - a[i][j];
                h++;
            } else break;
        }
        if(h > bestH || (h == bestH && i < bestId)){
            bestH = h;
            bestId = i;
        }
    }
    cout << bestH << " " << bestId + 1 << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;cin >> T;
    while(T--) solve();
    return 0;
}

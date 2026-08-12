#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    ll n,m;
    cin >> n >> m;

    vector<ll> k(n);
    vector<vector<ll>> g(n,vector<ll>(m));
    for(int i = 0;i < n;i++) cin >> k[i];
    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
            cin >> g[i][j];
        }
    }

    priority_queue<ll> pq;

    int ans = m;
    for(int i = n-1;i >= 0;i--){
        for(int j = 0 ;j < m;j++){
            pq.push(g[i][j]);
        }

        vector<ll> temp;
        for(int j = 0;j < m;j++){
            temp.push_back(pq.top());
            k[i] -= pq.top();
            pq.pop();
            if(k[i] <= 0){
                ans = min(j+1,ans);
                break;
            }
        }

        for(auto num : temp){
            pq.push(num);
        }
    }

    cout << ans << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int maxn = 2e5 + 5;
vector<int> g[maxn];
int depth[maxn],dp[maxn];
ll ans[maxn];

void dfs(int v,int p = -1){
    ans[v] = 0;
    dp[v] = depth[v];
    int m1 = depth[v],m2 = depth[v];
    for(auto u : g[v]){
        if(u != p){
            depth[u] = depth[v] + 1;
            dfs(u,v);
            dp[v] = max(dp[v],dp[u]);
            ans[v] += ans[u];
            if(dp[u] >= m1){
                m2 = m1;
                m1 = dp[u];
            } else if (dp[u] >= m2){
                m2 = dp[u];
            }
        }
    }
    ans[v] += m2 - depth[v] + 1;
}

void solve(){
    int n;
    cin >> n;
    for(int i = 1;i < n;i++){
        int p;
        cin >> p;
        g[p-1].push_back(i);
    }
    dfs(0);
    cout << ans[0] << "\n";
    for(int i = 0;i < n;i++){
        g[i].clear();
    }

}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;cin >> t;
    while(t--) solve();
    return 0;
}

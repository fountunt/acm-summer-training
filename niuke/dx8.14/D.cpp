#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n+1);
    for(int i=0;i<n-1;i++){
        int u,v; cin>>u>>v;
        adj[u].push_back(v); adj[v].push_back(u);
    }
    vector<int> dep(n+1,0), par(n+1,0);
    vector<int> pre;                     // 前序,逆序即"孩子先于父亲"的自底向上顺序
    {   // 迭代 DFS 求深度与前序
        vector<int> stk{1};
        while(!stk.empty()){
            int u=stk.back(); stk.pop_back();
            pre.push_back(u);
            for(int v: adj[u]){
                if(v==par[u]) continue;
                par[v]=u; dep[v]=dep[u]+1; stk.push_back(v);
            }
        }
    }
    vector<vector<int>> starters(n+1);   // 在该结点出生的人
    vector<ll> key(m);
    for(int i=0;i<m;i++){
        int x; ll s;
        cin >> x >> s;
        key[i] = s + dep[x];             // key_i = s_i + dep[x_i]
        starters[x].push_back(i);
    }
    vector<char> dead(m,0);
    vector<map<ll,int>> mp(n+1);         // key -> 存活者
    for(int i=n-1;i>=0;i--){             // 自底向上
        int u = pre[i];
        int big = 0;
        for(int v: adj[u]) if(v!=par[u]){
            if(big==0 || mp[v].size()>mp[big].size()) big=v;   // 小并大:取最大的孩子
        }
        map<ll,int> local;
        if(big) local.swap(mp[big]);     // 大孩子的 map 直接拿来做基础
        set<ll> bad;                     // 在本结点已碰撞的 key(防止第三人被误留)
        auto add = [&](ll k, int p){
            if(bad.count(k)){ dead[p]=1; return; }
            auto it = local.find(k);
            if(it != local.end()){       // 同 key 相遇 -> 双双消失
                dead[it->second] = 1;
                dead[p] = 1;
                local.erase(it);
                bad.insert(k);
            }else{
                local[k] = p;
            }
        };
        for(int v: adj[u]) if(v!=par[u] && v!=big){
            for(auto &pr : mp[v]) add(pr.first, pr.second);
            mp[v].clear();
        }
        for(int p : starters[u]) add(key[p], p);
        mp[u] = move(local);
    }
    string out(m,'0');
    for(auto &pr : mp[1]) out[pr.second]='1';   // 根处理完仍在 map 中的人全部逃脱
    cout << out << "\n";
    return 0;
}

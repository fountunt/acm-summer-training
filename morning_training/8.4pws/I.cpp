#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin >> n;
    vector<vector<pair<int,int>>> adj(n+1);
    vector<pair<int,int>> raw;
    for(int i = 0;i < n-1;i++){
        int u,v;
        cin >> u >> v;
        raw.push_back({u,v});
        adj[u].push_back({v,+1});
        adj[v].push_back({u,-1});
    }

    vector<int> parent(n+1,0),parDir(n+1,0),order;
    vector<int> st = {1};
    parent[1] = -1;
    while(!st.empty()){
        int u = st.back();st.pop_back();
        order.push_back(u);
        for(auto& [v,dr] : adj[u]){
            if(v == parent[u]) continue;
            parent[v] = u;
            parDir[v] = dr;
            st.push_back(v);
        }
    }
    vector<vector<int>> children(n+1);
    for(int v = 1;v <= n;v++){
        if(parent[v] > 0) children[parent[v]].push_back(v);
    }

    vector<int> f(n+1,0),inu(n+1,0),outu(n+1,0);
    vector<int> moChild(n+1,0),miChild(n+1,0);
    for(int it = (int)order.size()-1;it >= 0;it--){
        int u = order[it];
        int base = 0,mo = 0,mi = 0;
        for(int c : children[u]){
            base += f[c];
            if(parDir[c] == +1){
                int imp = (1 + inu[c]) - f[c];
                if(imp > mo) mo = imp,moChild[u] = c;
            }else{
                int imp = (1 + outu[c]) - f[c];
                if(imp > mi) mi = imp,miChild[u] = c;
            }
        }
        f[u] = base + mo + mi;
        inu[u] = base + mo;
        outu[u] = base + mi;
    }
    int matching = f[1];
    int chains = n - matching;
    int K = chains - 1;

    vector<int> outSel(n+1,0),inSel(n+1,0);
    vector<int> state(n+1,0);
    state[1] = 0;
    queue<int> q;
    q.push(1);
    while(!q.empty()){
        int u = q.front();q.pop();
        int s = state[u];
        for(int c : children[u]){
            if(parDir[c] == +1){
                int imp = (1 + inu[c]) - f[c];
                if(c == moChild[u] && imp > 0 && s != 2){
                    outSel[u] = c;inSel[c] = u;
                    state[c] = 1;
                }else {
                    state[c] = 0;
                }
            }else{
                int imp = (1 + outu[c]) - f[c];
                if(c == miChild[u] && imp > 0 && s != 1){
                    inSel[u] = c;outSel[c] = u;
                    state[c] = 2;
                }else{
                    state[c] = 0;
                }
            }
            q.push(c);
        }
    }

    vector<int> chainOf(n+1,0),chainStart,chainEnd;
    int cid = 0;
    for(int u = 1;u <= n;u++){
        if(inSel[u] != 0) continue;
        cid++;
        int v = u,last = u;
        while(v != 0){
            chainOf[v] = cid;
            last = v;
            v = outSel[v];
        }
        chainStart.push_back(u);
        chainEnd.push_back(last);
    }

    vector<int> indeg(cid + 1,0);
    vector<vector<int>> cg(cid + 1);
    for(auto& [u,v] : raw){
        if(outSel[u] == v && inSel[v] == u) continue;
        int cu = chainOf[u],cv = chainOf[v];
        if(cu != cv) {cg[cu].push_back(cv);++indeg[cv];}
    }
    queue<int> cq;
    for(int i = 1;i <= cid;i++)
        if(indeg[i] == 0) cq.push(i);
    vector<int> corder;
    while(!cq.empty()){
        int u = cq.front();cq.pop();
        corder.push_back(u);
        for(int v : cg[u])
            if(--indeg[v] == 0) cq.push(v);
    }

    cout << K << "\n";
    if(K > 0){
        for(int i = 0;i < K;i++){
            int c1 = corder[i],c2 = corder[i+1];
            cout << chainEnd[c1-1] << " " << chainStart[c2 - 1] << "\n";
        }
    }
}

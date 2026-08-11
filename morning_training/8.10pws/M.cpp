#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int N,M,K,Q;
    cin >> N >> M >> K >> Q;

    vector<set<pair<int,int>>> adj(N);
    for(int j = 0;j < M;j++){
        int U,V,W;
        cin >> U >> V >> W;
        U--,V--,W--;

        adj[U].insert({V,W});
    }

    vector<set<pair<int,int>>> companies(K);
    set<int> visited;

    auto addCity = [&](int u){
        if(visited.count(u)) return;
        visited.insert(u);

        for(auto [v,w] : adj[u]){
            companies[w].insert({u,v});
        }
    };

    addCity(0);

    while(Q--){
        int t;
        cin >> t;

        if(t == 1){
            int u,v,w;
            cin >> u >> v >> w;
            u--,v--,w--;

            adj[u].insert({v,w});
            if(visited.count(u)){
                companies[w].insert({u,v});
            }
        } else if(t == 2){
            int u,v,w;
            cin >> u >> v >> w;
            u--,v--,w--;

            adj[u].erase({v,w});
            if(visited.count(u)){
                companies[w].erase({u,v});
            }
        } else if(t == 3){
            int w;
            cin >> w;
            w--;

            set<pair<int,int>> edgeToTraverse;
            swap(edgeToTraverse, companies[w]);
            for(auto [u,v] : edgeToTraverse){
                addCity(v);
            }

            cout << visited.size() << "\n";
        }
    }
}

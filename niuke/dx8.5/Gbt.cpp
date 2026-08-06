#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--){
        int n,m,k;
        cin >> n >> m >> k;
        vector<vector<int>> adj(n);
        for(int i = 0;i < m;i++){
            int u,v;cin >> u >> v;u--;v--;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        vector<char> S(n,0);
        for(int i = 0;i < k;i++){
            int a;
            cin >> a;
            a--;
            S[a] = 1;
        }

        vector<int> cnt(n,0);
        for(int v = 0;v < n;v++){
            for(int w : adj[v]) if(S[w]) cnt[v]++;
        }

        vector<char> inR(n,0);
        queue<int> q;
        for(int v = 0;v < n;v++){
            if(!S[v] && cnt[v] >= 2){
                inR[v] = 1;
                q.push(v);
            }
        }
        while(!q.empty()){
            int v = q.front();q.pop();
            for(int w : adj[v]){
                if(S[w] || inR[w]) continue;
                cnt[w]++;
                if(cnt[w] >= 2){
                    inR[w] = 1;
                    q.push(w);
                }
            }
        }

        vector<int> win;
        for(int v = 0;v < n;v++){
            if(!S[v] && cnt[v] >= 1) win.push_back(v);
        }

        cout << win.size() << "\n";
        for(int i = 0;i < (int)win.size();i++){
            cout << (i ? " " : "") << (win[i] + 1);
        }
        cout << "\n";
    }
}

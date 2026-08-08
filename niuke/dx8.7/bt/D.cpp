#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,x;
    cin >> n >> x;
    vector<int> P(n+1),Q(n+1),posP(n+1),posQ(n+1);
    for(int i = 1;i <= n;i++) {cin >> P[i];posP[P[i]] = i;}
    for(int i = 1;i <= n;i++) {cin >> Q[i];posQ[Q[i]] = i;}
    int a = posP[x],b = posQ[x];

    for(int w = 1;w <= n;w++){
        if(posQ[w] > b && posP[w] > a){
            cout << "NO\n";
            return 0;
        }
    }

    int pmax = 0;
    for(int i = 1;i < n;i++){
        pmax = max(pmax,posQ[P[i]]);
        if(pmax == i){
            cout << "NO\n";
            return 0;
        }
    }

    vector<char> dead(n+1,0);
    vector<int> elim;
    int pi = 1,qi = 1;
    while(true){
        while(pi <= n && dead[P[pi]]) pi++;
        while(qi <= n && dead[Q[qi]]) qi++;
        int u = P[pi],v = Q[qi];
        if(u == v) break;
        int take;
        if(u == x) take = u;
        else if(v == x) take = v;
        else if(posQ[u] < posP[v]) take = u;
        else take = v;
        elim.push_back(take);
        dead[take] = 1;
    }

    cout << "YES\n";
    for(int i = 0;i < (int)elim.size();i++){
        if(i) cout << " ";
        cout << elim[i];
    }
    cout << "\n";
    return 0;
}

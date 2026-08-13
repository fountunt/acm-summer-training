#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Node{
    int val;
    vector<int> son;
    int fa;
    int sonSum;
};

bool isSquare[1000001];

void init(){
    for(ll i = 1;i * i <= 1000000;i++){
        isSquare[i*i] = true;
    }
}

void solve(){
    int n;
    cin >> n;
    vector<Node> nodes(n+1);

    for(int i = 1;i <= n;i++){
        cin >> nodes[i].val;
    }
    for(int i = 1;i < n;i++){
        int u,v;
        cin >> u >> v;
        nodes[u].son.push_back(v);
        nodes[v].son.push_back(u);
    }

    vector<int> order;
    order.reserve(n);
    order.push_back(1);
    for(int i = 0;i < (int)order.size();i++){
        int x = order[i];
        for(int y : nodes[x].son){
            if(y != nodes[x].fa){
                nodes[y].fa = x;
                order.push_back(y);
            }
        }
    }
    for(int i = (int)order.size()-1;i >= 1;i--){
        int x = order[i];
        nodes[nodes[x].fa].sonSum += nodes[x].sonSum + 1;
    }

    ll ans = 0;
    for(int x = 1;x <= n;x++){
        if(!isSquare[nodes[x].val]) continue;

        ll sum1 = n;
        ll sum2 = 1;
        ll sum3 = 1;
        for(int y : nodes[x].son){
            ll s;
            if(y == nodes[x].fa) s = n - nodes[x].sonSum - 1;
            else s = nodes[y].sonSum + 1;
            sum2 += s * s;
            sum3 += s * s * s;
        }
        ans += (sum1*sum1*sum1 - 3*sum1*sum2 + 2*sum3)/6;
    }
    cout << ans << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    init();
    int t;
    cin >> t;
    while(t--) solve();
}

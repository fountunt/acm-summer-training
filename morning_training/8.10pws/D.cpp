#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 3e5 + 5;

int bit[N + 5];
int last[N + 5];

void update(int u,int val){
    for(; u <= N;u += (u & -u)){
        bit[u] += val;
    }
}

int query(int u){
    int total = 0;
    for(;u;u -= (u & -u)){
        total += bit[u];
    }
    return total;
}

int query(int l,int r){
    if(l == 0) return query(r);
    return query(r) - query(l-1);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m;
    cin >> n >> m;

    memset(last,-1,sizeof last);

    ll total = 0;
    ll distinct = 0;
    for(int i = 1;i <= n;i++){
        int x;
        cin >> x;
        update(i,1);
        if(last[x] == -1){
            distinct++;
        } else {
            update(last[x],-1);
        }
        total += query(last[x] + 1,i - 1);
        last[x] = i;
    }

    total += (m - distinct) * distinct + distinct * (distinct - 1) / 2;
    cout << total << "\n";
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n,m;
    cin >> n >> m;
    vector<vector<ll>> g(n+1,vector<ll>(m+1));
    for(int i = 1;i <= n;i++)
        for(int j = 1;j <= m;j++)
            cin >> g[i][j];

    if(n == 1 || m == 1){
        bool same = true;
        for(int i = 1;i <= n;i++)
            for(int j = 1;j <= m;j++)
                if(g[i][j] != g[1][1]) same = false;
        cout << (same ? 0 : -1) << "\n";
        return;
    }

    ll k = g[1][2] + g[2][1] - 2LL * g[1][1];
    if(k < 0) {cout << -1 << "\n";return;}
    ll x = k + g[1][1];

    for(int i = 1;i <= n;i++)
        for(int j = 1;j <= m;j++)
            if(x - g[i][j] < 0){cout << -1 << "\n";return;}

    bool ok = true;
    vector<ll> cur(n+1),nxt(n+1);
    cur[1] = x - g[1][1];

    for(int s = 2;s < n+m;s++){
        for(int r = 1;r <= n;r++){
            int c = s + 1 - r;
            nxt[r] = (c >= 1 && c <= m) ? x - g[r][c] : 0;
        }

        ll carry = 0;
        for(int r = 1;r <= n;r++){
            ll need = nxt[r] - carry;
            if(need < 0 || need > cur[r]) ok = false;
            else carry = cur[r] - need;
        }
        if(carry != 0) ok = false;
        cur.swap(nxt);
    }
    cout << (ok ? k : -1) << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--) solve();
}

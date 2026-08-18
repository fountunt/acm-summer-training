#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n;
    cin >> n;
    ll ans = 0;
    for(int i = 1;i <= n;i++){
        ans += 1LL * (n/i) * (n/i);
    }
    cout << ans << "\n";
}

int main(){
    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;

void solve(){
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 1;i <= n;i++) cin >> a[i];

    for(int i = 1;i <= n;i++){
        if(a[i] == n){cout << 0 << "\n";return;}
    }
    int p = 1;
    while(p + 1 <= n && a[p] <= a[p+1]) p++;
    while(p + 1 <= n && a[p] >= a[p+1]) p++;
    if(p < n){cout << 0 << "\n";return;}


}

int main(){
    int T;
    cin >> T;
    while(T--) solve();
}

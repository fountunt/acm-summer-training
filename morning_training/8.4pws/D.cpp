#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;cin >> n;
    vector<ll> a(n+1,0);
    for(int i = 1;i <= n;i++) cin >> a[i];

    vector<ll> dp(n+1);
    dp[1] = (a[1] == 0 ? 0 : 1);
    dp[n] = (a[n] == 0 ? 0 : 1);
    ll maxt = -1;
    for(int i = 2;i < n;i++){
        dp[i] = min(dp[i-1]+1,a[i]);
    }
    for(int i = n-1;i > 1;i--){
        dp[i] = min(dp[i+1]+1,dp[i]);
    }
    for(int i = 1;i <= n;i++) maxt = max(maxt,dp[i]);

    cout << maxt << "\n";
}

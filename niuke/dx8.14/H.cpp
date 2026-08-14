#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int n;
    cin >> n;
    vector<ll> a(n);
    for(int i = 0;i < n;i++) cin >> a[i];
    ll sum = 0;
    if(n == 1){
        cout << 1 << "\n";
        return 0;
    }

    sort(a.begin(),a.end());

    for(int i = 0;i < n;i++){
        sum += a[i];
    }

    ll ans = 0;
    if(sum & 1){
        ans = (a[n-1] + 1) / 2;
    } else {
        ans = a[n-1] / 2;
    }

    cout << ans << "\n";
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0;i < n;i++){
        cin >> a[i];
    }

    if(n & 1) {
        cout << "NO" << "\n";
        return;
    }

    int l = 0,r = 1e9 + 1;
    for(int i = 0;i < n;i+=2){
        if(a[i] <= a[i+1]){
            cout << "NO" << "\n";
            return;
        } else {
            r = min(r,a[i]);
            l = max(l,a[i+1]);
        }
    }

    if(r - l > 1){
        cout << "YES" << "\n";
    } else {
        cout << "NO" << "\n";
    }
}

int main(){
    int t;
    cin >> t;
    while(t--) solve();
}

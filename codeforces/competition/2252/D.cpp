#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n;
    cin >> n;
    vector<ll> a(n);
    vector<ll> diff(n+1,0);
    for(int i = 0;i < n;i++){
        cin >> a[i];
        if(i > 0) diff[i] = a[i] - a[i-1];
    }
    diff[0] = a[0];

    int l = 1,r = 2;
    while(r < n){
        if((diff[r] & 1) == (diff[r-1] & 1)){
            r++;
        } else {
            sort(diff.begin()+l,diff.begin()+r);
            l = r;
            r++;
        }
    }
    sort(diff.begin()+l,diff.begin()+r);

    vector<ll> ans(n);
    ans[0] = diff[0];
    for(int i = 1;i < n;i++){
        ans[i] = diff[i] + ans[i-1];
    }

    for(int i = 0;i < n;i++){
        cout << ans[i] << " ";
    }
    cout << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) solve();
}

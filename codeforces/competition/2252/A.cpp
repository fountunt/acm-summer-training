#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        unordered_map<int,int> a;
        int maxn = -1;
        int maxidx = -1;
        for(int i = 0;i < n;i++){
            int x;
            cin >> x;
            a[x]++;
            if(a[x] > maxn){
                maxn = a[x];
                maxidx = x;
            }
        }

        int ans = 0;
        if(2*maxn - 2 <= n){
            for(auto [k,v] : a){
                ans += k*v;
            }
        } else {
            for(auto [k,v] : a){
                ans += k*v;
            }
            ans -= maxidx * (2 * maxn - n - 2);
        }

        cout << ans << "\n";
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;

        vector<ll> a(n),b(n);
        for(int i = 0;i < n;i++) cin >> a[i];
        for(int i = 0;i < n;i++) cin >> b[i];

        ll sum1 = 0,sum2 = 0;

        bool ok = true;
        for(int i = 0;i < n;i++){
            sum1 += a[i];
            sum2 += b[i];
            if(sum1 > sum2){
                ok = false;
                break;
            }
        }

        cout << (ok ? "YES" : "NO") << "\n";
    }
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int n;cin >> n;
    vector<ll> side(n+1);
    ll maxl = -1,sum = 0;
    for(int i = 1;i <= n;i++){
        cin >> side[i];
        maxl = max(maxl,side[i]);
        sum += side[i];
    }
    if(maxl >= (sum-maxl)*2){
        cout << sum - maxl << "\n";
    }else{
        cout << sum / 3 << "\n";
    }
}

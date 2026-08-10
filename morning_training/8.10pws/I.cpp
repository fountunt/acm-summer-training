#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    ll g = 0;
    for(int i = 0;i < n;i++){
        ll x;
        cin >> x;
        g = gcd(g,x);
    }

    if(g >= 2){
        cout << 1 << "\n" << g << " " << 1 << "\n";
    } else {
        cout << 2 << "\n" << "4 2" << "\n" << "2 1" << "\n";
    }

    return 0;
}

#include<bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int t;
    cin >> t;
    while(t--){
        int x,y;
        cin >> x >> y;
        cout << (x % y == 0 ? "YES" : "NO") << "\n";
    }
}

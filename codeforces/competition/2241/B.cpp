#include <bits/stdc++.h>
using namespace std;

int main(){
    int t;
    cin >> t;
    while(t--){
        long long x;
        cin >> x;
        long long y = 1;

        while(x > 0){
            x /= 10;
            y *= 10;
        }

        cout << y + 1 << "\n";
    }
}

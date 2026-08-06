#include <bits/stdc++.h>
using namespace std;
using ll = long long;

bool pri(int n){
    if(n == 1) return false;
    if(n == 2) return true;
    for(int i = 2;i * i <= n;i++){
        if(n % i == 0) return false;
    }
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while(t--){
        int n;cin >> n;
        if(pri(n-1)){
            if(n > 6){
                for(int i = 1;i <= n-4;i++){
                    cout << i << " ";
                }
                for(int i = n;i > n-4;i--) cout << i << " \n"[i == n-3];
            }else{
                cout << -1 << "\n";
            }
        }else{
            for(int i = 1;i <= n;i++){
                cout << i << " \n"[i == n];
            }
        }
    }
}
//1 2 3 4 5 6 7 8 14 10 12 13 9
//1 2 3 4 5 6 12 8 9 10 11 7
//1 2 3 4 5 6 7 8 9 10
//1 2 3 4 8 7 6 5

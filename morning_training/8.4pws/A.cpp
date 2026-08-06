#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N,K;cin >> N >> K;
    if(K < N/2 || K == N){cout << -1 << "\n";return 0;}

    int m = N/2;
    int d = N - K;
    int r = m % d;
    int k = m / d;

    for(int i = 0;i < r;i++) cout << "(";
    for(int i = 0;i < r;i++) cout << ")";
    for(int i = 0;i < k;i++){
        for(int j = 0;j < d;j++){
            cout << "(";
        }
        for(int j = 0;j < d;j++){
            cout << ")";
        }
    }
    cout << "\n";
}

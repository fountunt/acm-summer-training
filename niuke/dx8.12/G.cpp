#include <bits/stdc++.h>
using namespace std;
const int  N = 1e5 + 5;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a,b,c;
    cin >> a >> b >> c;

    cout << "1";
    for(int i = 0;i < N;i++){
        cout << "0";
    }
    cout << "1 ";

    for(int i = 0;i < N+1;i++){
        cout << "9";
    }
    cout << " ";

    cout << "1";
    for(int i = 0;i < N;i++){
        cout << "0";
    }
    cout << "2 ";

    for(int i = 0;i < N+1;i++){
        cout << "9";
    }
    cout << "\n";
}

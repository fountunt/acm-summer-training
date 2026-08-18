#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n;
    cin >> n;
    string s1,s2;
    cin >> s1 >> s2;
    int ns10_1 = 0,ns10_2 = 0;
    int ns20_1 = 0,ns20_2 = 0;
    for(int i = 0;i < n;i++){
        if(s1[i] == '0'){
            if(i & 1){
                ns10_2++;
            } else {
                ns10_1++;
            }
        }

        if(s2[i] == '0'){
            if(i & 1){
                ns20_2++;
            } else {
                ns20_1++;
            }
        }
    }

    if(ns10_2 == ns20_2 && ns10_1 == ns20_1){
        cout << "YES" << "\n";
    } else {
        cout << "NO" << "\n";
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) solve();
}

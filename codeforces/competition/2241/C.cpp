#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        string s;
        cin >> s;

        int count = 1;
        for(int i = 1;i < n;i++){
            if(s[i] != s[i-1]){
                count++;
            }
        }

        if(count == 2) cout << 2 << "\n";
        else cout << 1 << "\n";
    }
}

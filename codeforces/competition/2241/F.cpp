#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n;
    cin >> n;
    string s;
    cin >> s;

    int total0 = 0;
    for(char c : s) if(c == '0') total0++;

    bool bob = true;
    int ones = 0,zeros = 0;
    for(int i = 0;i < n;i++){
        if(s[i] == '0'){
            if(ones & 1){bob = false;break;}
            zeros++;
        } else {
            if((total0 - zeros) & 1){ bob = false;break;}
            ones++;
        }
    }
    cout << (bob ? "Bob" : "Alice") << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--) solve();
}

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

        int n0 = 0,n1 = 0;
        for(int i = 0;i < n;i++){
            if(s[i] == '0') n0++;
            else n1++;
        }

        int diff = n0 - n1;
        if(abs(diff) > 2){
            cout << -1 << "\n";
            continue;
        }

        int l = 1;
        int l0 = (s[0] == '0' ? 1 : 0);
        int l1 = (s[0] == '1' ? 1 : 0);


        for(int i = 1;i < n;i++){
            if(s[i] != s[i-1]){
                l++;
                if(s[i] == '0') l0++;
                else l1++;
            }
        }

        int diff1 = l0 - l1;

        int ans = n - l + max(0, abs(diff - diff1) - 1);

        cout << ans << "\n";
    }
}

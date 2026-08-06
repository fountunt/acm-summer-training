#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int t;
    cin >> t;
    while(t--){
        vector<int> a(3);
        for(int i = 0;i < 3;i++){
            cin >> a[i];
        }
        sort(a.begin(),a.end());
        cout << min(a[2] - a[1],a[1] - a[0]) << "\n";
    }
}

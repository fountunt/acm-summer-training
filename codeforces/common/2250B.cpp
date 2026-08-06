//令1的数量等于0的数量或加1
//连续相邻x个提供x-1和pair
//不连续提供1个
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int t;cin >> t;
    while(t--){
        int n,k;
        cin >> n >> k;
        if(k == n-1){
            cout << -1 << "\n";
            continue;
        }else{
            int one = k / 2;
            int zero = k - one;
            for(int i = 0;i < one + 1;i++) cout << "1";
            for(int i = 0;i < zero + 1;i++) cout << "0";
            for(int i = 0;i + one + zero + 2 < n;i++){
                cout << (i & 1 ? "0" : "1");
            }
            cout << "\n";
        }
    }
}

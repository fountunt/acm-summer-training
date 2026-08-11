#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int n,m;
    cin >> n >> m;
    vector<int> a(n);
    for(int i = 0;i < n;i++){
        cin >> a[i];
    }

    vector<vector<int>> g(n,vector<int>(m,0));

    if(m % 2 == 0){
        for(int i = 0;i < n;i++){
            if(a[i] > m/2){
                cout << -1 << "\n";
                return 0;
            }
        }

        for(int i = 0;i < n;i++){
            if(i == 0){
                for(int j = 0;a[i] > 0;j+=2){
                    g[i][j] = 1;
                    a[i]--;
                }
            } else {
                int j;
                if(g[i-1][0] == 1) j = 1;
                else j = 0;
                for(;a[i] > 0;j += 2){
                    g[i][j] = 1;
                    a[i]--;
                }
            }
        }
    } else {
        for(int i = 0;i < n;i++){
            if(a[i] > (m+1)/2){
                cout << -1 << "\n";
                return 0;
            }
        }

        int lim = (m + 1)/2;
        vector<int> fullcol;
        for(int i = 0;i < n;i++){
            if(a[i] == (m+1)/2){
                for(int j = 0;j < m;j+=2) g[i][j] = 1;
                fullcol.push_back(i);
            }
        }
        //满行标记并赋值

        int size = fullcol.size();
        for(int i = 1;i < size;i++){
            int low = fullcol[i-1],up = fullcol[i];
            if(up - low == 1) continue;

            //中间夹层为偶数
            if((up - low - 1) % 2 == 0){
                if((up - low - 1) % (m + 1) != 0){
                    cout << -1 << "\n";
                    return 0;
                }
                int block = 0;
                for(int j = low + 1;j < up;j++){
                    if(block == 0){
                        for(int k = 1;a[j] > 0;k += 2){
                            g[j][k] = 1;
                            a[j]--;
                        }
                    } else {
                        for(int k = (block + 1) % m;a[j] > 0;k = (k + 2) % m){
                            g[j][k] = 1;
                            a[j]--;
                        }
                    }
                    block = (block + 1) % m;
                }
            } else { //中间夹层为奇数
                for(int j = low + 1;j < up;j++){
                    for(int k = 0;k < m;k++){
                        if(g[j-1][k] == 0 && a[j] > 0){
                            a[j]--;
                            g[j][k] = 1;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
            cout << g[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
    return 0;
}

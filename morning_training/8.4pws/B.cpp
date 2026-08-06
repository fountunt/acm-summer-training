#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int dirs[8][2] = {{0,1},{0,-1},{1,0},{-1,0},{-1,-1},{1,1},{1,-1},{-1,1}};
char a[3][3];

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for(int i = 0;i < 3;i++)
        for(int j = 0;j < 3;j++)
            cin >> a[i][j];

    string ans = "zzz";
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            for(auto [x1,y1] : dirs){
                int nx1 = i + x1;
                int ny1 = j + y1;
                if(nx1 < 0 || nx1 >= 3 || ny1 < 0 || ny1 >= 3) continue;
                for(auto [x2,y2] : dirs){
                    int nx2 = i + x2;
                    int ny2 = j + y2;
                    if(ny2 < 0 || ny2 >= 3 || nx2 < 0 || nx2 >= 3) continue;
                    if(nx2 == nx1 && ny2 == ny1) continue;
                    string ans1,ans2;
                    ans1 += a[nx1][ny1];ans1 += a[i][j];ans1 += a[nx2][ny2];
                    ans = min(ans1,ans);
                    ans1.reserve();
                    ans = min(ans,ans1);
                }
            }
        }
    }
    cout << ans << "\n";
}

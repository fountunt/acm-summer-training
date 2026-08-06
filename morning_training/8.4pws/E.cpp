//dp
//记录每一位加入x或者y
//当某一块能达到长度n即可
//回溯选取过程 在选取过程中记录下来

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    int N;cin >> N;
    int M = N*2;
    vector<int> C(M+1);
    for(int i = 1;i <= M;i++) cin >> C[i];

    vector<char> peak(M+1,0);
    int mx = 0;
    for(int i = 1;i <= M;i++){
        if(C[i] >= mx) {mx = C[i];peak[i] = 1;}
    }//找到比前面都高的点

    vector<char> dpA(N+1,0),dpB(N+1,0);
    vector<vector<array<int,2>>> prev(M+1,vector<array<int,2>>(N+1,array<int,2>({-1,-1})));
    //记录轨迹[位置][数量][块类型] = 上一步状态
    dpA[1] = 1;
    dpB[0] = 1;
    prev[1][1][0] = -2;
    prev[1][0][1] = -2;

    for(int t = 2;t <= M;t++){
        vector<char> nda(N+1,0),ndb(N+1,0);
        for(int a = 0;a <= N;a++){
            if(dpA[a]){
                if(a+1 <= N){
                    nda[a+1] = 1;
                    prev[t][a+1][0] = 2 * a + 0;
                }
                if(peak[t]){
                    ndb[a] = 1;
                    prev[t][a][1] = 2 * a;
                }
            }
            if(dpB[a]){
                ndb[a] = 1;
                prev[t][a][1] = 2 * a + 1;
                if(peak[t] && a + 1 <= N){
                    nda[a+1] = 1;
                    prev[t][a+1][0] = a * 2 + 1;
                }
            }
        }
        dpA = move(nda);
        dpB = move(ndb);
    }
    int fin_type = -1;
    if(dpA[N]) fin_type = 0;
    else if(dpB[N]) fin_type = 1;
    if(fin_type == -1){
        cout << -1 << "\n";
        return 0;
    }

    vector<int> assign(M+1,-1);
    int t = M,typ = fin_type,a = N;
    while(t >= 1){
        int packed = prev[t][a][typ];
        assign[t] = typ;
        if(packed == -2) break;
        a = packed / 2;
        typ = packed % 2;
        t--;
    }

    vector<int> A,B;
    for(int i = 1;i <= M;i++){
        if(assign[i] == 0) A.push_back(C[i]);
        else B.push_back(C[i]);
    }
    for(int i = 0;i < N;i++) cout << A[i] << " \n"[i+1 == N];
    for(int i = 0;i < N;i++) cout << B[i] << " \n"[i+1 == N];
}

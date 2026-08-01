#include <bits/stdc++.h>
using namespace std;
const int N = 3e5+5;
int n,m;
int fa[N],ans[N];

int find(int x){
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for(int i = 1;i <= n+1;i++) fa[i] = i;
    while(m--){
        int l,r,x;
        cin >> l >> r >> x;
        for(int cur = find(l);cur < x;cur = find(cur)){
            ans[cur] = x;
            fa[cur] = find(cur+1);
        }
        for(int cur = find(x+1);cur <= r;cur = find(cur)){
            ans[cur] = x;
            fa[cur] = find(cur+1);
        }
    }
    for(int i = 1;i <= n;i++) cout << ans[i] << " \n"[i == n];
}

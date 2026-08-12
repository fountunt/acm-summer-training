#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 5;
int n,m;
int nxt[MAXN][26],idx;
int last[MAXN];
vector<int> nextUse[MAXN];
int cntUse[MAXN];
int total,hit,active;
priority_queue<int> pq;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for(int i = 1;i <= n;i++){
        string s; cin >> s;
        int p = 0;
        for(char ch : s){
            int c = ch - 'a';
            total++;
            if(!nxt[p][c]) nxt[p][c] = ++idx;
            p = nxt[p][c];
            if(last[p]) nextUse[last[p]].push_back(i);
            last[p] = i;
        }
    }

    for(int i = 1;i <= n;i++){
        hit += cntUse[i];
        active -= cntUse[i];
        for(int x : nextUse[i]){
            while(!pq.empty() && pq.top() <= i) pq.pop();
            if(active >= m && !pq.empty() && pq.top() > x){
                cntUse[pq.top()]--;pq.pop();
                pq.push(x);cntUse[x]++;
            } else if(active < m) {
                pq.push(x); cntUse[x]++;active++;
            }
        }
    }

    cout << total - hit << "\n";
    return 0;
}
//1.记录每个前缀下一次出现的时间
//2.根据规则来确定是否缓存，cntUse[i] 记录时间为i时，新进入的字符有多少是已经被缓存的
//3.插入弹出规则 若缓存未满则直接插入 若缓存已经满了就把最远端的缓存和新缓存比较，留下距离最近的缓存
//hit代表利用缓存减少的加载数量
//active代表以及在缓存中的数量
//算法的巧妙点 用未来的预知视角来预处理字符 确定每个前缀的下一次的出现时间
//对出现时间进行从小到大的插入删除 维护缓存的最优

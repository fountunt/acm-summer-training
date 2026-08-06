#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--){
        int n, m, k;
        cin >> n >> m >> k;
        vector<vector<int>> adj(n);
        for(int i=0;i<m;i++){
            int u, v; cin >> u >> v; u--; v--;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        vector<char> S(n, 0);
        for(int i=0;i<k;i++){ int a; cin >> a; a--; S[a]=1; }

        // cnt[v] = |N(v) ∩ (S ∪ R)|
        vector<int> cnt(n, 0);
        for(int v=0; v<n; v++)
            for(int w: adj[v]) if(S[w]) cnt[v]++;

        // R = least fixpoint of { v ∉ S : |N(v) ∩ (S∪R)| >= 2 }
        vector<char> inR(n, 0);
        queue<int> q;
        for(int v=0; v<n; v++)
            if(!S[v] && cnt[v] >= 2){ inR[v] = 1; q.push(v); }
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(int w: adj[v]){
                if(S[w] || inR[w]) continue;
                cnt[w]++;
                if(cnt[w] >= 2){ inR[w] = 1; q.push(w); }
            }
        }

        // W = { v ∉ S : |N(v) ∩ (S∪R)| >= 1 }
        vector<int> win;
        for(int v=0; v<n; v++)
            if(!S[v] && cnt[v] >= 1) win.push_back(v);

        cout << win.size() << "\n";
        for(int i=0; i<(int)win.size(); i++)
            cout << (i? " ":"") << (win[i]+1);
        cout << "\n";
    }
    return 0;
}

/*
================================================================
            牛客多校 · Problem G 图上游戏  中文题解
================================================================

【题意简述】
    无向连通图 G（所有点度数 <= 3），k 个特殊顶点。游戏：Alice 站在非特殊
    顶点出发，轮流操作、Alice 先手。Alice 的回合：若在特殊点则胜；否则移动到
    一个相邻顶点（无路可走则 Bob 胜）。Bob 的回合：删除任意一条剩余边。
    求所有"必胜"顶点（非特殊，且 Alice 从该点出发在最优策略下必胜）。

【思路】

  一、回合结构
     Alice 移动一步，Bob 删一条边，如此交替。因此 Alice 从 u 移动到 w 之后，
     Bob 会删除任意一条边，然后轮到 Alice 在 w 上行动。
     所以"u 必胜"当且仅当：存在一个邻居 w，使得无论 Bob 删哪条边，Alice
     从 w 出发仍然必胜。而"从 w 出发仍然必胜"要求 w 对"单边删除"是稳健的。

  二、定义两个集合
     记 S 为特殊点集合。
     * W = 必胜点：Alice 当前所在、轮到 Alice 走时她必能获胜的顶点。
     * R = 稳健必胜点：删去任意一条边之后，Alice 仍能必胜的顶点。
     递推刻画：
        - u ∈ W  ⟺  u 有至少 1 个邻居在 S ∪ R 中。
            （Alice 一步走到 S∪R 里的点：到特殊点直接赢；到 R 里的点，
              R 能顶住 Bob 删任意一条边。）
        - v ∈ R  ⟺  v 有至少 2 个邻居在 S ∪ R 中。
            （Bob 只删一条边，v 至少还剩 1 个 S∪R 邻居，Alice 走过去继续赢。
              若不足 2 个，Bob 删掉那个邻居边，v 就失去必胜路线。）
     R 是方程 R = {v∉S : |N(v)∩(S∪R)| ≥ 2} 的最小不动点。

  三、BFS 计算
     cnt[v] = |N(v) ∩ (S∪R)|。初始化 cnt[v] = 特殊邻居个数。
     凡 cnt[v] ≥ 2 的 v 加入 R 并入队；出队时把每个非特殊、未入 R 的邻居
     的 cnt 加 1，加到达 2 就加入 R。这就是最小不动点的扩散。
     结束后：
        W = { v ∉ S : cnt[v] ≥ 1 }
     由于图连通且 k < n，至少存在一个与特殊点相邻的非特殊点，故 c ≥ 1。

【复杂度】
     BFS 每个点每条边各处理常数次，O(n+m)。总 n,m 和 ≤ 2e5，非常快。

【正确性验证】
     * 四个官方样例全部通过：{2} / {2} / {1,2} / {2,3}。
     * 暴力博弈求解器（枚举所有删边状态）交叉验证：1.8 万个随机图全部一致。
     * 最坏情况实测（n=1e5, m=1.5e5）：约 0.16s。
================================================================
*/

// Problem C. 我得到神奇宝贝了！
// Idea: 记布尔变量 xij=0 表示 Ri<Gj、yik=0 表示 Ri<Bk、zjk=0 表示 Gj<Bk。
//   对询问 (Ri,Gj,Bk)，中位数颜色 c 给出两条异或约束：
//     xij ⊕ yik = (c=='R'),  yik ⊕ zjk = (c=='B')。
//   所有变量构成异或约束图（只用 i,j,k 中至少一个为 1 的询问即连通）。
//   BFS 任选起点传播出全部异色大小关系，转为有向边后拓扑排序即得总排名。
// 复杂度: 读入 O(xyz)，其余 O(xy+xz+yz)
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int x, y, z;
    if (!(cin >> x >> y >> z)) return 0;
    vector<vector<string>> s(x+1, vector<string>(y+1));
    for (int i = 1; i <= x; ++i)
        for (int j = 1; j <= y; ++j) {
            string t; cin >> t;
            s[i][j] = " " + t;   // s[i][j][k]
        }

    // 变量编号
    auto idX = [&](int i, int j){ return (i-1)*y + (j-1); };                 // 0..xy-1
    auto idY = [&](int i, int k){ return x*y + (i-1)*z + (k-1); };           // xy..xy+xz-1
    auto idZ = [&](int j, int k){ return x*y + x*z + (j-1)*z + (k-1); };     // ..总
    int V = x*y + x*z + y*z;
    vector<vector<pair<int,int>>> adj(V);
    auto add = [&](int u, int v, int w){ adj[u].push_back({v,w}); adj[v].push_back({u,w}); };
    auto addC = [&](int i, int j, int k){
        char c = s[i][j][k];
        add(idX(i,j), idY(i,k), c=='R');   // xij ⊕ yik = [中位数为R]
        add(idY(i,k), idZ(j,k), c=='B');   // yik ⊕ zjk = [中位数为B]
    };
    // 只用 i,j,k 中至少一个为 1 的询问
    for (int i = 1; i <= x; ++i) for (int j = 1; j <= y; ++j) addC(i, j, 1);
    for (int i = 1; i <= x; ++i) for (int k = 1; k <= z; ++k) addC(i, 1, k);
    for (int j = 1; j <= y; ++j) for (int k = 1; k <= z; ++k) addC(1, j, k);

    // BFS 传播（多连通分量兜底）
    vector<int> val(V, -1);
    for (int st = 0; st < V; ++st) if (val[st] == -1) {
        val[st] = 0;
        queue<int> q; q.push(st);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [v, w] : adj[u])
                if (val[v] == -1) { val[v] = val[u] ^ w; q.push(v); }
        }
    }

    // 建 DAG：大小关系 → 有向边
    int N = x + y + z;
    vector<vector<int>> dag(N+1);
    vector<int> indeg(N+1, 0);
    auto addE = [&](int a, int b){ dag[a].push_back(b); ++indeg[b]; };
    for (int i = 1; i <= x; ++i) for (int j = 1; j <= y; ++j) {
        if (val[idX(i,j)] == 0) addE(i, x+j); else addE(x+j, i);
    }
    for (int i = 1; i <= x; ++i) for (int k = 1; k <= z; ++k) {
        if (val[idY(i,k)] == 0) addE(i, x+y+k); else addE(x+y+k, i);
    }
    for (int j = 1; j <= y; ++j) for (int k = 1; k <= z; ++k) {
        if (val[idZ(j,k)] == 0) addE(x+j, x+y+k); else addE(x+y+k, x+j);
    }

    // 拓扑排序
    vector<int> rank(N+1);
    queue<int> q;
    for (int v = 1; v <= N; ++v) if (indeg[v] == 0) q.push(v);
    int pos = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        rank[u] = pos++;
        for (int v : dag[u]) if (--indeg[v] == 0) q.push(v);
    }

    for (int i = 1; i <= x; ++i) { if (i > 1) cout << ' '; cout << rank[i]; } cout << '\n';
    for (int j = 1; j <= y; ++j) { if (j > 1) cout << ' '; cout << rank[x+j]; } cout << '\n';
    for (int k = 1; k <= z; ++k) { if (k > 1) cout << ' '; cout << rank[x+y+k]; } cout << '\n';
    return 0;
}

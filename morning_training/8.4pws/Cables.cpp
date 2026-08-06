#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<pair<int, int>>> adj(n + 1); // (neighbor, dir): +1 表示 u->v, -1 表示 v->u
    vector<pair<int, int>> raw;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v; // 应急模式: 仅允许 u -> v
        raw.push_back({u, v});
        adj[u].push_back({v, +1});
        adj[v].push_back({u, -1});
    }

    // ---- 建父子关系 (迭代 DFS) ----
    vector<int> parent(n + 1, 0), parDir(n + 1, 0), order;
    vector<int> st = {1};
    parent[1] = -1;
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        order.push_back(u);
        for (auto& [v, dr] : adj[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            parDir[v] = dr;  // 子视角: 边 parent->v 则 +1, v->parent 则 -1
            st.push_back(v);
        }
    }
    vector<vector<int>> children(n + 1);
    for (int v = 1; v <= n; ++v)
        if (parent[v] > 0) children[parent[v]].push_back(v);

    // ---- DP: 最大匹配 (可保留在哈密顿链上的原边数) ----
    // f[u]: 子树最优(父边不选)  inu[u]: u 的入槽已被占  outu[u]: u 的出槽已被占
    vector<int> f(n + 1, 0), inu(n + 1, 0), outu(n + 1, 0);
    vector<int> moChild(n + 1, 0), miChild(n + 1, 0);
    for (int it = (int)order.size() - 1; it >= 0; --it) {
        int u = order[it];
        int base = 0, mo = 0, mi = 0;
        for (int c : children[u]) {
            base += f[c];
            if (parDir[c] == +1) { // u->c 出边
                int imp = (1 + inu[c]) - f[c];
                if (imp > mo) mo = imp, moChild[u] = c;
            } else {              // c->u 入边
                int imp = (1 + outu[c]) - f[c];
                if (imp > mi) mi = imp, miChild[u] = c;
            }
        }
        f[u] = base + mo + mi;
        inu[u] = base + mo;
        outu[u] = base + mi;
    }
    int matching = f[1];
    int chains = n - matching;
    int K = chains - 1;

    // ---- 重建选择的边 ----
    vector<int> outSel(n + 1, 0), inSel(n + 1, 0); // 选中的出/入边
    vector<int> state(n + 1, 0); // 0 自由, 1 入槽已占, 2 出槽已占
    state[1] = 0;
    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int s = state[u]; // 0 自由, 1 入槽被父边占用, 2 出槽被父边占用
        for (int c : children[u]) {
            if (parDir[c] == +1) { // u->c 出边，只能选最多一条(用 u 出槽/c 入槽)
                int imp = (1 + inu[c]) - f[c];
                if (c == moChild[u] && imp > 0 && s != 2) {
                    outSel[u] = c; inSel[c] = u;
                    state[c] = 1;
                } else {
                    state[c] = 0;
                }
            } else {               // c->u 入边，只能选最多一条(用 u 入槽/c 出槽)
                int imp = (1 + outu[c]) - f[c];
                if (c == miChild[u] && imp > 0 && s != 1) {
                    inSel[u] = c; outSel[c] = u;
                    state[c] = 2;
                } else {
                    state[c] = 0;
                }
            }
            q.push(c);
        }
    }

    // ---- 由选边构造链 ----
    vector<int> chainOf(n + 1, 0), chainStart, chainEnd;
    int cid = 0;
    for (int u = 1; u <= n; ++u) {
        if (inSel[u] != 0) continue; // 有入边，不是链头
        ++cid;
        int v = u, last = u;
        while (v != 0) {
            chainOf[v] = cid;
            last = v;
            v = outSel[v];
        }
        chainStart.push_back(u);
        chainEnd.push_back(last);
    }

    // ---- 链之间拓扑排序 ----
    vector<int> indeg(cid + 1, 0);
    vector<vector<int>> cg(cid + 1);
    for (auto& [u, v] : raw) {
        if (outSel[u] == v && inSel[v] == u) continue; // 已选边在链内
        int cu = chainOf[u], cv = chainOf[v];
        if (cu != cv) { cg[cu].push_back(cv); ++indeg[cv]; }
    }
    queue<int> cq;
    for (int i = 1; i <= cid; ++i)
        if (indeg[i] == 0) cq.push(i);
    vector<int> corder;
    while (!cq.empty()) {
        int u = cq.front(); cq.pop();
        corder.push_back(u);
        for (int v : cg[u])
            if (--indeg[v] == 0) cq.push(v);
    }

    cout << K << '\n';
    if (K > 0) {
        for (int i = 0; i < K; ++i) {
            int c1 = corder[i], c2 = corder[i + 1];
            cout << chainEnd[c1 - 1] << ' ' << chainStart[c2 - 1] << '\n';
        }
    }
    return 0;
}

/* ============================================================
 * 题解
 * ============================================================
 * 【题意】
 *   给定一棵定向树（N-1 条有向边，构成 DAG）。可任意添加有向边，
 *   使最终图恰好存在一个满足所有边方向的排列（即唯一拓扑序）。
 *   求最少加边数并输出一组方案。
 *
 * 【关键结论】
 *   DAG 恰有一个拓扑序  <=>  图中存在哈密顿路径（经过所有顶点的有向路径）。
 *   - 若存在哈密顿路径，所有点必须沿该路径排列，拓扑序唯一；
 *   - 若拓扑序唯一，任意相邻两元素之间必有直接有向边（否则交换仍合法），
 *     相邻边连成一条哈密顿路径。
 *
 *   于是问题变为：在树上选尽量多的原边构成若干条顶点不相交的有向链，
 *   再用新边把链首尾相接。设分成 c 条链，需要 c-1 条新边，答案 K = c-1。
 *
 * 【最少链数 = n - 最大匹配】
 *   每个点有两个槽位：入槽（被一条进来的边占用）、出槽（被一条出去的边占用）。
 *   选边 u->v 占用 u 的出槽与 v 的入槽；每个槽至多用一次，即二分图最大匹配
 *   （左部=出槽，右部=入槽）。原图是树，槽图为森林，可用树形 DP 求最大匹配：
 *
 *   以 1 为根。定义：
 *     f[u]   : 子树内最大选边数（父边不选）
 *     inu[u] : u 的入槽被父边占用时子树最优
 *     outu[u]: u 的出槽被父边占用时子树最优
 *   对每个孩子 c：
 *     - 边 u->c：不选贡献 f[c]，选则贡献 1 + inu[c]（用 u 出槽、c 入槽）
 *     - 边 c->u：不选贡献 f[c]，选则贡献 1 + outu[c]（用 u 入槽、c 出槽）
 *   u 至多选一条出边、一条入边，各取最大改进：
 *     f[u]   = Σ f[c] + mo + mi
 *     inu[u] = Σ f[c] + mo     （入槽已占用，仍可再选一条出边）
 *     outu[u]= Σ f[c] + mi     （出槽已占用，仍可再选一条入边）
 *
 *   最大匹配 = f[1]，K = (n-1) - f[1] = 链数 - 1。
 *
 * 【重建方案】
 *   1. 按 DP 的 argmax 回溯标记被选中的边，得到若干条链；
 *   2. 未被选中的原边连接两条不同链，构成"链间 DAG"（无环），对其拓扑排序；
 *   3. 依次连接 上一条链的链尾 -> 下一条链的链头，共 K 条新边。
 *
 *   正确性：
 *   - 链内按有向边排列、链间按 DAG 拓扑序排列，拼接序列是原图的合法拓扑序；
 *   - 新边把各链首尾连成哈密顿路径，故最终图拓扑序唯一；
 *   - 每条新边至多并两条链，链覆盖最少需链数-1 条边，K 最小。
 *
 * 【复杂度】
 *   时间 O(n)，空间 O(n)。n <= 1e5。
 * ============================================================ */

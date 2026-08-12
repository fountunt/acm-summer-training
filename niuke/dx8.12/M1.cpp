#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1000005;
int n, m;
int nxt[MAXN][26], idx;       // Trie 子边
int last[MAXN];               // 节点上一次被访问的请求编号
vector<int> nextUse[MAXN];    // nextUse[t]: 在请求 t 被用到、下次在 x 被用到的所有 x
int cntUse[MAXN];             // 缓存中 next-use == x 的节点数
int total, hit, active;       // 总长度、总命中数、缓存占用
priority_queue<int> pq;       // 缓存节点的 next-use（大根堆）

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++){
        string s; cin >> s;
        int p = 0;
        for (char ch : s){
            int c = ch - 'a';
            total++;
            if (!nxt[p][c]) nxt[p][c] = ++idx;
            p = nxt[p][c];
            if (last[p]) nextUse[last[p]].push_back(i);  // 记录相邻两次访问
            last[p] = i;
        }
    }

    for (int i = 1; i <= n; i++){
        hit += cntUse[i];          // next-use == i 的节点此刻被命中
        active -= cntUse[i];
        for (int x : nextUse[i]){  // 刚在 i 被用到，下一次在 x 用到
            while (!pq.empty() && pq.top() <= i) pq.pop();   // 懒删除过期项
            if (active >= m && !pq.empty() && pq.top() > x){
                // 缓存已满，且新节点比最远的更"近"：驱逐最远的
                cntUse[pq.top()]--; pq.pop();
                pq.push(x); cntUse[x]++;
            } else if (active < m){
                pq.push(x); cntUse[x]++; active++;
            }
            // 已满且新节点不比最远者更近：不放入，下次必 miss
        }
    }
    cout << total - hit << "\n";
    return 0;
}

/* ============================================================
 * 全流程讲解
 * ============================================================
 *
 * 【问题模型】
 *   缓存是一棵容量为 m 的 Trie。处理请求 s：
 *     代价 = 沿路径新加的边数 = |s| − 与当前缓存的最长公共前缀长度。
 *   因此  总代价 = 总字符数 total − 总命中数 hit。
 *   我们只需求最大总命中数(免费复用的边数)。
 *
 * 【核心思想：Bélády OPT】
 *   把每条 Trie 边当作一个独立的缓存项,容量 m 的最优替换策略是：
 *     满时驱逐「下次被使用时间最晚」的项,即始终保留 next-use 最小的 m 条边。
 *   关键性质：前缀封闭约束自动满足——
 *     对祖先 u 与子孙 v,任何请求经过 v 必经过 u,所以 next_use(u) ≤ next_use(v),
 *     深节点 v 永远比祖先 u 更早被驱逐。留下的集合天然是一棵前缀封闭的 Trie,
 *     无需显式维护"只能删叶节点"。
 *
 * 【阶段一：建 Trie + 记录相邻两次访问】(第一个 for 循环)
 *   遍历所有请求,把每个请求的路径插入 Trie(节点编号 nxt[p][c])。
 *   last[p] : 节点 p 上一次被访问的请求编号。
 *   当请求 i 再次经过节点 p 时(last[p] != 0)：
 *     nextUse[last[p]].push_back(i)
 *     含义 = "有一个节点在请求 last[p] 被用到,下一次在请求 i 被用到"。
 *   于是 nextUse[t] 收集了所有"在请求 t 被用到"的节点的 next-use 时间集合。
 *   total 累加所有字符串长度。
 *
 * 【阶段二：按时间顺序做 Bélády 缓存模拟】(第二个 for 循环)
 *   维护：
 *     pq      : 最大堆,存"缓存中节点的 next-use"。越大 = 越晚用到 = 越该先驱逐。
 *     cntUse[x] : 缓存中 next-use 恰为 x 的节点个数。
 *     active  : 当前缓存占用(所有 cntUse[x] 之和,恒 ≤ m)。
 *     hit     : 累计命中数(节省的代价)。
 *
 *   对每个请求 i：
 *     (1) hit += cntUse[i], active -= cntUse[i];
 *         next-use == i 的节点此刻正被请求 i 复用 → 命中,计入节省;
 *         它们从"待用"计数中移除(下一段会以新的 next-use 重新加入)。
 *     (2) for (x : nextUse[i]):
 *         刚刚在 i 被用到的每个节点,其下一次使用时间为 x,需要重新放入缓存。
 *         a. while (pq.top() <= i) pq.pop();
 *            懒删除：堆里 next-use 已过期的项(其占用已在 (1) 中扣除)清掉。
 *         b. 若缓存已满(active >= m)且该节点比堆顶(最远者)更近(pq.top() > x):
 *            驱逐最远的节点(cntUse[pq.top()]--, pq.pop()),换入当前节点
 *            (pq.push(x), cntUse[x]++)。 —— 这就是 Bélády 替换。
 *         c. 否则若缓存未满(active < m):直接放入,pq.push(x),cntUse[x]++,active++。
 *         d. 若已满且当前节点不比最远者更近(pq.top() <= x):
 *            不放入缓存——它下次再被用到时必然是一次 miss(计 1 代价)。
 *
 * 【答案】
 *   cout << total - hit;  总代价 = 总长度 − 总命中数。
 *
 * 【复杂度】
 *   时间 O(L log L),L = Σ|s_i| ≤ 1e6(每对相邻访问一次堆操作)。
 *   空间 O(26L)(Trie)+ O(L)(nextUse 列表总长 = L − 节点数,堆)。
 *
 * 【为什么不用处理 Trie 结构?】
 *   见"核心思想"：next_use 沿祖先-子孙方向单调不减,所以 Bélády 保留下来的
 *   m 条边自动前缀封闭,等价于题目要求的"只删叶节点"。这是本题最关键的性质。
 * ============================================================ */

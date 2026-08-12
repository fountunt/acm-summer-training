#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 998244353;

// 后缀数组：倍增 + 计数排序（串含唯一哨兵）
vector<int> build_sa(const string& s){
    int n = s.size(); const int A = 256;
    vector<int> p(n), c(n), cnt(max(A, n), 0);
    for(int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    for(int i = 1; i < A; i++) cnt[i] += cnt[i-1];
    for(int i = 0; i < n; i++) p[--cnt[(unsigned char)s[i]]] = i;
    c[p[0]] = 0; int cls = 1;
    for(int i = 1; i < n; i++){ if(s[p[i]] != s[p[i-1]]) cls++; c[p[i]] = cls-1; }
    vector<int> pn(n), cn(n);
    for(int h = 0; (1<<h) < n; ++h){
        for(int i = 0; i < n; i++){ pn[i] = p[i]-(1<<h); if(pn[i] < 0) pn[i] += n; }
        fill(cnt.begin(), cnt.begin()+cls, 0);
        for(int i = 0; i < n; i++) cnt[c[pn[i]]]++;
        for(int i = 1; i < cls; i++) cnt[i] += cnt[i-1];
        for(int i = n-1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0; cls = 1;
        for(int i = 1; i < n; i++){
            pair<int,int> a = {c[p[i]], c[(p[i]+(1<<h))%n]};
            pair<int,int> b = {c[p[i-1]], c[(p[i-1]+(1<<h))%n]};
            if(a != b) ++cls; cn[p[i]] = cls-1;
        }
        c.swap(cn);
    }
    return p;
}
// Kasai：lcp[i] = LCP(sa[i-1], sa[i])，lcp[0] = 0
vector<int> build_lcp(const string& s, const vector<int>& sa){
    int n = s.size();
    vector<int> rank(n), lcp(n, 0);
    for(int i = 0; i < n; i++) rank[sa[i]] = i;
    int k = 0;
    for(int i = 0; i < n; i++){
        if(rank[i] == 0){ k = 0; continue; }
        int j = sa[rank[i]-1];
        while(i+k < n && j+k < n && s[i+k] == s[j+k]) k++;
        lcp[rank[i]] = k;
        if(k) k--;
    }
    return lcp;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, p;
    cin >> s >> p;
    int n = s.size(), m = p.size();

    // ---- 由 SA+LCP 建压缩后缀树 ----
    string ss = s; ss.push_back(char(0));           // 加哨兵
    vector<int> sa = build_sa(ss), lcp = build_lcp(ss, sa);
    int N = ss.size();

    vector<int> parent{-1}, depth{0}, rep{0}, stk{0};
    for(int i = 0; i < N; i++){
        int cur = lcp[i], last = -1;
        while(depth[stk.back()] > cur){ last = stk.back(); stk.pop_back(); }
        if(depth[stk.back()] < cur){
            int v = parent.size();
            parent.push_back(stk.back()); depth.push_back(cur); rep.push_back(0);
            if(last != -1) parent[last] = v;
            stk.push_back(v);
        }
        int leaf = parent.size();
        parent.push_back(stk.back()); depth.push_back(N - sa[i]); rep.push_back(sa[i]);
        stk.push_back(leaf);
    }
    int V = parent.size();
    vector<vector<int>> children(V);
    for(int v = 1; v < V; v++) children[parent[v]].push_back(v);

    // dep[v]：真实标签长度（叶的标签以哨兵结尾，减 1）
    vector<int> dep(V);
    for(int v = 0; v < V; v++) dep[v] = children[v].empty() ? depth[v]-1 : depth[v];

    // rep[v]：子树内某个后缀起点，代表 v 的一个出现位置
    vector<int> order(V); iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a,int b){ return depth[a] > depth[b]; });
    for(int v : order) if(v != 0 && rep[v]) rep[parent[v]] = rep[v];

    // ---- lim[i] = s[i..] 不含 p 的最长前缀长度 ----
    vector<char> isEnd(n, 0);
    {
        vector<int> pi(m);                          // KMP 前缀函数，找 p 的结束位置
        for(int i = 1; i < m; i++){
            int j = pi[i-1];
            while(j > 0 && p[i] != p[j]) j = pi[j-1];
            if(p[i] == p[j]) j++;
            pi[i] = j;
        }
        int j = 0;
        for(int i = 0; i < n; i++){
            while(j > 0 && s[i] != p[j]) j = pi[j-1];
            if(s[i] == p[j]) j++;
            if(j == m){ isEnd[i] = 1; j = pi[j-1]; }
        }
    }
    const int INF = n + 5;
    vector<int> nxtEnd(n+1, INF); int best = INF;
    for(int i = n-1; i >= 0; i--){ if(isEnd[i]) best = i; nxtEnd[i] = best; }
    auto lim = [&](int i){                         // 下一个 p 结束位置 → 最长合法前缀
        int t = i + m - 1;
        if(t > n-1) return INF;
        int e = nxtEnd[t];
        return e >= INF ? INF : e - i;
    };

    // ---- 自底向上 DP：dp[v] = 1 + Π 子边贡献 ----
    // 压缩边 u→v（父 u，子 v，深度差 dep[v]-dep[u]）：
    //   L >= dep[v]（v 可选）: 贡献 = dp[v] + dep[v]-dep[u]-1
    //                           （取某个隐式点，或走子树 v 的方案）；
    //   L <  dep[v]（v 不可选）: 贡献 = max(L-dep[u],0)+1
    //                           （只取前 L-dep[u] 个隐式点，或不取）。
    vector<ll> dp(V, 0);
    for(int v : order){
        if(v == 0) continue;
        ll prod = 1;
        for(int u : children[v]){
            int L = lim(rep[u]);
            ll c;
            if(L >= dep[u]) c = (dp[u] + dep[u] - dep[v] - 1) % MOD;
            else            c = (max(L - dep[v], 0) + 1) % MOD;
            prod = prod * c % MOD;
        }
        dp[v] = (1 + prod) % MOD;
    }
    // 根代表空串，不是码字，故不加 +1；答案 = Π 根子边贡献 − 1（减空集）
    ll ans = 1;
    for(int u : children[0]){
        int L = lim(rep[u]);
        ll c;
        if(L >= dep[u]) c = (dp[u] + dep[u] - 1) % MOD;
        else            c = (max(L, 0) + 1) % MOD;
        ans = ans * c % MOD;
    }
    cout << (ans - 1 + MOD) % MOD << "\n";
    return 0;
}

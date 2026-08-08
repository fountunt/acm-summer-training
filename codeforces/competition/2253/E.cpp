#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353, G = 3;

long long modpow(long long b, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * b % MOD;
        b = b * b % MOD;
        e >>= 1;
    }
    return r;
}

void ntt(vector<int>& a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = modpow(G, (MOD - 1) / len);
        if (invert) wlen = modpow(wlen, MOD - 2);
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            for (int j = 0; j < len / 2; j++) {
                int u = a[i + j];
                int v = (int)(a[i + j + len / 2] * w % MOD);
                a[i + j] = (u + v) % MOD;
                a[i + j + len / 2] = (u - v + MOD) % MOD;
                w = w * wlen % MOD;
            }
        }
    }
    if (invert) {
        long long inv = modpow(n, MOD - 2);
        for (int& x : a) x = (int)(x * inv % MOD);
    }
}

vector<int> convolution(vector<int> a, vector<int> b) {
    int need = (int)a.size() + (int)b.size() - 1;
    int len = 1;
    while (len < need) len <<= 1;
    a.resize(len);
    b.resize(len);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < len; i++) a[i] = (long long)a[i] * b[i] % MOD;
    ntt(a, true);
    a.resize(need);
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<vector<int>> adj(n);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            --u; --v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // 两次 BFS 找直径
        auto bfs = [&](int s) {
            vector<int> dist(n, -1), par(n, -1);
            dist[s] = 0;
            queue<int> q;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : adj[u]) {
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        par[v] = u;
                        q.push(v);
                    }
                }
            }
            int far = 0;
            for (int i = 0; i < n; i++)
                if (dist[i] > dist[far]) far = i;
            return make_pair(far, make_pair(dist, par));
        };

        auto [u, res1] = bfs(0);
        auto [v, res2] = bfs(u);
        vector<int> distV = res2.first, parV = res2.second;
        int D = distV[v];

        // 提取直径路径,找中心边
        vector<int> path;
        for (int cur = v; cur != -1; cur = parV[cur]) path.push_back(cur);
        reverse(path.begin(), path.end());
        int m = D / 2;
        int a = path[m], b = path[m + 1];

        // 一侧:root 为根,不经过 banned,返回 LA 与可达 lca 深度集合
        auto side = [&](int root, int banned) {
            vector<int> dist(n, -1), par(n, -1);
            vector<int> order;
            dist[root] = 0;
            queue<int> q;
            q.push(root);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                order.push_back(u);
                for (int v : adj[u]) {
                    if (v == banned) continue;
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        par[v] = u;
                        q.push(v);
                    }
                }
            }
            int L = 0;
            for (int i = 0; i < n; i++) L = max(L, dist[i]);

            vector<char> inU(n, 0);
            for (int i = 0; i < n; i++)
                if (dist[i] == L) inU[i] = 1;

            vector<int> cntU(n, 0);
            vector<char> reach(L + 1, 0);
            vector<int> reachList;
            reach[L] = 1;
            reachList.push_back(L);

            for (int idx = (int)order.size() - 1; idx >= 0; idx--) {
                int u = order[idx];
                int c = inU[u] ? 1 : 0;
                int childU = 0;
                for (int w : adj[u]) {
                    if (w == banned) continue;
                    if (par[w] == u) {
                        if (cntU[w] > 0) childU++;
                        c += cntU[w];
                    }
                }
                cntU[u] = c;
                if (childU >= 2 && !reach[dist[u]]) {
                    reach[dist[u]] = 1;
                    reachList.push_back(dist[u]);
                }
            }
            return make_pair(L, reachList);
        };

        auto [LA, SA] = side(a, b);
        auto [LB, SB] = side(b, a);

        // Minkowski 和:1 + sa + sb,布尔卷积
        vector<int> PA(LA + 1, 0), PB(LB + 1, 0);
        for (int sa : SA) PA[sa] = 1;
        for (int sb : SB) PB[sb] = 1;
        vector<int> C = convolution(PA, PB);

        vector<int> ans;
        for (int t = 0; t < (int)C.size() && t <= LA + LB; t++) {
            if (C[t] > 0) ans.push_back(t + 1);
        }

        cout << ans.size();
        for (int k : ans) cout << ' ' << k;
        cout << '\n';
    }
    return 0;
}

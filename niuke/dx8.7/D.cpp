// Problem D. 天下第一武道会
// 官方题解：
// 有解充要条件：
//   1) 对每个 i<n，{P1..Pi} != {Q1..Qi}  （等价：不存在 i<n 使 max(s[1..i])==i，
//      s[i]=posQ[P[i]]）
//   2) 不存在 y!=x 同时满足 posP(y)>posP(x) 且 posQ(y)>posQ(x)
// 构造（贪心）：维护两个队首 u,v。
//   - 冠军 x 到队首后，始终取另一个；
//   - 否则取"对象"初始位置更靠前的（posQ(u) vs posP(v)，小者所在队首被淘汰）。
// 复杂度: 时间 O(n)，空间 O(n)
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, x;
    if (!(cin >> n >> x)) return 0;
    vector<int> P(n+1), Q(n+1), posP(n+1), posQ(n+1);
    for (int i = 1; i <= n; ++i) { cin >> P[i]; posP[P[i]] = i; }
    for (int i = 1; i <= n; ++i) { cin >> Q[i]; posQ[Q[i]] = i; }
    int a = posP[x], b = posQ[x];

    // 条件 2：不存在 y!=x 使 posP(y)>a 且 posQ(y)>b
    for (int w = 1; w <= n; ++w)
        if (posP[w] > a && posQ[w] > b) { cout << "NO\n"; return 0; }

    // 条件 1：不存在 i<n 使 max(s[1..i])==i
    int pmax = 0;
    for (int i = 1; i < n; ++i) {
        pmax = max(pmax, posQ[P[i]]);
        if (pmax == i) { cout << "NO\n"; return 0; }
    }

    // 贪心构造淘汰顺序
    vector<char> dead(n+1, 0);
    vector<int> elim;
    int pi = 1, qi = 1;
    while (true) {
        while (pi <= n && dead[P[pi]]) ++pi;
        while (qi <= n && dead[Q[qi]]) ++qi;
        int u = P[pi], v = Q[qi];   // 有解时 pi,qi<=n
        if (u == v) break;          // 只剩冠军 x，结束
        int take;
        if (u == x) take = v;
        else if (v == x) take = u;
        else if (posQ[u] < posP[v]) take = u;
        else take = v;
        elim.push_back(take);
        dead[take] = 1;
    }

    cout << "YES\n";
    for (int i = 0; i < (int)elim.size(); ++i) {
        if (i) cout << ' ';
        cout << elim[i];
    }
    cout << '\n';
    return 0;
}

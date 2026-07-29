#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 200005;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }//输入数据

    // 离散化
    vector<ll> vals = a;
    sort(vals.begin() + 1, vals.end());
    vals.erase(unique(vals.begin() + 1, vals.end()), vals.end());
    int m = vals.size() - 1;

    auto get_idx = [&](ll x) {
        return lower_bound(vals.begin() + 1, vals.end(), x) - vals.begin();
    };

    // f[j][i] : 以 i 结尾，长度为 j 的合法子序列的最大起始位置
    vector<vector<int>> f(2);          // f[1] 开始使用
    vector<vector<int>> pre_max(2);    // 前缀最大值

    // j = 1
    f[1].resize(n + 1);
    pre_max[1].resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        f[1][i] = i;
        pre_max[1][i] = i;
    }

    int size = 1;
    while (size < m) size <<= 1;
    vector<int> tree(2 * size, 0);

    int max_len = 1;
    for (int j = 2; j < 65; ++j) {   // 长度不会超过 60
        fill(tree.begin(), tree.end(), 0);
        vector<int> cur_f(n + 1, 0);
        bool any = false;

        for (int i = 1; i <= n; ++i) {
            ll ai = a[i];
            ll L = (ai + 2) / 3;        // ceil(ai/3)
            ll R = ai / 2;              // floor(ai/2)
            if (L <= R) {
                int idxL = lower_bound(vals.begin() + 1, vals.end(), L) - vals.begin();
                int idxR = upper_bound(vals.begin() + 1, vals.end(), R) - vals.begin() - 1;
                if (idxL <= idxR && idxL >= 1 && idxR <= m) {
                    // 查询区间 [idxL, idxR] 最大值
                    int l = idxL + size - 1;
                    int r = idxR + size - 1;
                    int mx = 0;
                    while (l <= r) {
                        if (l & 1) mx = max(mx, tree[l++]);
                        if (!(r & 1)) mx = max(mx, tree[r--]);
                        l >>= 1; r >>= 1;
                    }
                    if (mx > 0) {
                        cur_f[i] = mx;
                        any = true;
                    }
                }
            }

            // 将 j-1 层的结果插入线段树，供后续查询使用
            int val = f[j - 1][i];
            if (val > 0) {
                int pos = get_idx(ai);
                int p = pos + size - 1;
                if (val > tree[p]) {
                    tree[p] = val;
                    for (p >>= 1; p; p >>= 1)
                        tree[p] = max(tree[p << 1], tree[p << 1 | 1]);
                }
            }
        }

        if (!any) break;

        f.push_back(cur_f);
        pre_max.emplace_back(n + 1, 0);
        auto &pm = pre_max.back();
        for (int i = 1; i <= n; ++i) {
            pm[i] = max(pm[i - 1], cur_f[i]);
        }
        max_len = j;
    }

    // 回答询问
    while (q--) {
        int l, r;
        cin >> l >> r;
        int ans = 1;
        for (int j = max_len; j >= 2; --j) {
            if (j < (int)pre_max.size() && pre_max[j][r] >= l) {
                ans = j;
                break;
            }
        }
        cout << ans << '\n';
    }

    return 0;
}

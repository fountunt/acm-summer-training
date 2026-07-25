#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int N = n * m;          // 总格子数，也是值域上限（因为 ai,j ≤ n*m）

    // ---------- 第一部分：读取并统计每个数字的出现次数 ----------
    vector<int> val(N);            // 存储原始网格，按行优先展开为一维
    vector<int> cnt(N + 1, 0);     // cnt[x] 记录数字 x 的出现次数（值域 1..N）

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int x;
            cin >> x;
            val[i * m + j] = x;    // 保存原始值（之后用于计数排序）
            cnt[x]++;              // 统计频次
        }
    }

    // ---------- 第二部分：计数排序——构建每个数字的起始位置 ----------
    vector<int> start(N + 2, 0);   // start[x] 为数字 x 在 posR/posC 中的起始下标
    for (int x = 1; x <= N; ++x) {
        start[x + 1] = start[x] + cnt[x];
    }
    // 此时 start[x] = 所有小于 x 的数字的出现次数总和
    // 数字 x 的区间为 [start[x], start[x+1])

    vector<int> cur = start;       // cur 是用于填充的游标（可修改）
    vector<int> posR(N), posC(N);  // 存储每个格子的行、列（按数字分组连续存放）

    // ---------- 第三部分：将每个格子按数字填入连续区间 ----------
    for (int idx = 0; idx < N; ++idx) {
        int x = val[idx];          // 取原值
        int p = cur[x]++;          // 获取当前数字 x 的可插入位置，并自增
        posR[p] = idx / m;         // 行号
        posC[p] = idx % m;         // 列号
    }

    // 释放不再需要的数组，减少内存占用（可选）
    vector<int>().swap(val);
    vector<int>().swap(cnt);
    vector<int>().swap(cur);

    // ---------- 第四部分：二维差分数组初始化 ----------
    int W = m + 2;                 // 为了边界处理，预留一行一列
    vector<int> diff((n + 2) * W, 0); // 一维模拟二维差分，大小 (n+2)*(m+2)

    auto addRect = [&](int r1, int c1, int r2, int c2) {
        // 添加闭区间矩形 [r1,r2] × [c1,c2] 的二维差分标记
        diff[r1 * W + c1]++;
        diff[(r2 + 1) * W + c1]--;
        diff[r1 * W + (c2 + 1)]--;
        diff[(r2 + 1) * W + (c2 + 1)]++;
    };

    // ---------- 第五部分：按每个数字独立处理 ----------
    for (int x = 1; x <= N; ++x) {
        int l = start[x];
        int r = start[x + 1];
        int k = r - l;             // 数字 x 出现的总次数
        if (k < 2) continue;       // 少于两个格子，不可能产生矩形

        // ----- 5.1 将该数字按行聚合，得到每行的最小列和最大列 -----
        vector<int> rows;
        vector<int> mn, mx;
        rows.reserve(k);           // 预留空间优化性能（实际行数 ≤ k）
        mn.reserve(k);
        mx.reserve(k);

        int p = l;
        while (p < r) {
            int row = posR[p];
            int minc = posC[p], maxc = posC[p];

            // 同一行内连续（因为 posR 是按行优先放入的）
            while (p < r && posR[p] == row) {
                minc = min(minc, posC[p]);
                maxc = max(maxc, posC[p]);
                ++p;
            }

            rows.push_back(row);
            mn.push_back(minc);
            mx.push_back(maxc);
        }

        int K = (int)rows.size();
        if (K < 2) continue;       // 如果该数字只出现在一行，也不可能产生矩形

        // ----- 5.2 计算后缀最大值（列） -----
        vector<int> suffMax(K);
        suffMax[K - 1] = mx[K - 1];
        for (int i = K - 2; i >= 0; --i) {
            suffMax[i] = max(mx[i], suffMax[i + 1]);
        }

        // ----- 5.3 扫描分界线，添加矩形到差分 -----
        int prefMin = mn[0];        // 前缀最小值（列）
        for (int t = 1; t < K; ++t) {
            int L = prefMin;               // 分界线上方的最小列
            int R = suffMax[t];            // 分界线下方（含第 t 行）的最大列

            if (L < R) {
                int r1 = rows[t - 1];
                int r2 = rows[t];
                addRect(r1, L, r2, R);
            }

            prefMin = min(prefMin, mn[t]); // 更新前缀最小值
        }
    }

    // 释放位置信息（可选）
    vector<int>().swap(posR);
    vector<int>().swap(posC);
    vector<int>().swap(start);
    // suffMax 和 rows/mn/mx 已在循环内释放，此处无需处理

    // ---------- 第六部分：二维差分前缀和，还原覆盖情况 ----------
    for (int i = 0; i < n + 2; ++i) {
        for (int j = 0; j < m + 2; ++j) {
            int id = i * W + j;
            if (i > 0) diff[id] += diff[(i - 1) * W + j];
            if (j > 0) diff[id] += diff[i * W + (j - 1)];
            if (i > 0 && j > 0) diff[id] -= diff[(i - 1) * W + (j - 1)];
        }
    }

    // ---------- 第七部分：输出结果 ----------
    for (int i = 0; i < n; ++i) {
        string ans;
        ans.reserve(m);
        for (int j = 0; j < m; ++j) {
            ans.push_back(diff[i * W + j] > 0 ? '1' : '0');
        }
        cout << ans << '\n';
    }

    return 0;
}

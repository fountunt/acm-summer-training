#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MAXM = 200000 + 5;
const int INF = 1e9;

int n, m, k, q;
set<pair<int,int>> occupied;          // 当前占座 (x, y)
pair<int,int> person_pos[200000 + 5]; // 每个人座位
vector<int> col[MAXM];                // 每列的占用行

int minRow[MAXM];                     // 每列最早被遮挡的行（第一行不能坐）

// 分数结构体，表示 p/q (p>=0, q>0)
struct Frac
{
    ll p, q; // p/q
    Frac(ll _p = -1, ll _q = 1) : p(_p), q(_q) {
        if (q < 0) { p = -p; q = -q; }
    }
    bool operator<(const Frac& other) const {
        return p * other.q < other.p * q;
    }
    bool operator>(const Frac& other) const {
        return p * other.q > other.p * q;
    }
};

// 向上取整除法 (a + b - 1) / b，保证 a>=0, b>0
ll ceil_div(ll a, ll b) {
    return (a + b - 1) / b;
}

// 每次查询重新计算答案
ll query()
{
    // 清空列信息
    for (int i = 1; i <= m; ++i) col[i].clear();

    //读入占用信息
    for (auto &p : occupied)
    {
        int x = p.first, y = p.second;
        col[y].push_back(x);
    }

    for (int i = 1; i <= m; ++i) minRow[i] = n + 1;
    //初始化会被挡住的位置

    // ---- 同列 + 左侧扫描 ----
    Frac maxLeft(-1, 0); // 表示负无穷
    for (int i = 1; i <= m; ++i)
    {
        // 1) 同列
        for (int x : col[i])
        {
            minRow[i] = min(minRow[i], x);
        }

        // 2) 左侧贡献（只有 i>1 且 maxLeft 有效）
        if (i > 1 && maxLeft.p > 0)
        {
            // maxLeft = p/q 表示 (y-1)/x 的最大值
            // 遮挡行 = ceil( (i-1) * q / p )
            ll row = ceil_div(1LL * (i - 1) * maxLeft.q, maxLeft.p);
            if (row < minRow[i]) minRow[i] = (int)row;
        }

        // 3) 将本列点加入 maxLeft
        for (int x : col[i])
        {
            // 注意：y-1 = i-1, 分数为 (i-1)/x
            if (i - 1 > 0)
            {
                Frac cur(i - 1, x);
                if (cur > maxLeft) maxLeft = cur;
            }
        }
    }

    // ---- 右侧扫描 ----
    Frac maxRight(-1, 0);
    for (int i = m; i >= 1; --i)
    {
        // 右侧贡献（只有 i<m 且 maxRight 有效）
        if (i < m && maxRight.p > 0)
        {
            // maxRight = p/q 表示 (m-y)/x 的最大值
            ll row = ceil_div(1LL * (m - i) * maxRight.q, maxRight.p);
            if (row < minRow[i]) minRow[i] = (int)row;
        }

        // 将本列点加入 maxRight
        for (int x : col[i])
        {
            if (m - i > 0)
            {
                Frac cur(m - i, x);
                if (cur > maxRight) maxRight = cur;
            }
        }
    }

    // 统计答案
    ll ans = 0;
    for (int i = 1; i <= m; ++i)
    {
        ans += minRow[i] - 1; // 行号从1开始，好座位为 1..minRow[i]-1
    }
    return ans;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k >> q;

    for (int i = 1; i <= k; ++i)
    {
        int x, y;
        cin >> x >> y;
        occupied.insert({x, y});
        person_pos[i] = {x, y};
    }

    while (q--)
    {
        int p, x, y;
        cin >> p >> x >> y;

        // 移动第 p 个人
        auto old = person_pos[p];
        occupied.erase(old);
        occupied.insert({x, y});
        person_pos[p] = {x, y};

        cout << query() << '\n';
    }

    return 0;
}

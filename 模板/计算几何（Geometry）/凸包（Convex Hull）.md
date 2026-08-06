# 凸包（Convex Hull）

## 概念

给定平面点集，求出包含所有点的最小凸多边形边界（上凸包 / 下凸包）。用**单调链（Monotone Chain）** 在 O(n log n) 内完成。

**常见用途**：
- 几何最值：斜率、极值点。
- **支撑线 / 切线**：从外一点到凸包做切线，切线点可用二分求（O(log n)）。
- 与分块 / 线段树结合：每个块 / 节点存凸包，支持快速查询（见「分块」模板）。

---

## 代码 — 单调链求上凸包 O(n log n)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> Point;   // (x, y)

// 上凸包：按 x 升序（相同 x 保留 y 大的），维护斜率递减
vector<Point> upper_hull(vector<Point> pts){
    sort(pts.begin(), pts.end());
    vector<Point> up;
    for(auto &C : pts){
        while(up.size() >= 2){
            auto A = up[up.size()-2], B = up[up.size()-1];
            // 叉积 (B-A) × (C-B)；> 0 表示 C 在 AB 左侧（凸起）
            __int128 cr = (__int128)(B.first-A.first)*(C.second-B.second)
                        - (__int128)(B.second-A.second)*(C.first-B.first);
            if(cr < 0) break;      // C 严格在上方 → 保留
            up.pop_back();
        }
        up.push_back(C);
    }
    return up;   // 按 x 递增
}
```

> 完整凸包 = 下凸包 + 上凸包（单调链），最后拼起来即是按逆时针的凸包。

---

## 经典查询：从点 p 到凸包上找「使 f(q) 取峰值的点」

很多题目需要在凸包上对某个单峰函数二分（比如"从某点出发、斜率最大/最小的点"）。
只要沿凸包（按 x 有序）函数是单峰的，就能二分。

**示例**：给定点 p 和一个不含 p 的点集的上凸包，找 q 最大化 `(a_p + a_q)/(b_p + b_q)`。

```cpp
// 比较 f(k1) 与 f(k2)：f(k) = (a_p + a_k)/(b_p + b_k)
bool leqAt(int k1,int k2, pair<ll,ll> p, const vector<pair<ll,ll>>& up){
    __int128 L = (__int128)(p.second + up[k1].second) * (p.first + up[k2].first);
    __int128 R = (__int128)(p.second + up[k2].second) * (p.first + up[k1].first);
    return L <= R;
}
// 单峰二分求峰值（取最右的峰值）
pair<ll,ll> bestOnHull(const vector<pair<ll,ll>>& up, pair<ll,ll> p){
    int lo = 0, hi = (int)up.size() - 1;
    while(lo < hi){
        int mid = (lo + hi) / 2;
        if(leqAt(mid, mid+1, p, up)) lo = mid + 1;  // 上升段 → 峰值在右
        else hi = mid;                               // 下降段 → 峰值在左
    }
    return up[lo];
}
```

> 用 `__int128` 做叉积 / 比较，避免坐标相乘溢出（坐标到 1e9 时乘积达 1e18，int64 仍安全，但更高时务必用 __int128）。

---

## 模板题应用示例

> 来源：牛客多校「加除」。区间查询 `max (a_i+a_j)/(b_i+b_j)`。
> 每块预处理上凸包；询问时对不含最大斜率点 p 的块，在凸包上二分求 p 的最佳搭档；
> 该函数沿上凸包单峰（已对拍验证）。配合分块模板得到 O(√n log n) 的询问。

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 叉积符号 | 用 `__int128` 计算叉积，防止溢出；>0 / <0 的方向取决于凸包是"上"还是"下" |
| 相同 x | 相同 x 时上凸包只留 y 最大的点 |
| 单峰性 | 二分前务必确认目标函数沿凸包是单峰的；否则需扫描或换方法 |
| 凸包大小 | 凸包顶点数可能到 O(n)，动态维护（点更新）代价高，常配合分块静态重建 |
| 分数比较 | 比较两个分数 `a/b` 与 `c/d` 用 `a*d` 与 `c*b`，用 `__int128` |

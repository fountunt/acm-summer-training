# 分块（Sqrt Decomposition）

## 概念

把数组分成大小约为 `√n` 的若干**块**。对每块**预处理**出该块的信息（排序、凸包、最值等）。
- **点更新**：只重建所在块，O(B log B)。
- **区间查询**：完整块用预处理结果，两端不完整的"边角"直接扫描，O(n/B · 单块查询代价 + B)。

通过块大小 `B ≈ √n` 平衡「块数 n/B」与「块大小 B」，得到 O(√n · log n) 左右的单次查询。

**适用场景**：区间查询 + 点更新，且单块信息可以预处理、快速合并（排序、凸包、最值、前缀和……）。

---

## 通用框架

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Block {
    vector<pair<ll,ll>> pts;   // 块内元素（可带辅助信息）
    // …… 预处理出的块信息（如排序后数组 / 凸包 / 块内最值）……
    int bestIdx;               // 示例：块内某个指标最优的元素下标
};

int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<ll> a(n);           // 数组
    for(int i=0;i<n;i++) cin >> a[i];

    const int B = 350;                          // 块大小（≈√n）
    int nb = (n + B - 1) / B;
    vector<int> blOf(n);                        // 每个下标所属块
    vector<Block> blocks(nb);

    // 重建第 bi 块：从数组取回元素，重新预处理
    auto rebuild = [&](int bi){
        int l = bi*B, r = min(n, (bi+1)*B);
        blocks[bi].pts.clear();
        for(int i=l;i<r;i++) blocks[bi].pts.push_back({a[i], i});
        // …… 对 blocks[bi].pts 做排序 / 建凸包 / 求最值等 O(B log B) 预处理……
    };
    for(int bi=0;bi<nb;bi++){
        for(int i=bi*B;i<min(n,(bi+1)*B);i++) blOf[i]=bi;
        rebuild(bi);
    }

    // 处理询问
    for(int qi=0;qi<q;qi++){
        int typ; cin >> typ;
        if(typ == 1){ int i; ll x; cin>>i>>x; i--; a[i]=x; rebuild(blOf[i]); }   // 更新 → 重建块
        else {
            int l, r; cin >> l >> r; l--; r--;
            // 收集完整块 + 边角下标
            vector<int> fullBlocks, partialIdx;
            int bl1 = blOf[l], bl2 = blOf[r];
            if(bl1 == bl2){ for(int i=l;i<=r;i++) partialIdx.push_back(i); }
            else {
                for(int i=l;i<min(n,(bl1+1)*B);i++) partialIdx.push_back(i);
                for(int i=bl2*B;i<=r;i++) partialIdx.push_back(i);
                for(int bi=bl1+1;bi<bl2;bi++) fullBlocks.push_back(bi);
            }
            // …… 边角直接扫描，完整块用预处理信息合并，得到答案……
        }
    }
    return 0;
}
```

---

## 模板题应用示例：区间「最大斜率点 + 最佳搭档」

> 来源：牛客多校「加除」。维护数组 a、b，询问区间内 `max (a_i+a_j)/(b_i+b_j)`。
> 关键观察：最优对子一定含**最大斜率点 p**（a/b 最大）；另一个点 q（p 的最佳搭档）在不含 p 的块内
> 必在块的**上凸包**上，且函数单峰可二分。
>
> 分块做法：
> 1. 每块预处理：上凸包、块内最大斜率点。
> 2. 询问：① 边界扫描 + 各块 max-slope 取最大得 p；② 边界扫描 + 不含 p 的块在凸包上二分、
>    p 所在块整体扫描，得 q；③ 答案 `(a_p+a_q)/(b_p+b_q)` 约分。
>
> 复杂度：询问 O((n/B)log B + B)，更新 O(B log B)。

```cpp
// 块内"给定 p，求 p 的最佳搭档"：在块的上凸包 up 上二分峰值
// f(k) = (a_p + a_k)/(b_p + b_k)，沿上凸包单峰
bool leqAt(int k1,int k2,pair<ll,ll> p,const vector<pair<ll,ll>>& up){
    __int128 L=(__int128)(p.second+up[k1].second)*(p.first+up[k2].first);
    __int128 R=(__int128)(p.second+up[k2].second)*(p.first+up[k1].first);
    return L <= R;
}
pair<ll,ll> bestPartnerInBlock(const vector<pair<ll,ll>>& up, pair<ll,ll> p){
    int lo=0, hi=(int)up.size()-1;
    while(lo<hi){ int mid=(lo+hi)/2; if(leqAt(mid,mid+1,p,up)) lo=mid+1; else hi=mid; }
    return up[lo];
}
```

---

## 注意事项

| 要点 | 说明 |
|------|------|
| 块大小 B | 取 `B ≈ √n`（350 左右），用 `max(B log B, (n/B)·查询代价)` 平衡 |
| 更新 | 只重建所在块，不要重建全部 |
| 边角扫描 | 区间两端不完整的块必须直接扫描，完整块才能用预处理 |
| 完整块判断 | `bl >= l && br-1 <= r` 才算完整块 |
| 排序每块 | 重建时对块内元素重新排序即可，O(B log B) |
| 适合场景 | 查询 / 更新其中一方偏多、块内可预处理的信息 |

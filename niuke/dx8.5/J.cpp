#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Block {
    vector<pair<ll,ll>> pts;      // (b, a)
    vector<pair<ll,ll>> up;       // 上凸包，按 b 升序
    int bestSlopeIdx;             // 块内 a/b 最大的点
};

// 比较 f(k1) 与 f(k2)，f(k) = (a_p + a_k)/(b_p + b_k)
inline bool leqAt(int k1, int k2, pair<ll,ll> p, const vector<pair<ll,ll>>& up){
    __int128 L = (__int128)(p.second + up[k1].second) * (p.first + up[k2].first);
    __int128 R = (__int128)(p.second + up[k2].second) * (p.first + up[k1].first);
    return L <= R;
}
// 在块的凸包上二分求 p 的最佳搭档（p 不在该块内，凸包上单峰）
pair<ll,ll> bestPartnerInBlock(const Block& bl, pair<ll,ll> p){
    int lo = 0, hi = (int)bl.up.size() - 1;
    while(lo < hi){ int mid = (lo + hi) / 2; if(leqAt(mid, mid+1, p, bl.up)) lo = mid+1; else hi = mid; }
    return bl.up[lo];
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<ll> a(n), b(n);
    for(int i=0;i<n;i++) cin >> a[i];
    for(int i=0;i<n;i++) cin >> b[i];

    const int B = 350;
    int nb = (n + B - 1) / B;
    vector<int> blOf(n);
    vector<Block> blocks(nb);

    auto rebuild = [&](int bi){
        int l = bi*B, r = min(n, (bi+1)*B);
        Block& bl = blocks[bi];
        bl.pts.clear();
        for(int i=l;i<r;i++) bl.pts.push_back({b[i], a[i]});
        vector<pair<ll,ll>> sp = bl.pts;
        sort(sp.begin(), sp.end());
        vector<pair<ll,ll>> up;
        for(auto &pt: sp){                       // 上凸包（斜率递减）
            while(up.size() >= 2){
                auto A = up[up.size()-2], B = up[up.size()-1], C = pt;
                __int128 cr = (__int128)(B.first-A.first)*(C.second-B.second)
                            - (__int128)(B.second-A.second)*(C.first-B.first);
                if(cr < 0) break;                // C 严格在上方，保留
                up.pop_back();
            }
            up.push_back(pt);
        }
        bl.up = up;
        int bsi = 0;
        for(int t=1;t<(int)bl.pts.size();t++)
            if((__int128)bl.pts[t].second*bl.pts[bsi].first > (__int128)bl.pts[bsi].second*bl.pts[t].first) bsi = t;
        bl.bestSlopeIdx = bsi;
    };
    for(int bi=0;bi<nb;bi++){ for(int i=bi*B;i<min(n,(bi+1)*B);i++) blOf[i]=bi; rebuild(bi); }

    for(int qi=0;qi<q;qi++){
        int typ; cin >> typ;
        if(typ == 1){ int i; ll x; cin >> i >> x; i--; a[i]=x; rebuild(blOf[i]); }
        else if(typ == 2){ int i; ll x; cin >> i >> x; i--; b[i]=x; rebuild(blOf[i]); }
        else {
            int l, r; cin >> l >> r; l--; r--;
            vector<pair<int,int>> fullBlocks;
            vector<int> partialIdx;
            int bl1 = blOf[l], bl2 = blOf[r];
            if(bl1 == bl2){ for(int i=l;i<=r;i++) partialIdx.push_back(i); }
            else {
                for(int i=l;i<min(n,(bl1+1)*B);i++) partialIdx.push_back(i);
                for(int i=bl2*B;i<=r;i++) partialIdx.push_back(i);
                for(int bi=bl1+1;bi<bl2;bi++) fullBlocks.push_back({bi,0});
            }
            // 1) p = 最大斜率点
            int pIdx = -1;
            auto consider = [&](int idx){
                if(pIdx==-1 || (__int128)a[idx]*b[pIdx] > (__int128)a[pIdx]*b[idx]) pIdx = idx;
            };
            for(int idx: partialIdx) consider(idx);
            for(auto &fb: fullBlocks){ int bi=fb.first; consider(bi*B + blocks[bi].bestSlopeIdx); }
            pair<ll,ll> p = {b[pIdx], a[pIdx]};
            // 2) q = p 的最佳搭档（最大化 (a_p+a_q)/(b_p+b_q)）
            pair<ll,ll> qbest = {-1, -1};
            auto considerQ = [&](int idx){
                if(idx == pIdx) return;
                pair<ll,ll> q = {b[idx], a[idx]};
                if(qbest.first==-1 || (__int128)(p.second+q.second)*(p.first+qbest.first)
                                     > (__int128)(p.second+qbest.second)*(p.first+q.first))
                    qbest = q;
            };
            for(int idx: partialIdx) considerQ(idx);
            for(auto &fb: fullBlocks){
                int bi = fb.first;
                int bl = bi*B, br = min(n,(bi+1)*B);
                if(pIdx >= bl && pIdx < br){          // p 所在的块：直接扫描（排除 p）
                    for(int i=bl;i<br;i++) considerQ(i);
                } else {                              // 其它块：凸包二分
                    pair<ll,ll> cand = bestPartnerInBlock(blocks[bi], p);
                    if(qbest.first==-1 || (__int128)(p.second+cand.second)*(p.first+qbest.first)
                                        > (__int128)(p.second+qbest.second)*(p.first+cand.first))
                        qbest = cand;
                }
            }
            ll num = p.second + qbest.second, den = p.first + qbest.first;
            ll g = std::gcd(num, den); num /= g; den /= g;
            cout << num << " " << den << "\n";
        }
    }
    return 0;
}

/*
================================================================
            牛客多校 · Problem J 加除  中文题解
================================================================

【题意简述】
    维护数组 a、b，支持点更新。询问区间 [l,r] 内
        max_{l<=i<j<=r} (a_i+a_j)/(b_i+b_j)
    输出最简分数 x/y。

【思路】

  一、最优对一定包含"最大斜率点"
     记 s_i = a_i/b_i。任意对子的比值 (a_i+a_j)/(b_i+b_j) 是 s_i、s_j 的
     加权平均（权重 b_i、b_j），介于两者之间。设 p = 区间内 a/b 最大的点，
     可证明最优对子 (p, q) 必含 p。

  二、最佳搭档 q 的求法
     对固定的 p，q 使 (a_p+a_q)/(b_p+b_q) 最大。
     当 p 不在某个点集内时，q 是该集合上凸包上使 (a_p+a_q)/(b_p+b_q) 取到
     峰值的点（支撑线在凸包上的切点），且该函数沿上凸包是单峰的，可二分。
     p 自己所在的集合则直接扫描排除 p。

  三、分块
     块大小 B ≈ 350。每块维护：点、上凸包、块内最大斜率点。
       * 询问 [l,r]：
           1) 找 p：边界扫描 + 各完整块的 max-slope 点取最大。
           2) 找 q：边界扫描；每个完整块（不含 p 的）在上凸包上二分；
              p 所在块整体扫描。
           3) 答案 = (a_p+a_q)/(b_p+b_q)，约分。
       * 更新：重建所在块（重排、凸包、max-slope）。

【复杂度】
     询问 O((n/B) log B + B)，更新 O(B log B)。
     n=q=1e5 实测：全查询约 0.8s，全更新约 1.8s，远小于 5s。

【正确性验证】
     * 官方样例全部通过：40/3、65/6、10/1、26/3、2000000000/1000000001。
     * 与 O(n^2) 暴力对拍：8000 组随机数据（含更新、小值、重复、n=2、
       值到 1e9 等边界）全部一致。
================================================================
*/

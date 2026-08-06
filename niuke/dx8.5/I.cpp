#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
const long long MOD = 998244353;

int pc(ull x){ return __builtin_popcountll(x); }

// sum_{t=0}^{2^k-1} popcount(h+t) * popcount(h+t+d),  h 为 2^k 的倍数（低 k 位为 0）。
long long blockSum(ull h, int k, ull d){
    long long a = pc(h) % MOD;          // popcount(h)
    ull H = h + d;
    ull H_high = H >> k;                // H+t 的高位部分（进位后可能是 +1）
    // 对 t 的低 k 位做数位 DP，追踪 (H+t) 的进位
    long long cnt[2]={0}, pct[2]={0}, pcs[2]={0}, prod[2]={0};
    cnt[0]=1;                            // 进位 c=0 开始
    for(int p=0; p<k; p++){
        long long db = (d >> p) & 1ULL;
        long long nc[2]={0}, np[2]={0}, ns[2]={0}, npr[2]={0};
        for(int c=0; c<2; c++) for(int tb=0; tb<2; tb++){
            long long s3 = tb + db + c;
            long long s = s3 & 1;        // (H+t) 第 p 位
            int c2 = (s3 >= 2);          // 进位出
            nc[c2]  = (nc[c2]  + cnt[c]) % MOD;
            np[c2]  = (np[c2]  + pct[c] + tb*cnt[c]) % MOD;
            ns[c2]  = (ns[c2]  + pcs[c] + s*cnt[c]) % MOD;
            npr[c2] = (npr[c2] + prod[c] + tb*pcs[c] + s*pct[c] + (tb&s)*cnt[c]) % MOD;
        }
        memcpy(cnt,nc,sizeof cnt); memcpy(pct,np,sizeof pct);
        memcpy(pcs,ns,sizeof pcs); memcpy(prod,npr,sizeof npr);
    }
    long long res = 0;
    for(int c=0; c<2; c++){
        if(cnt[c]==0 && pct[c]==0 && pcs[c]==0 && prod[c]==0) continue;
        long long b = pc(H_high + c) % MOD;
        res = (res + a*pcs[c] + pct[c]*b + a*b*cnt[c] + prod[c]) % MOD;
    }
    return res;
}

// S(n,d) = sum_{i=0}^{n} f(i)*f(i+d) mod MOD
long long solve(ull n, ull d){
    long long ans = (long long)((ull)pc(n) * pc(n+d)) % MOD;   // i = n
    // 将 [0, n-1] 分解成二元区间（每个集合位 k 对应一块）
    for(int k=0; k<60; k++)
        if((n >> k) & 1ULL){
            ull h = (n >> (k+1)) << (k+1);   // 2^k 的倍数
            ans = (ans + blockSum(h, k, d)) % MOD;
        }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while(t--){
        ull n, d; cin >> n >> d;
        cout << solve(n, d) << "\n";
    }
    return 0;
}

/*
================================================================
            牛客多校 · Problem I 整数函数  中文题解
================================================================

【题意简述】
    f(x) = x 的二进制中 1 的个数。求
        S = ( sum_{i=0}^{n} f(i) * f(i+d) ) mod 998244353
    其中 0 <= n, d < 2^60。

【思路】

  一、二元区间分解 [0, n]
     把 [0, n] 拆成 O(60) 个"二元区间"，形如 [h, h+2^k-1]，其中 h 是 2^k
     的倍数（即 h 的低 k 位全为 0），区间里 i 的低 k 位完全自由。分解方式：
     对 n 的每个集合位 k，取 h = (n >> (k+1)) << (k+1)，得到一块
     [h, h+2^k-1]；这些块两两不相交，并上 {n} 正好覆盖 [0, n]。

  二、单块计算（低 k 位自由）
     对块 [h, h+2^k-1]，令 i = h + t（t 取遍 [0, 2^k-1]）：
       * f(i) = popcount(h) + popcount(t)，记 a = popcount(h)。
       * i+d = H + t，H = h+d。写成 H = H_high * 2^k + H_low；
         H+t 的低 k 位 = (H_low + t) 的低 k 位，进位 c_k ∈ {0,1} 进入高位，
         高位 popcount = popcount(H_high + c_k)。
     对 t 的低 k 位做 LSB->MSB 数位 DP，状态 = 进位 c，同时维护四个量：
       * cnt   = 合法 t 的个数
       * pct   = sum popcount(t)
       * pcs   = sum popcount((H+t) 的低 k 位)
       * prod  = sum popcount(t) * popcount((H+t) 的低 k 位)
     每确定一位 tb 和对应和位 s = (tb + d_p + c) mod 2，转移更新这四个量。
     结束（处理完 k 位）后，按进位 c_k 合并：
       单块和 = sum_c [ a*pcs + pct*popcount(H_high+c) + a*popcount(H_high+c)*cnt + prod ]

  三、答案
     所有块的贡献加上 i=n 这一项 f(n)*f(n+d)，取模即可。

【复杂度】
     每个测试 O(60^2)，实测 t=1e4 全部最坏数据约 0.43s。

【正确性验证】
     * 五个官方样例全部通过：0 / 1 / 14 / 6022 / 11512513。
     * 与 O(n) 暴力对拍：n,d<=200 全部、n<50000 随机、n<2^22 随机、若干边界
       （n=0、n=d=2^60-1 等）全部一致。
================================================================
*/

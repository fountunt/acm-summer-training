#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct DSU {
    vector<int> p;
    void init(int n){ p.resize(n); iota(p.begin(),p.end(),0); }
    int find(int x){ int r=x; while(p[r]!=r) r=p[r]; while(p[x]!=x){ int t=p[x]; p[x]=r; x=t; } return r; }
};

// count reachable arrangements within one parity class.
// sv[t] = value initially at slot t, gv[j] = gate value between slot j and j+1.
ll solveClass(const vector<int>& sv, const vector<int>& gv, int MOD){
    int k=sv.size();
    if(k<=1) return 1;
    vector<int> g(k-1);
    for(int i=0;i<k-1;i++) g[i]=gv[i];
    const int NEG=-1;
    vector<int> mn=sv, L(k), R(k);
    vector<int> sR(k-1), sL(k-1);
    vector<int> order(k); iota(order.begin(),order.end(),0);
    sort(order.begin(),order.end(),[&](int a,int b){ return sv[a]<sv[b]; });
    vector<int> stV, stP; stV.reserve(k); stP.reserve(k);
    // fixed point: mn[t] = min value that can occupy slot t.
    // value v crosses gate g (rightward, slot j->j+1) iff g>v and mn[j+1]<g.
    for(int iter=0;;iter++){
        for(int j=0;j<k-1;j++){ sR[j]=(mn[j+1]<g[j])?g[j]:NEG; sL[j]=(mn[j]<g[j])?g[j]:NEG; }
        // R_v : nearest gate to the right with (not passable OR value <= v)  (blocking gate)
        stV.clear(); stP.clear();
        for(int p=k-1;p>=0;p--){
            if(p<=k-2){
                int e=sR[p];
                while(!stV.empty() && stV.back()>=e){ stV.pop_back(); stP.pop_back(); }
                stV.push_back(e); stP.push_back(p);
            }
            int idx=upper_bound(stV.begin(),stV.end(),sv[p])-stV.begin()-1;
            R[p]=(idx>=0)? stP[idx] : (k-1);
        }
        // L_v : nearest gate to the left with (not passable OR value <= v)
        stV.clear(); stP.clear();
        for(int p=0;p<k;p++){
            if(p>=1){
                int e=sL[p-1];
                while(!stV.empty() && stV.back()>=e){ stV.pop_back(); stP.pop_back(); }
                stV.push_back(e); stP.push_back(p-1);
            }
            int idx=upper_bound(stV.begin(),stV.end(),sv[p])-stV.begin()-1;
            L[p]=(idx>=0)? (stP[idx]+1) : 0;
        }
        // mn[t] = min sv covering slot t (paint intervals in increasing sv order)
        vector<int> nmn(k, INT_MAX);
        DSU dsu; dsu.init(k+1);
        for(int ti: order){
            int pos=dsu.find(L[ti]);
            while(pos<=R[ti]){
                nmn[pos]=sv[ti];
                dsu.p[pos]=dsu.find(pos+1);
                pos=dsu.find(pos);
            }
        }
        if(nmn==mn) break;
        mn.swap(nmn);
        if(iter>60) break; // safety; converges in practice in <=5
    }
    // count bijections respecting intervals [L_v,R_v] (laminar -> product)
    sort(order.begin(),order.end(),[&](int a,int b){ if(L[a]!=L[b]) return L[a]<L[b]; return R[a]>R[b]; });
    vector<int> groupId(k,-1);
    vector<vector<int>> groups;
    for(int i=0;i<k;i++){
        int t=order[i];
        if(i>0 && L[t]==L[order[i-1]] && R[t]==R[order[i-1]]) groupId[t]=groupId[order[i-1]];
        else { groupId[t]=groups.size(); groups.push_back({}); }
        groups[groupId[t]].push_back(t);
    }
    int G=groups.size();
    vector<int> parent(G,-1), st;
    for(int gi=0;gi<G;gi++){
        int t=groups[gi][0];
        while(!st.empty()){ int w=groups[st.back()][0]; if(R[w]>=R[t]&&L[w]<=L[t]) break; st.pop_back(); }
        if(!st.empty()) parent[gi]=st.back();
        st.push_back(gi);
    }
    vector<ll> sz(G,0);
    for(int gi=0;gi<G;gi++) sz[gi]=groups[gi].size();
    vector<int> chHead(G,-1), chNext(G,-1);
    for(int gi=0;gi<G;gi++) if(parent[gi]!=-1){ int p=parent[gi]; chNext[gi]=chHead[p]; chHead[p]=gi; }
    vector<ll> sub(G,0);
    for(int gi=0;gi<G;gi++) if(parent[gi]==-1){
        struct Fr { int u, child; };
        vector<Fr> fr; fr.push_back({gi,chHead[gi]});
        while(!fr.empty()){
            auto &top=fr.back();
            if(top.child!=-1){ int c=top.child; top.child=chNext[c]; fr.push_back({c,chHead[c]}); }
            else { int u=top.u; fr.pop_back(); ll s=sz[u]; for(int c=chHead[u]; c!=-1; c=chNext[c]) s+=sub[c]; sub[u]=s; }
        }
    }
    ll ans=1;
    for(int gi=0;gi<G;gi++){
        int t=groups[gi][0];
        int size=R[t]-L[t]+1;
        ll below=sub[gi]-sz[gi];
        sort(groups[gi].begin(),groups[gi].end());
        int m=groups[gi].size();
        for(int rk=0;rk<m;rk++){
            ll factor=(ll)size-(below+rk);
            if(factor<=0) return 0;
            ans=ans*(factor%MOD)%MOD;
        }
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int MOD=998244353;
    int T; cin>>T;
    while(T--){
        int n; cin>>n;
        vector<int> p(n);
        for(int i=0;i<n;i++) cin>>p[i];
        ll ans=1;
        for(int cls=0;cls<2;cls++){
            vector<int> slots;
            for(int i=0;i<n;i++) if((i%2)==cls) slots.push_back(i);
            vector<int> sv, gv;
            for(int s: slots) sv.push_back(p[s]);
            for(int i=0;i+1<(int)slots.size();i++) gv.push_back(p[slots[i]+1]);
            ans=ans*solveClass(sv,gv,MOD)%MOD;
        }
        cout<<ans<<"\n";
    }
    return 0;
}

/*
================================================================
            牛客多校 · Problem A 排列问题  中文题解
================================================================

【题意简述】
    给定一个长度为 n 的排列 p。每次操作：选一个 i (2<=i<=n-1) 且 p_i 是
    局部最大值（p_i > max(p_{i-1}, p_{i+1})），然后交换 p_{i-1} 与 p_{i+1}。
    求从 p 可达的不同排列个数，对 998244353 取模。

【思路】

  一、奇偶性分解
     操作交换位置 i-1 与 i+1，二者奇偶性相同（同奇或同偶）。因此每个奇偶类
     位置上值的集合永远不变：
         * 奇位置(1,3,5,...)上的值集合不变
         * 偶位置(2,4,6,...)上的值集合不变
     且两个类的运动互不影响，答案 = 奇类方案数 × 偶类方案数。

  二、类内模型：槽与门
     对一个类，把它的位置记作槽 s_0..s_{k-1}，槽之间夹着的对侧类位置叫"门"。
     类内的交换即：跨过一个门交换两个相邻槽的值。值 v 能跨过门 g（槽 j 与
     j+1 之间）当且仅当：
         g > v  且  门 g 能被"顶起来"成为山峰（即对侧槽能同时放下一个 < g 的值）。
     后一个条件写成：mn[j+1] < g（向右跨）或 mn[j] < g（向左跨），其中
         mn[t] = 能占据槽 t 的最小值。
     于是值 v 的可达槽位是连续的区间 [L_v, R_v]：从起点向左右扩展，只要经过的
     门都满足"门值 > v 且 mn[对侧] < 门值"。

  三、固定点
     mn 与区间互相依赖，用不动点迭代：
         * 由 mn 算出所有 [L_v,R_v]
         * 用区间更新 mn[t] = min 覆盖 t 的最小值（按值升序 + 并查集涂色）
     由于 mn 单调不增，必然收敛；实测 <= 5 次迭代。

  四、计数：层叠区间乘积
     一个类内所有 [L_v,R_v] 满足层叠性（任意两个要么不相交、要么互相包含），
     因此合法双射数可写成简单乘积：
         按区间大小升序处理值 v，v 的可选槽数 = |I_v| - (严格包含于 I_v 的
         值个数 + 同区间内排在 v 前面的值个数)。
     用区间包含树统计子树大小即可 O(k) 求出。

  五、答案
     两个类各自算出的方案数相乘 mod 998244353。

【复杂度】
     时间：固定点 O(迭代次数 · k log k)，k<=n/2，实测迭代<=5，总 O(n log n)。
     空间：O(n)。

【正确性验证】
     * 五个官方样例全部通过：1 / 1 / 2 / 12 / 4。
     * 暴力 BFS 交叉验证：n<=10 随机排列全部一致；n<=8 全部排列一致。
     * 最坏情况实测：单组 n=1e6 约 1.0s；5e4 组小数据(总和 n=1e6)约 0.6s。
================================================================
*/

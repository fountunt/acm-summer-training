#include <bits/stdc++.h>
using namespace std;

const int V = 4;          // Grundy 值域 0..3

// ---- 函数(值域 {0..3} -> {0..3})打包成 8 位, 每个输入 2 位 ----
int pack(int f0,int f1,int f2,int f3){ return f0 | (f1<<2) | (f2<<4) | (f3<<6); }
inline int applyf(int f,int a){ return (f >> (2*a)) & 3; }
const int IDENT = pack(0,1,2,3);
int COMP[256][256];       // COMP[g][h] = g∘h

int mex_v(int a,int b,int c){
    bool seen[4]={};
    if(a>=0) seen[a]=true;
    if(b>=0) seen[b]=true;
    if(c>=0) seen[c]=true;
    for(int m=0;m<4;m++) if(!seen[m]) return m;
    return 0;
}
// 组合两个子游戏: -1 表示该侧不存在
int comb(int a,int b){
    if(a==-1 && b==-1) return mex_v(0,-1,-1);   // = mex{0} = 1
    if(a==-1) return mex_v(0,b,-1);
    if(b==-1) return mex_v(0,a,-1);
    return mex_v(a,b,a^b);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for(int g=0;g<256;g++)for(int h=0;h<256;h++){
        COMP[g][h] = pack(applyf(g,applyf(h,0)), applyf(g,applyf(h,1)),
                          applyf(g,applyf(h,2)), applyf(g,applyf(h,3)));
    }

    int t; cin >> t;
    while(t--){
        int n, m; cin >> n >> m;
        vector<int> p(n);
        for(int i=0;i<n;i++) cin >> p[i];

        // ---- 最小笛卡尔树 (按 p 值最小堆, 中序 = 位置) ----
        vector<int> lc(n,-1), rc(n,-1), par(n,-1);
        vector<int> st;
        for(int i=0;i<n;i++){
            int last=-1;
            while(!st.empty() && p[st.back()] > p[i]){ last=st.back(); st.pop_back(); }
            if(last!=-1){ lc[i]=last; par[last]=i; }
            if(!st.empty()){ rc[st.back()]=i; par[i]=st.back(); }
            st.push_back(i);
        }
        int root = st[0];

        // ---- 迭代后序: depth, lo, hi, g ----
        vector<int> depth(n), lo(n), hi(n), g(n);
        {
            vector<pair<int,int>> stk; stk.push_back({root,0});
            while(!stk.empty()){
                auto [v,d]=stk.back(); stk.pop_back();
                depth[v]=d;
                if(lc[v]!=-1) stk.push_back({lc[v],d+1});
                if(rc[v]!=-1) stk.push_back({rc[v],d+1});
            }
            vector<int> order, stack{root};
            while(!stack.empty()){
                int v=stack.back(); stack.pop_back();
                order.push_back(v);
                if(lc[v]!=-1) stack.push_back(lc[v]);
                if(rc[v]!=-1) stack.push_back(rc[v]);
            }
            for(int idx=(int)order.size()-1; idx>=0; idx--){
                int v=order[idx];
                lo[v]=hi[v]=v;
                if(lc[v]!=-1) lo[v]=lo[lc[v]];
                if(rc[v]!=-1) hi[v]=hi[rc[v]];
                g[v]=comb(lc[v]!=-1? g[lc[v]] : -1, rc[v]!=-1? g[rc[v]] : -1);
            }
        }

        // ---- baseP/baseQ: 在节点 x 处终止时的值 ----
        vector<int> baseP(n), baseQ(n);
        for(int v=0;v<n;v++){
            baseP[v] = (rc[v]!=-1) ? comb(g[rc[v]], -1) : 1;
            baseQ[v] = (lc[v]!=-1) ? comb(g[lc[v]], -1) : 1;
        }

        // ---- 倍增: up, funP, funQ ----
        int LOG = 1; while((1<<LOG) <= n) LOG++;
        vector<vector<int>> up(LOG, vector<int>(n));
        vector<vector<unsigned char>> funP(LOG, vector<unsigned char>(n));
        vector<vector<unsigned char>> funQ(LOG, vector<unsigned char>(n));
        auto wrapP = [&](int w){  // 从 w 的左孩子向上走: a -> comb(a, g[rc[w]]|EMPTY)
            int rv = rc[w]!=-1 ? g[rc[w]] : -1;
            return pack(comb(0,rv),comb(1,rv),comb(2,rv),comb(3,rv));
        };
        auto wrapQ = [&](int w){  // 从 w 的右孩子向上走: a -> comb(g[lc[w]]|EMPTY, a)
            int lv = lc[w]!=-1 ? g[lc[w]] : -1;
            return pack(comb(lv,0),comb(lv,1),comb(lv,2),comb(lv,3));
        };
        for(int v=0;v<n;v++){
            up[0][v] = (par[v]!=-1) ? par[v] : v;   // 根自环
            if(par[v]!=-1 && lc[par[v]]==v) funP[0][v] = wrapP(par[v]); else funP[0][v] = IDENT;
            if(par[v]!=-1 && rc[par[v]]==v) funQ[0][v] = wrapQ(par[v]); else funQ[0][v] = IDENT;
        }
        for(int j=1;j<LOG;j++){
            for(int v=0;v<n;v++){
                up[j][v] = up[j-1][ up[j-1][v] ];
                funP[j][v] = COMP[ funP[j-1][ up[j-1][v] ] ][ funP[j-1][v] ];
                funQ[j][v] = COMP[ funQ[j-1][ up[j-1][v] ] ][ funQ[j-1][v] ];
            }
        }
        auto climb = [&](int x, int v, int which){  // 从 x 向上组合到 v(不含 v), 返回函数
            int F = IDENT, cur = x;
            int rem = depth[x] - depth[v];
            for(int j=LOG-1;j>=0;j--){
                if(rem >= (1<<j)){
                    if(which==0) F = COMP[ funP[j][cur] ][ F ];
                    else         F = COMP[ funQ[j][cur] ][ F ];
                    cur = up[j][cur];
                    rem -= (1<<j);
                }
            }
            return F;
        };
        auto getP = [&](int v, int l){
            int x = l;
            int F = climb(x, v, 0);
            return applyf(F, baseP[x]);
        };
        auto getQ = [&](int v, int r){
            int x = r;
            int F = climb(x, v, 1);
            return applyf(F, baseQ[x]);
        };
        auto lca = [&](int a, int b){
            if(depth[a] < depth[b]) swap(a,b);
            int diff = depth[a]-depth[b];
            for(int j=LOG-1;j>=0;j--) if(diff & (1<<j)) a = up[j][a];
            if(a==b) return a;
            for(int j=LOG-1;j>=0;j--) if(up[j][a]!=up[j][b]){ a=up[j][a]; b=up[j][b]; }
            return up[0][a];
        };

        int xorsum = 0;
        for(int q=0;q<m;q++){
            int l,r; cin >> l >> r; --l; --r;
            int k = lca(l,r);
            int lv = (l<k) ? getP(lc[k], l) : -1;
            int rv = (r>k) ? getQ(rc[k], r) : -1;
            xorsum ^= comb(lv, rv);
        }
        cout << (xorsum != 0 ? "Chito" : "Yuuri") << "\n";
    }
    return 0;
}

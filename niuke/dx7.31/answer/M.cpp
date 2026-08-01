#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef vector<int> Poly;

/* ============ NTT (mod 998244353, primitive root 3, ACL-style butterfly) ============ */
const int MOD = 998244353;
const int ROOT = 3;
static int sum_e[30], sum_ie[30];
static bool ntt_ready = false;

int modpow(ll a, ll e) {
    ll r = 1;
    while (e) { if (e & 1) r = r * a % MOD; a = a * a % MOD; e >>= 1; }
    return (int)r;
}

void init_ntt() {
    if (ntt_ready) return;
    ntt_ready = true;
    int cnt2 = __builtin_ctz(MOD - 1);              // 23
    int e = modpow(ROOT, (MOD - 1) >> cnt2);
    int ie = modpow(e, MOD - 2);
    int es[30], ies[30];
    for (int i = cnt2; i >= 2; --i) {
        es[i - 2] = e; ies[i - 2] = ie;
        e = (ll)e * e % MOD; ie = (ll)ie * ie % MOD;
    }
    ll now = 1;
    for (int i = 0; i <= cnt2 - 2; ++i) { sum_e[i] = es[i] * now % MOD; now = now * ies[i] % MOD; }
    now = 1;
    for (int i = 0; i <= cnt2 - 2; ++i) { sum_ie[i] = ies[i] * now % MOD; now = now * es[i] % MOD; }
}

void butterfly(vector<int>& a) {
    int n = (int)a.size();
    int h = __builtin_ctz((unsigned)n);
    for (int ph = 1; ph <= h; ++ph) {
        int w = 1 << (ph - 1), p = 1 << (h - ph);
        ll now = 1;
        for (int s = 0; s < w; ++s) {
            int offset = s << (h - ph + 1);
            for (int i = 0; i < p; ++i) {
                int l = a[i + offset];
                int r = (ll)a[i + offset + p] * now % MOD;
                int x = l + r; if (x >= MOD) x -= MOD;
                int y = l - r; if (y < 0) y += MOD;
                a[i + offset] = x;
                a[i + offset + p] = y;
            }
            now = now * sum_e[__builtin_ctz((unsigned)~s)] % MOD;
        }
    }
}

void butterfly_inv(vector<int>& a) {
    int n = (int)a.size();
    int h = __builtin_ctz((unsigned)n);
    for (int ph = h; ph >= 1; --ph) {
        int w = 1 << (ph - 1), p = 1 << (h - ph);
        ll inow = 1;
        for (int s = 0; s < w; ++s) {
            int offset = s << (h - ph + 1);
            for (int i = 0; i < p; ++i) {
                int l = a[i + offset];
                int r = a[i + offset + p];
                int x = l + r; if (x >= MOD) x -= MOD;
                int y = l - r; if (y < 0) y += MOD;
                a[i + offset] = x;
                a[i + offset + p] = (ll)y * inow % MOD;
            }
            inow = inow * sum_ie[__builtin_ctz((unsigned)~s)] % MOD;
        }
    }
}

/* F2 多项式乘法：输入系数 0/1，输出系数 0/1，截断到次数 <= limit。
   技巧：把 0/1 多项式视为整数多项式，在 F_p (p>N) 中 NTT 相乘得到精确整数系数，
   再逐项 & 1 还原到 F2。因为每步结果都是 0/1，整数乘积系数 <= N+1 < p。 */
Poly mulF2(const Poly& a, const Poly& b, int limit) {
    if (limit < 0 || a.empty() || b.empty()) return {};
    int da = min((int)a.size() - 1, limit);
    int db = min((int)b.size() - 1, limit);
    int resDeg = min(da + db, limit);
    if (resDeg < 0) return {};
    if ((ll)(da + 1) * (db + 1) <= 4096) {           // 小规模朴素
        Poly r(resDeg + 1, 0);
        for (int i = 0; i <= da; ++i) if (a[i]) {
            int mj = min(db, resDeg - i);
            for (int j = 0; j <= mj; ++j) if (b[j]) r[i + j] ^= 1;
        }
        return r;
    }
    int sz = 1, fullDeg = da + db;
    while (sz <= fullDeg) sz <<= 1;                  // 线性卷积所需长度
    init_ntt();
    vector<int> fa(sz, 0), fb(sz, 0);
    for (int i = 0; i <= da; ++i) fa[i] = a[i];
    for (int i = 0; i <= db; ++i) fb[i] = b[i];
    butterfly(fa); butterfly(fb);
    for (int i = 0; i < sz; ++i) fa[i] = (ll)fa[i] * fb[i] % MOD;
    butterfly_inv(fa);
    int iz = modpow(sz, MOD - 2);
    Poly r(resDeg + 1);
    for (int i = 0; i <= resDeg; ++i) r[i] = (ll)fa[i] * iz % MOD & 1;
    return r;
}

/* F2 平方（Frobenius）：(Σ a_i x^i)^2 = Σ a_i x^{2i}，免费 */
Poly squareF2(const Poly& a, int limit) {
    if (limit < 0) return {};
    Poly r(limit + 1, 0);
    for (int i = 0; i < (int)a.size() && 2 * i <= limit; ++i)
        if (a[i]) r[2 * i] = 1;
    return r;
}

/* 偶 / 奇系数抽取：Λ0(A)=[a0,a2,...], Λ1(A)=[a1,a3,...]，截断到次数 limit */
Poly evenExtract(const Poly& a, int limit) {
    if (limit < 0) return {};
    Poly r(limit + 1, 0);
    for (int i = 0; i <= limit && 2 * i < (int)a.size(); ++i) r[i] = a[2 * i];
    return r;
}
Poly oddExtract(const Poly& a, int limit) {
    if (limit < 0) return {};
    Poly r(limit + 1, 0);
    for (int i = 0; i <= limit && 2 * i + 1 < (int)a.size(); ++i) r[i] = a[2 * i + 1];
    return r;
}

/* F2 形式幂级数求逆（常数项=1）。特征2下 Newton 迭代：g <- f * g^2 (mod x^{2m}) */
Poly invF2(const Poly& f, int n) {
    if (n <= 0) return {};
    Poly g(1, 1);
    int m = 1;
    while (m < n) {
        int m2 = min(2 * m, n);
        Poly g2 = squareF2(g, m2 - 1);
        Poly t = mulF2(f, g2, m2 - 1);
        g.assign(t.begin(), t.begin() + m2);
        m = m2;
    }
    return g;
}

/* ============ 对角线系数递归 ============
   s_m = [x^m] A(x) V(x)^m,  V 为平方（故 V(x)^2 = V(x^2)，只需 W = Λ0(V)）。
   递归折半：s_{2m}=[x^m]Λ0(A)V^m, s_{2m+1}=[x^m]Λ1(AV)V^m，
   且 Λ1(AV) = Λ1(A) * Λ0(V)。 */
vector<int> S;   // S[m] = s_m

void diag_rec(const Poly& A, int lo, int hi, int shift, int prefix, const Poly& W) {
    if (lo > hi) return;
    if (hi == 0) {
        if (prefix >= 0 && prefix < (int)S.size())
            S[prefix] = A.empty() ? 0 : A[0];
        return;
    }
    Poly Ae = evenExtract(A, hi / 2);
    diag_rec(Ae, (lo + 1) / 2, hi / 2, shift + 1, prefix, W);

    int olo = lo / 2, ohi = (hi - 1) / 2;
    if (olo <= ohi) {
        Poly Ao = oddExtract(A, ohi);
        Poly B;
        if (W.size() == 1 && W[0] == 1) B = Ao;
        else B = mulF2(Ao, W, ohi);
        diag_rec(B, olo, ohi, shift + 1, prefix + (1 << shift), W);
    }
}

/* ============ 单个测试点 ============ */
string solve_case(ll H, int N, int K) {
    // ---- 计算 Q_h（截断到次数 N）：Q0=1, Q1=1+z, Qh=Q_{h-1}+z^{2^{h-1}}Q_{h-2}
    // 当 2^{h-1} > N 时 Q 稳定，提前终止（H 可到 1e18）
    vector<Poly> Q;
    Q.push_back(Poly(1, 1));                          // Q_0
    if (H - 1 >= 1) {
        Poly q1(N + 1, 0);
        if (N >= 0) q1[0] = 1;
        if (N >= 1) q1[1] = 1;
        while (!q1.empty() && q1.back() == 0) q1.pop_back();
        Q.push_back(q1);                              // Q_1
    }
    for (ll h = 2; h <= H - 1; ++h) {
        if (h - 1 >= 60 || (1LL << (h - 1)) > N) break;
        int sh = 1 << (h - 1);
        Poly qh(N + 1, 0);
        Poly& qm = Q[h - 1];
        for (int i = 0; i < (int)qm.size() && i <= N; ++i) qh[i] = qm[i];
        Poly& qp = Q[h - 2];
        for (int i = 0; i < (int)qp.size() && i + sh <= N; ++i)
            qh[i + sh] ^= qp[i];
        while (!qh.empty() && qh.back() == 0) qh.pop_back();
        Q.push_back(qh);
    }

    Poly QH1 = Q.back();                              // Q_{H-1}
    Poly QH2;                                          // Q_{H-2}
    if (H - 2 <= 0) QH2 = Poly(1, 1);
    else QH2 = Q[(int)min<ll>(H - 2, (ll)Q.size() - 1)];

    // ---- 构造 D,Q,U,V,W,A（均截断到次数 N）
    Poly W1 = invF2(QH1, N + 1);                      // Q_{H-1}^{-1}
    Poly W1sq = squareF2(W1, N);                      // Q_{H-1}^{-2}
    Poly C = mulF2(W1sq, W1, N);                      // C = Q_{H-1}^{-3}
    Poly QH2sq = squareF2(QH2, N);
    Poly QH2qu = squareF2(QH2sq, N);                  // Q_{H-2}^4
    Poly U = mulF2(QH2qu, W1sq, N);                   // U = Q_{H-2}^4/Q_{H-1}^2
    Poly V = invF2(U, N + 1);                         // V = U^{-1}
    Poly W = evenExtract(V, N / 2);                   // W = Λ0(V)

    Poly A = C;                                       // A = C * U^N
    vector<Poly> upow;
    Poly cur = U;
    upow.push_back(cur);
    for (int j = 1; (1LL << j) <= N; ++j) {
        cur = squareF2(cur, N);
        upow.push_back(cur);
    }
    ll rem = N;
    for (int j = 0; rem; ++j, rem >>= 1)
        if (rem & 1) A = mulF2(A, upow[j], N);

    // ---- 对角线系数递归求 s_m, m = N-K+1 .. N
    int lo = N - K + 1, hi = N;
    S.assign(N + 1, 0);
    diag_rec(A, lo, hi, 0, 0, W);

    // ---- 答案：第 k 个字符 = s_{N-k+1}
    string ans;
    ans.reserve(K);
    for (int k = 1; k <= K; ++k)
        ans += (S[N - k + 1] ? '1' : '0');
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        ll H;
        int N, K;
        cin >> H >> N >> K;
        cout << solve_case(H, N, K) << '\n';
    }
    return 0;
}

/* ============================================================================
 * 完整思路整理
 * ============================================================================
 *
 * 【目标】对所有“根权值恰为 H、内部结点数恰为 2^H - 1 + N”的满三叉平面树，
 *         统计蓝点（权值恰为 H 的内部结点）数为 k 的树数对 2 取模，k = 1..K。
 *
 * 一、生成函数（一切在 F2 上；z 的指数 = 内部结点数）
 *     令 B_h 为根权值 <= h 的树。根为叶子贡献 1；根为内部结点时，
 *     “三子权值均 <= h-1”（此时根权值 <= h）或“恰一个子权值 = h、另两子 <= h-1”
 *     （根权值 = h）两类恰好不重不漏。用 E_h = B_h + B_{h-1} 表示“权值恰为 h”，
 *         B_h = 1 + z(B_{h-1}^3 + 3E_h B_{h-1}^2) = 1 + z B_h B_{h-1}^2  （F2 中 3≡1）
 *         即 B_h = 1 / (1 + z B_{h-1}^2)，  E_h = B_h + B_{h-1}。
 *
 *     定义 Q_0 = 1，Q_1 = 1 + z，Q_h = Q_{h-1} + z^{2^{h-1}} Q_{h-2}，可归纳得到闭式
 *         B_h = Q_{h-1}^2 / Q_h，          E_h = z^{2^h - 1} / (Q_h Q_{h-1})。
 *     由 B_h 的递推（即 1/B_h = 1 + z B_{h-1}^2）和 Frobenius 还能得到关键恒等式
 *         Q_h + Q_{h-1}^2 = z Q_{h-2}^4。
 *     注意：E_h 的最小次数是 2^h - 1（对应最小内部结点数），不是 2^{h-1}。
 *     当 2^{h-1} > N 时 z^{2^{h-1}}Q_{h-2} 超出截断，Q 从该层起稳定，
 *     因此 H 即使高达 1e18 也只需递推 O(log N) 层。
 *
 * 二、蓝点成链，F_H 的闭式
 *     一个内部结点若有两个子权值为 H，则其权值为 H+1，所以每个结点至多一个蓝子结点，
 *     所有蓝点沿唯一子结点构成一条从根向下的链。
 *     - 非末端蓝点：1 个蓝子 + 2 个 <= H-1 的子，贡献 z·u·B_{H-1}^2（位置因子 3≡1）；
 *     - 末端蓝点：3 子均 <= H-1 且至少两子权值恰为 H-1，其生成函数
 *       3E_{H-1}^2 B_{H-2} + E_{H-1}^3 = B_{H-1}E_{H-1}^2。
 *     故（u 记蓝点数）
 *         F_H(z,u) = u z B_{H-1}E_{H-1}^2 / (1 + u z B_{H-1}^2)。
 *     [u^k]F_H = z^k E_{H-1}^2 B_{H-1}^{2k-1}。把 B、E 用 Q 代入并提取
 *     z^{2^H-1+N}（最小结点数 2^H-1 提出后只剩 N 个额外结点）：
 *         ansk = [z^{N-k+1}] Q_{H-2}^{4(k-1)} Q_{H-1}^{-(2k+1)}
 *               = [z^N] D^{-3} R^{k-1}，  D = Q_{H-1}，R = z Q_{H-2}^4 / Q_{H-1}^2。
 *
 * 三、特征 2 的结构 → 转成对角线系数
 *     R = z·U，U = B_{H-1}^2 = Q_{H-2}^4 / Q_{H-1}^2 是平方，故 U 只有偶次项、U'=0。
 *     令 m = N - k + 1，由 R = zU 得
 *         ansk = [z^m] C·U^{N-m}，  C = Q_{H-1}^{-3} = D^{-3}。
 *     把它写成标准对角形式（m 出现在指数里）：
 *         A = C·U^N，  V = U^{-1}（V 仍是平方，故 V(x)^2 = V(x^2)），
 *         ansk = s_m = [x^m] A(x)·V(x)^m。
 *
 * 四、对角线系数的递归折半
 *     s_m = [x^m] A V^m。利用 V(x)^2 = V(x^2) 与偶/奇抽取算子
 *         Λ0(Σa_i x^i) = Σ a_{2i} x^i， Λ1(Σa_i x^i) = Σ a_{2i+1} x^i：
 *         s_{2m}   = [x^m] Λ0(A)·V^m，
 *         s_{2m+1} = [x^m] Λ1(AV)·V^m。
 *     又 V 只有偶次项，Λ1(AV) = Λ1(A) * Λ0(V)，所以只需预计算 W = Λ0(V)。
 *     于是对区间 [lo, hi] 递归（shift/prefix 记录全局 m 的二进制前缀）：
 *         - 偶数支：新多项式 Λ0(A)，新区间 [(lo+1)/2, hi/2]；
 *         - 奇数支：新多项式 B = Λ1(A)*W，新区间 [lo/2, (hi-1)/2]；
 *         - 终止：hi = 0 时 s = A[0]，存入 S[全局m]。
 *     每层多项式规模折半，区间同步折半，总代价 O(M(N) log N)。
 *
 * 五、F2 上的多项式乘法
 *     全程系数都是 0/1。乘法时把 0/1 多项式当作整数多项式，在 998244353 下做 NTT：
 *     由于每步都已归约回 F2，两个 0/1 多项式乘积的整数系数不超过 N+1 < p，
 *     NTT 恰好给出精确整数系数，再逐项 & 1 即还原 F2 乘积。
 *     平方用 Frobenius 免费得到；求逆用特征 2 下的 Newton 迭代
 *         g <- f · g^2 (mod x^{2m})（每步一次乘法，总代价 O(M(N))）。
 *     A = C·U^N 用快速幂：U^{2^j} 全由免费平方得到，只需 popcount(N) 次乘法。
 *
 * 【复杂度】时间 O(M(N) log N)，其中 M(N) 为 NTT 乘法开销（N <= 5e5 单组约 1.5s）；
 *           空间 O(N)。满足 5s 时限与 sum N <= 5e5 的约束。
 * ========================================================================== */


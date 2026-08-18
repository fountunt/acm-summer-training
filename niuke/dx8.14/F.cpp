#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 每个块对应网格中一个正方形区域
struct Block {
    int sign;      // 0 叶, +1 直接和(+), -1 斜和(-)
    int lo, hi;    // 值域 [lo, hi]
    int R0, C0, H; // 正方形左上角 (R0,C0) 与边长 H
    ll f, A, B, C, D;           // 五元组
    ll dmf, dmA, dmB, dmC, dmD; // 直接子块最大值(用于展平连续同号)
};

ll max3(ll a, ll b, ll c) { return max(a, max(b, c)); }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> p(n + 1);
    for (int i = 1; i <= n; ++i) cin >> p[i];

    vector<Block> st;
    for (int i = 1; i <= n; ++i) {
        Block b;
        b.sign = 0;
        b.lo = b.hi = p[i];
        b.R0 = i; b.C0 = p[i]; b.H = 1;
        b.f = 0;
        b.A = (ll)i - p[i];
        b.B = (ll)p[i] - i;
        b.C = (ll)i + p[i];
        b.D = -(ll)(i + p[i]);
        b.dmf = 0;
        b.dmA = b.A; b.dmB = b.B; b.dmC = b.C; b.dmD = b.D;
        st.push_back(b);

        while (st.size() >= 2) {
            Block P = st[st.size() - 2];
            Block Q = st[st.size() - 1];
            int s;
            if (P.hi + 1 == Q.lo) s = 1;        // 值域递增 -> +
            else if (Q.hi + 1 == P.lo) s = -1;  // 值域递减 -> -
            else break;

            // 连续同号展平: 同号子块只贡献其直接子块最大值
            auto ctf = [&](const Block& X, ll& f, ll& A, ll& B, ll& C, ll& D) {
                f = (X.sign == s) ? X.dmf : X.f;
                A = (X.sign == s) ? X.dmA : X.A;
                B = (X.sign == s) ? X.dmB : X.B;
                C = (X.sign == s) ? X.dmC : X.C;
                D = (X.sign == s) ? X.dmD : X.D;
            };
            ll P_f, P_A, P_B, P_C, P_D, Q_f, Q_A, Q_B, Q_C, Q_D;
            ctf(P, P_f, P_A, P_B, P_C, P_D);
            ctf(Q, Q_f, Q_A, Q_B, Q_C, Q_D);

            ll mf = max(P_f, Q_f), mA = max(P_A, Q_A), mB = max(P_B, Q_B),
                mC = max(P_C, Q_C), mD = max(P_D, Q_D);

            Block N;
            N.sign = s;
            N.R0 = P.R0;
            N.H = P.H + Q.H;
            if (s == 1) {
                N.lo = P.lo; N.hi = Q.hi;
                N.C0 = P.C0;
                ll d1 = (ll)(N.H - 1) + N.C0 - N.R0;  // 对角线上方 y-x 最大值
                ll d2 = (ll)(N.H - 1) + N.R0 - N.C0;  // 对角线下方 x-y 最大值
                N.f = max3(mf, mA + d1, mB + d2);
                N.A = max(mA, 2 * d2 + mB);
                N.B = max(mB, 2 * d1 + mA);
                N.C = max3(mC, 2 * (N.C0 + N.H - 1) + mA, 2 * (N.R0 + N.H - 1) + mB);
                N.D = max3(mD, mA - 2 * N.R0, mB - 2 * N.C0);
            } else {
                N.lo = Q.lo; N.hi = P.hi;
                N.C0 = Q.C0;
                ll base = N.R0 + N.C0;
                N.f = max3(mf, mC - base, base + 2LL * (N.H - 1) + mD);
                N.A = max3(mA, mC - 2LL * N.C0, 2LL * (N.R0 + N.H - 1) + mD);
                N.B = max3(mB, mC - 2LL * N.R0, 2LL * (N.C0 + N.H - 1) + mD);
                N.C = max(mC, 2LL * (base + 2LL * (N.H - 1)) + mD);
                N.D = max(mD, mC - 2LL * base);
            }
            N.dmf = mf;
            N.dmA = mA; N.dmB = mB; N.dmC = mC; N.dmD = mD;

            st.pop_back();
            st.pop_back();
            st.push_back(N);
        }
    }

    if (st.size() != 1) cout << -1 << "\n";
    else cout << st[0].f << "\n";
    return 0;
}

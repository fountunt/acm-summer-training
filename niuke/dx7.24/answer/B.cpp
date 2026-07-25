//m < n 0
//m = n (1-a)/b)^m
//m > n (m-n)%c != 0 0
//m > n (m-n)%c == 0 中奖(m-n)/c次
#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
const int MOD = 998244353;
const int MAXM = 2000000; // 根据题目上限

// 快速幂
int64 modpow(int64 a, int64 e) {
    int64 r = 1;
    while (e > 0) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. 预处理阶乘、逆阶乘、逆元
    vector<int64> fact(MAXM + 1), invfact(MAXM + 1), inv(MAXM + 1);
    fact[0] = 1;
    for (int i = 1; i <= MAXM; i++) fact[i] = fact[i-1] * i % MOD;

    invfact[MAXM] = modpow(fact[MAXM], MOD - 2);
    for (int i = MAXM; i >= 1; i--) invfact[i-1] = invfact[i] * i % MOD;

    inv[1] = 1;
    for (int i = 2; i <= MAXM; i++)
        inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;

    int T;
    cin >> T;
    while (T--)
    {
        int n, m, c, a, b;
        cin >> n >> m >> c >> a >> b;

        // 2. 不合法判定
        if (m < n || (m - n) % c != 0) {
            cout << 0 << '\n';
            continue;
        }

        int K = (m - n) / c; // 中奖次数
        int L = m - K;       // 未中奖次数

        // 3. 计算组合数 C(m, K)
        int64 comb = fact[m] * invfact[K] % MOD * invfact[L] % MOD;

        // 4. 计算概率因子
        int64 inv_b = modpow(b, MOD - 2);
        int64 p = (int64)a * inv_b % MOD;          // a/b
        int64 q = (int64)(b - a) * inv_b % MOD;   // (b-a)/b

        // 5. 组装答案
        int64 ans = comb;
        ans = ans * modpow(p, K) % MOD;
        ans = ans * modpow(q, L) % MOD;
        ans = ans * n % MOD;
        ans = ans * inv[m] % MOD; // 乘 n/m

        cout << ans << '\n';
    }
    return 0;
}
//Ans = 和法顺序 * 每个顺序的概率
//合法顺序 组合数表达出总的中奖顺序*合法起点的概率修正
//合法起点n块本金那么就有n个合法起点 概率即为n/m

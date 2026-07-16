#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1000000007;
using ll = long long;
using int128 = __int128;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    while (T--)
    {
        int n;
        ll k;
        cin >> n >> k;
        vector<int> a(n);
        for (int i = 0; i < n; ++i)
            cin >> a[i];

        vector<int> Q = {1};
        for (int x : a)
        {
            vector<int> nxt(Q.size() + x, 0);
            for (int i = 0; i < (int)Q.size(); ++i)
            {
                if (Q[i] == 0)
                    continue;
                nxt[i] = (nxt[i] + Q[i]) % MOD;
                nxt[i + x] = (nxt[i + x] - Q[i] + MOD) % MOD;
            }
            Q = move(nxt);
        }
        int D = (int)Q.size() - 1;
        vector<int> P = {1};

        while (k > 0)
        {
            if (P.empty())
                break;

            vector<int> Q_minus(D + 1);
            for (int i = 0; i <= D; ++i)
                Q_minus[i] = (i & 1) ? (MOD - Q[i]) % MOD : Q[i];

            int dP = (int)P.size() - 1;
            vector<int128> newP(dP + D + 1, 0);
            for (int i = 0; i <= dP; ++i)
            {
                int pi = P[i];
                if (pi == 0)
                    continue;
                for (int j = 0; j <= D; ++j)
                    newP[i + j] += (int128)pi * Q_minus[j];
            }

            vector<int> nP;
            if (k & 1)
            {
                for (size_t i = 1; i < newP.size(); i += 2)
                    nP.push_back((int)(newP[i] % MOD));
            }
            else
            {
                for (size_t i = 0; i < newP.size(); i += 2)
                    nP.push_back((int)(newP[i] % MOD));
            }
            P = move(nP);

            vector<int128> newQ(2 * D + 1, 0);
            for (int i = 0; i <= D; ++i)
            {
                int qi = Q[i];
                if (qi == 0)
                    continue;
                for (int j = 0; j <= D; ++j)
                    newQ[i + j] += (int128)qi * Q_minus[j];
            }
            for (int i = 0; i <= D; ++i)
                Q[i] = (int)(newQ[2 * i] % MOD);

            k >>= 1;
        }

        int ans = P.empty() ? 0 : (P[0] % MOD);
        cout << ans << '\n';
    }
    return 0;
}

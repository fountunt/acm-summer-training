#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 998244353;

#define Matrix vector<vector<ll>>

Matrix mul(const Matrix &a, const Matrix &b) {
    ll n = a.size(), p = b.size(), m = b[0].size();
    Matrix res(n, vector<ll>(m, 0));
    for (ll i = 0; i < n; i++) {
        for (ll k = 0; k < p; k++) {
            if (!a[i][k]) continue;
            for (ll j = 0; j < m; j++) {
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return res;
}

Matrix matrix_pow(Matrix a, ll k)
{
    ll n = a.size();
    Matrix res(n, vector<ll>(n, 0));
    for (ll i = 0; i < n; i++) res[i][i] = 1;

    while (k) {
        if (k & 1) res = mul(res, a);
        a = mul(a, a);
        k >>= 1;
    }
    return res;
}

int n;
vector<vector<int>> states;

void dfs(int pos,vector<int>& cur)
{
    if(pos == n)
    {
        states.push_back(cur);
        return;
    }
    for(int i = 0;i <= 2;i++)
    {
        if(pos > 0 && i == cur[pos-1]) continue;
        cur.push_back(i);
        dfs(pos+1,cur);
        cur.pop_back();
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> n >> m;
    vector<int> temp;
    dfs(0,temp);

    int S = states.size();
    Matrix trans(S,vector<ll>(S,0));
    for(int u = 0;u < S;u++)
    {
        auto &col_u = states[u];
        for(int v = 0;v < S;v++)
        {
            auto &col_v = states[v];
            bool ok = true;
            for(int i = 0;i < n;i++)
            {
                if(col_u[i] == col_v[i])
                {
                    ok = false;
                    break;
                }
            }
            if(ok) trans[u][v] = 1;
        }
    }

    Matrix a = matrix_pow(trans,m-1);
    ll ans = 0;
    for(int i = 0;i < S;i++)
    {
        for(int j = 0;j < S;j++)
        {
            ans += a[i][j];
            ans %= MOD;
        }
    }
    cout << ans << "\n";
}

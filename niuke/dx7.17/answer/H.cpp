#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using array3 = array<int, 3>;

const int UP = 1000;
int id[4][4], tot;
array3 state[22];
double f[UP + 10][11][11];

void work()
{
    int b;
    cin >> b;
    vector<int> a;
    for (int i = 1; i <= 2; ++i)
    {
        string str;
        cin >> str;
        int cntR = count(str.begin(), str.end(), 'R');
        int cntS = count(str.begin(), str.end(), 'S');
        a.push_back(id[cntR][cntS]);
    }
    double ans;
    if (b <= UP) ans = f[b][a[0]][a[1]];
    else ans = f[UP][a[0]][a[1]] + (b - UP) * (f[UP][a[0]][a[1]] - f[UP - 1][a[0]][a[1]]);
    cout << setprecision(10) << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 预处理所有状态 (三元组 (R,S,O) 和为3)
    for (int i = 0; i <= 3; ++i)
    {
        for (int j = 0; j <= 3 - i; ++j)
        {
            id[i][j] = ++tot;
            state[tot] = {i, j, 3 - i - j};
        }
    }

    // DP 递推
    for (int T = 1; T <= UP; ++T)
    {
        for (int x = 1; x <= 10; ++x)
        {
            for (int y = 1; y <= 10; ++y)
            {
                array3 a = state[x], b = state[y];
                double best = -1e18;
                for (int i = 0; i < 3; ++i) if (a[i])
                {
                    --a[i];
                    double worst = 1e18;
                    for (int j = 0; j < 3; ++j) if (b[j])
                    {
                        --b[j];
                        double s = 0;
                        if (i == j) s = 1;
                        else if ((i + 1) % 3 == j) s = 3;
                        // 随机恢复一个资源 (9种等概率组合)
                        for (int u = 0; u < 3; ++u)
                        {
                            for (int v = 0; v < 3; ++v)
                            {
                                ++a[u];
                                ++b[v];
                                s += f[T - 1][id[a[0]][a[1]]][id[b[0]][b[1]]] / 9.0;
                                --a[u];
                                --b[v];
                            }
                        }
                        worst = min(worst, s);
                        ++b[j];
                    }
                    best = max(best, worst);
                    ++a[i];
                }
                f[T][x][y] = best;
            }
        }
    }

    int T;
    cin >> T;
    while (T--) work();
    return 0;
}

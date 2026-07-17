#include<bits/stdc++.h>
using namespace std;
using ll = long long;

int trans[10][3][3];
map<vector<int>, int> hand_to_id;
vector<vector<int>> id_to_hand(10);
long double dp[5005][10][10];

// 计分函数：0-R, 1-S, 2-P
int get_score(int a, int b) {
    if (a == b) return 1;
    if ((a == 0 && b == 1) || (a == 1 && b == 2) || (a == 2 && b == 0)) return 3;
    return 0;
}

// 预处理所有状态和 DP 值
void precompute() {
    int cnt = 0;
    // 生成所有 10 种手牌组合
    for (int i = 0; i <= 2; ++i) {
        for (int j = i; j <= 2; ++j) {
            for (int k = j; k <= 2; ++k) {
                vector<int> h = {i, j, k};
                hand_to_id[h] = cnt;
                id_to_hand[cnt] = h;
                cnt++;
            }
        }
    }

    // 预处理打牌和抽牌后的状态转移
    for (int u = 0; u < 10; ++u) {
        for (int c = 0; c < 3; ++c) {
            for (int n = 0; n < 3; ++n) {
                vector<int> h = id_to_hand[u];
                bool found = false;
                for (int i = 0; i < 3; ++i) {
                    if (h[i] == c) {
                        h.erase(h.begin() + i);
                        found = true;
                        break;
                    }
                }
                if (found) {
                    h.push_back(n);
                    sort(h.begin(), h.end());
                    trans[u][c][n] = hand_to_id[h];
                } else {
                    trans[u][c][n] = -1; // 手牌中没有卡牌 c
                }
            }
        }
    }

    // 进行马尔可夫决策过程 (MDP) 迭代
    for (int k = 1; k <= 5000; ++k) {
        for (int u = 0; u < 10; ++u) {
            for (int v = 0; v < 10; ++v) {
                long double max_A = -1.0;
                for (int cA = 0; cA < 3; ++cA) {
                    if (trans[u][cA][0] == -1) continue; // Alice 没有 cA
                    long double min_B = 1e18;
                    for (int cB = 0; cB < 3; ++cB) {
                        if (trans[v][cB][0] == -1) continue; // Bob 没有 cB

                        long double expected_future = 0.0;
                        for (int nA = 0; nA < 3; ++nA) {
                            int nu = trans[u][cA][nA];
                            for (int nB = 0; nB < 3; ++nB) {
                                int nv = trans[v][cB][nB];
                                expected_future += dp[k - 1][nu][nv];
                            }
                        }
                        expected_future /= 9.0;

                        long double current_score = get_score(cA, cB) + expected_future;
                        if (current_score < min_B) {
                            min_B = current_score; // Bob 尽量最小化得分
                        }
                    }
                    if (min_B > max_A) {
                        max_A = min_B; // Alice 尽量最大化最小得分
                    }
                }
                dp[k][u][v] = max_A;
            }
        }
    }
}

void solve() {
    long long k;
    string sA, sB;
    cin >> k >> sA >> sB;

    auto get_id = [&](string s) {
        vector<int> h(3);
        for (int i = 0; i < 3; ++i) {
            if (s[i] == 'R') h[i] = 0;
            else if (s[i] == 'S') h[i] = 1;
            else if (s[i] == 'P') h[i] = 2;
        }
        sort(h.begin(), h.end());
        return hand_to_id[h];
    };

    int u = get_id(sA);
    int v = get_id(sB);

    // 对于足够大的 k，利用 MDP 的极限线性增量来直接进行常数推断 O(1)
    if (k <= 5000) {
        cout << fixed << setprecision(12) << dp[k][u][v] << "\n";
    } else {
        long double g = dp[5000][u][v] - dp[4999][u][v];
        long double ans = dp[5000][u][v] + (long double)(k - 5000) * g;
        cout << fixed << setprecision(12) << ans << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    precompute();

    int t;
    if (cin >> t) {
        while (t--) solve();
    }

    return 0;
}

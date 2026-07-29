//暴力解法：
//遍历s 对每一个s使其前缀多一个a 排序后比较大小
//选取最小的作为解
//复杂度 O(Tn3lgn)
//对于每个k 修改是独立的
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef vector <int> vi;
typedef pair <int, int> pii;
typedef vector <pii> vp;

const int MAX_N = 1 << 20;
const int INF32 = 0x3f3f3f3f;

void solve() {
    int n, m = 0, q = 0;
    cin >> n;
    vector <string> a(n);
    for (int i = 0; i < n; i ++) {
        cin >> a[i];
        for (auto &ch : a[i]) {
            if (ch != 'a') m ++;
        }
        q += a[i].size();
    }
    sort(a.begin(), a.end(), [&](const string &a, const string &b) -> bool {return a + b < b + a;});
    vi lena(m + 1); vector <string> f(m + 1), ans(m + 1);
    auto getAns = [&](int a, const string &b) -> string {
        return string(a, 'a') + b;
    };
    for (auto &s : a) {
        int cnta = 0;
        for (auto &ch : s) {
            if (ch != 'a') cnta ++;
        }
        vi tmpL = lena;
        vector <string> tmpF(m + 1, "{"), tmpA(m + 1, "{");
        for (int i = 0; i <= m; i ++) tmpF[i] = f[i] + s;
        for (int i = 0; i <= m; i ++) {
            if (i + cnta <= m) {
                tmpA[i + cnta] = getAns(s.size(), ans[i]);
                if (tmpL[i + cnta] < lena[i] + s.size() || tmpL[i + cnta] == lena[i] + s.size() && f[i] < tmpF[i + cnta]) {
                    tmpL[i + cnta] = lena[i] + s.size(); tmpF[i + cnta] = f[i];
                }
            }
            if (i >= cnta) tmpA[i] = min(tmpA[i], ans[i] + s);
            else tmpA[i] = ans[i] + s;
        }
        for (int i = 0; i <= m; i ++) {
            for (int j = 0, k = 0; k <= cnta && i + k <= m; j ++, k ++) {
                while (j < (int)s.size() && s[j] == 'a') j ++;
                tmpA[i + k] = min(tmpA[i + k], getAns(lena[i] + j, j < s.size() ? s.substr(j, s.size() - j) : "") + f[i]);
            }
        }
        lena.swap(tmpL);
        f.swap(tmpF);
        ans.swap(tmpA);
    }
    for (int i = 0; i <= q; i ++) cout << ans[min(i, m)] << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int T;
    cin >> T;
    while (T --) solve();

    return 0;
}

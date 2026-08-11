// ============================================================
// 暴力程序 bf.cpp —— 只求正确,不求效率
// 读与 std 相同的输入,输出相同格式的答案,作为"标准答案"
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1,s2;
    cin >> s1 >> s2;
    int n1 = s1.size(),n2 = s2.size();

    // 单字符:前缀/后缀无法同时满足非空重叠,与 std 一致输出 -1
    if (n1 == 1 || n2 == 1) {
        cout << -1 << "\n";
        return 0;
    }

    int l = -1, r = -1;
    for (int i = 1; i < n1; i++) {
        for (int j = 0; j < n2 - 1; j++) {
            if (s1[i] == s2[j] && (l == -1 || i + n2 - j < l + n2 - r)) {
                l = i;
                r = j;
            }
        }
    }

    // 无重叠解 → -1,防止 r=-1 时 s2[-1] 越界
    if (l == -1) {
        cout << -1 << "\n";
        return 0;
    }

    for (int i = 1; i < l; i++) cout << s1[i];
    for (int j = r; j < n2; j++) cout << s2[j];
    cout << "\n";

    return 0;
}

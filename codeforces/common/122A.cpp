#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;

    // 生成所有 ≤ 1000 的幸运数
    vector<int> lucky = {4, 7, 44, 47, 74, 77, 444, 447, 474, 477, 744, 747, 774, 777};

    for (int x : lucky) {
        if (n % x == 0) {
            cout << "YES" << endl;
            return 0;
        }
    }

    cout << "NO" << endl;
    return 0;
}

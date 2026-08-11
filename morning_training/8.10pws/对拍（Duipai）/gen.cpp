// ============================================================
// 数据生成器 gen.cpp —— 随机造一组测试数据,写到 stdout
// 对拍时由 duipai.sh 每次调用,生成器输出即本轮输入 data.in
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 想复现某组出错数据时,把种子固定成常数(如 mt19937 rng(12345))
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    auto rnd = [&](int l, int r) {              // [l, r] 闭区间随机
        return uniform_int_distribution<int>(l, r)(rng);
    };

    // 数据要小,保证暴力 bf 能跑完;同时覆盖边界:
    // 比如 n=1,全 0,递增/递减序列,与主数据交替出现
    int n = rnd(1, 10);
    int m = rnd(1,10);
    string s1;
    string s2;
    for(int i = 0;i < n;i++){
        int x1 = rnd(0,25);
        int x2 = rnd(0,25);
        s1 += (char)('a' + x1);
        s2 += (char)('a' + x2);
    }

    cout << s1 << "\n" << s2 << "\n";

    return 0;
}

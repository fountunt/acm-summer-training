//O(n)
//预处理左右的f(1,i,ai) ,f(j,n,aj) map哈希查找 O(n);
//线段树维护f()以f()的大小为比较值 维护区间内比f()小的f()数量
//查询(i,n) 比f(j)小的对数O(lgn)
//遍历O(n)
//总复杂度O(nlgn)

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 1e6 + 5;
int a[MAXN],L[MAXN],R[MAXN];

struct Fenwick{
    int n;
    vector<int> bit;
    Fenwick(int n) : n(n),bit(n+1,0) {}
    void add(int idx,int val){
        for(;idx <= n;idx += idx & -idx)
            bit[idx] += val;
    }
    int sum(int idx){
        int res = 0;
        for(;idx > 0;idx -= idx & -idx)
            res += bit[idx];
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    // 离散化
    vector<int> vals(a + 1, a + n + 1);
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int m = vals.size();

    // 计算 L
    vector<int> cnt(m + 1, 0);
    for (int i = 1; i <= n; i++) {
        int id = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin() + 1;
        cnt[id]++;
        L[i] = cnt[id];
    }

    // 计算 R
    fill(cnt.begin(), cnt.end(), 0);
    for (int i = n; i >= 1; i--) {
        int id = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin() + 1;
        cnt[id]++;
        R[i] = cnt[id];
    }

    // 扫描统计
    Fenwick bit(n);
    long long ans = 0;
    int total = 0;

    for (int j = 1; j <= n; j++) {
        int x = R[j];
        int lessEq = bit.sum(x);          // L[i] <= x 的个数
        ans += total - lessEq;            // L[i] > x 的个数
        bit.add(L[j], 1);
        total++;
    }

    cout << ans << '\n';
    return 0;
}

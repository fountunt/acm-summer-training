#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DSU
{
    vector<int> fa,sz;
    DSU(int n)
    {
        fa.resize(n);
        sz.resize(n,1);
        for(int i = 0;i < n;i++) fa[i] = i;
    }

    int find(int x) {return fa[x] == x ? x : fa[x] = find(fa[x]);}

    bool unite(int x,int y)
    {
        x = find(x),y = find(y);
        if(x == y) return false;
        if(sz[x] < sz[y]) swap(x,y);
        fa[y] = x;
        sz[x] += sz[y];
        return true;
    }
};

void solve()
{
    int n,p;
    cin >> n >> p;
    vector<int> a(n);
    for(int i = 0;i < n;i++) cin >> a[i];

    vector<int> ord(n);
    iota(ord.begin(),ord.end(),0);
    sort(ord.begin(),ord.end(),[&](int i,int j){
        return a[i] < a[j];
    });//给边排序

    DSU dsu(n);
    vector<bool> used(n,false);
    ll ans = 0;

    for(int i : ord)
    {
        if(a[i] >= p) break;//比p大直接跳过
        if(used[i]) continue;//用过的直接跳过

        int j = i = 1;
        while(j < n && a[j] % a[i] == 0)
        {
            if(dsu.unite(i,j)) ans += a[i];//先处理小的 从小到大加边
            if(used[j]) break;             //那么已经连接过的必然比后面的小 所以直接跳过
            used[j] = true;//连接两边 用掉节点
            j++;
        }

        j = i - 1;//i,j都是对于数组来说的 也就是没有排序的状况进行遍历
        while(j >= 0 && a[j] % a[i] == 0)
        {
            if(dsu.unite(i,j)) ans += a[i];
            if(used[j]) break;
            used[j] = true;
            j--;
        }//两个朝向

        used[i] = true;
    }

    for(int i = 1;i < n;i++)
    {
        if(dsu.unite(i-1,i)) ans += p;
    }

    cout << ans << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while(t--) solve();
    return 0;
}

//题目已经给了基础树
//通过公式来寻找更短边来替换基础的边
//公式等价于 区间最小值能整除区间内所有元素

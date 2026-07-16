#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 1e6 + 5;
int t[N*2],nxt[N*2],cnt,h[N];
int n,d;
void add(int x,int y)
{
    t[++cnt] = y;//第cnt条边指向y节点
    nxt[cnt] = h[x];//这条新边指向上一条x为起点的边
    h[x] = cnt;//更新现在以x为起点的最后一条边
}//链式前向星加边

int a[N],b[N];
bool f[2][N];
void dfs1(int x,int fa,int dis)
{
    a[dis] = x;//缓存当前路径
    if(dis > d) b[x] = a[dis - d];//记录d级祖先
    else b[x] = 1;

    for(int i = h[x];i;i = nxt[i])
    {
        if(t[i] == fa) continue;
        dfs1(t[i],x,dis + 1);
    }//递归子节点
}

void dfs2(int x,int fa,int tp)
{
    bool u = 0;
    for(int i = h[x] ;i;i = nxt[i])
    {
        if(t[i] == fa) continue;
        dfs2(t[i],x,tp);
        u |= f[tp][t[i]];
    }
    f[tp][x] |= u;
}//需求扩散，自下向上递归

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> d;
    for(int i = 1;i < n;i++)
    {
        int x,y;
        cin >> x >> y;
        add(x,y),add(y,x);
    }
    dfs1(1,0,1);
    for(int i = 0;i <= 1;i++)
    {
        int num;
        cin >> num;
        for(int j = 1;j <= num;j++)
        {
            int x;
            cin >> x;
            f[i][x] = 1,f[i ^ 1][b[x]] = 1;
        }
    }

    for(int i = 0;i <= 1;i++) dfs2(1,0,i);
    int ans = 0;
    for(int i = 0;i <= 1;i++)
    {
        for(int j = 2;j <= n;j++)
        {
            if(f[i][j]) ans += 2;
        }
    }//统计答案，有需求的路都走
    cout << ans;
    return 0;
}


//本题和前几天写的很像
//从上往下递归初始化信息
//从下往上回传信息
//最后遍历统计
//不过本题用到了链式前向星 用于优化遍历树的数据结构

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 2.5e6 + 5;
int n,m;
vector<int> pos[N];
int fa[N];
int val[N];
int sz[N];
int mx[N];

int dirs[4][2] = {{1,0},{-1,0},{0,-1},{0,1}};

int find(int x)
{
    if(x == fa[x]) return x;
    int p = find(fa[x]);
    mx[x] = max(mx[x],mx[fa[x]]);
    return fa[x] = p;
}

void unite(int a,int b)
{//更新最大值mx
 //更新连通块大小sz
    b = find(b);//找到父节点 把新节点连进去
    if(b == a) return;

    fa[b] = a;
    //mx[b] = max(mx[b],val[a] - sz[b]);//更新最大值
    mx[b] = max(mx[b],val[a] - sz[b]);
    sz[a] += sz[b];
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> n >> m >> q;
	for(int i = 0; i <= n + 1; i++)
		pos[i].resize(m + 2);

    ll l = 0;
    int x,y,v,c = 0;
    while(q--)
    {
        int op;cin >> op;
        if(op & 1)
        {
            //把点读入并查集 二维到一维映射
            //并查集查找联通块的大小 dirs[] 四方查找
            //更新连通块 大小+1
            cin >> x >> y >> v;
            x ^= l;y ^= l;
            //初始化
            pos[x][y] = ++c;
            fa[c] = c;
            sz[c] = 1;
            val[c] = v;
            for(auto [d1,d2] : dirs)
            {
                int nx = x + d1,ny = y + d2;
                if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
                if(pos[nx][ny]) unite(c,pos[nx][ny]);
            }
            l = sz[c]-1;
            cout << l << "\n";
        }
        else
        {
            //并查集查找最大的子块
            //ans 就是 最大的子块和吃鱼时候增长的做差
            cin >> x >> y;
            x ^= l,y ^= l;
            find(pos[x][y]);
            l = max(mx[pos[x][y]] + 1 - val[pos[x][y]],0);
            cout << l << "\n";
        }
    }
}

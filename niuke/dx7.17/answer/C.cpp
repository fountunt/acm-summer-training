#include <bits/stdc++.h>

using namespace std;

const int N = 2.5e5 + 5, inf = 1e9;
int fa[N], mx[N];
vector<int> pos[N];
int val[N], sz[N];
int getf(int x)
{
	if(fa[x] == x)
		return x;
	int p = getf(fa[x]);//并查集递归查找
	mx[x] = max(mx[x], mx[fa[x]]);//找到最大的点
	return fa[x] = p;
}
void add(int x, int y)
{
	x = getf(x);
	if(x == y)
		return ;
	fa[x] = y;
	mx[x] = max(mx[x], val[y] - sz[x]);//吃了鱼后的最大大小
	sz[y] += sz[x];//连通块加法
}
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

int main()
{
	int n, m, q, i, j, t, x, y, v, c = 0, ans = 0;
	scanf("%d%d%d", &n, &m, &q);
	for(i = 0; i <= n + 1; i++)
		pos[i].resize(m + 2);
	while(q--)
	{
		scanf("%d%d%d", &t, &x, &y);
		x ^= ans;
	    y ^= ans;//解密
		if(t == 1)
		{//读入数据
			scanf("%d", &v);
			pos[x][y] = ++c;//坐标转化为并查集idx
			val[c] = v;//记录大小
			fa[c] = c;//并查集初始化
            sz[c] = 1;//初始连通块大小为1
			for(i = 0; i < 4; i++)
				if(pos[x + dx[i]][y + dy[i]])
					add(pos[x + dx[i]][y + dy[i]], c);//搜索四周 加入连通块
			printf("%d\n", ans = sz[c] - 1);
		}
		else
		{
			getf(pos[x][y]);
			printf("%d\n", ans = max(mx[pos[x][y]] + 1 - val[pos[x][y]], 0));//取出最大值
		}
	}
	return 0;
}
//把并查集融入到二维坐标中

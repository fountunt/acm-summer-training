//读入数据
//整理二维数组 将相同数的下标放到记录为连续数组
//start[x] = p -> pos[p] pos[p] p映射x，y下标 类哈希表结构
//二维差分处理每个数
//还原 输出

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,m;
    cin >> n >> m;

    int N = n * m;
    vector<int> val(N);
    vector<int> cnt(N+1,0);

    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < m;j++)
        {
            int x;
            cin >> x;
            val[i * m + j] = x;
            cnt[x]++;
        }
    }

    vector<int> start(N+2,0);
    for(int x = 1;x <= N;x++)
    {
        start[x + 1] = start[x] + cnt[x];
    }

    vector<int> cur = start;
    vector<int> posR(N),posC(N);

    for(int idx = 0;idx < N;idx++)
    {
        int x = val[idx];
        int p = cur[x]++;
        posR[p] = idx / m;
        posC[p] = idx % m;
    }

    vector<int>().swap(val);
    vector<int>().swap(cnt);
    vector<int>().swap(cur);

    int W = m + 2;
    vector<int> diff((n + 2) * W,0);

    auto addRect = [&](int r1,int c1,int r2,int c2)
    {
        diff[r1 * W + c1]++;
        diff[(r2 + 1) * W + c1]--;
        diff[r1 * W + c1 + 1]--;
        diff[(r2 + 1) * W + c1 + 1]++;
    };

    for(int x = 1;x <= N;x++)
    {
        int l = start[x];
        int r = start[x + 1];
        int k = r - l;
        if(k < 2) continue;

        vector<int> rows;
        vector<int> mn,mx;
        rows.reserve(k);
        mn.reserve(k);
        mx.reserve(k);

        int p = l;
        while(p < r)
        {
            int row = posR[p];
            int minc = posC[p],maxc = posC[p];

            while(p < r && posR[p] == row)
            {
                minc = min(minc,posC[p]);
                maxc = max(maxc,posC[p]);
                p++;
            }

            rows.push_back(row);
            mn.push_back(minc);
            mx.push_back(maxc);
        }

        int K = (int)rows.size();
        if(K < 2) continue;

        vector<int> suffMax(K);
        suffMax[K-1] = mx[K-1];
        for(int i = K - 2;i >= 0;i--)
        {
            suffMax[i] = max(mx[i],suffMax[i + 1]);
        }

        int prefMin = mn[0];
        for(int t = 1;t < k;t++)
        {
            int L = prefMin;
            int R = suffMax[t];

            if(L < R)
            {
                int r1 = rows[t - 1];
                int r2 = rows[t];
                addRect(r1,L,r2,R);
            }

            prefMin = min(prefMin,mn[t]);
        }
    }

    vector<int>().swap(posR);
    vector<int>().swap(posC);
    vector<int>().swap(start);

    for(int i = 0;i < n + 2;i++)
    {
        for(int j = 0;j < m + 2;j++)
        {
            int id = i * W + j;
            if(i > 0) diff[id] += diff[(i-1)*W+j];
            if(j > 0) diff[id] += diff[i * W + (j-1)];
            if(i > 0 && j > 0) diff[id] -= diff[(i-1) * W + (j-1)];
        }
    }

    for(int i = 0;i < n;i++)
    {
        string ans;
        ans.reserve(m);
        for(int j = 0;j < m;j++)
        {
            ans.push_back(diff[i * W + j] > 0 ? '1' : '0');
        }
        cout << ans << '\n';
    }
}

//交换i，j后f(P)的差值为
//i < j
//p[i]*((n-i-1) - i) = p[i]*(n-1-2i)
//p[i]*((n-1-2j))
//diff = p[i](2i-2j)
//p[i]*(2i-2j) - p[j]*(2i-2j)
//p`(x) = p(x) + k*n;
//求出p(x)
//强行固定p`[k] = x
//交换原来的k位置上的数 计算出差值
//寻找一个a,b来填补差值
//swap(i,j) O(1)

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 200000 + 5;

int p[N];

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, x;
    cin >> n >> k >> x;

    int idx1 = -1;
    for (int i = 0; i < n; i++)
    {
        cin >> p[i];
        if (p[i] == x) idx1 = i;
    }

    swap(p[k], p[idx1]);

    ll delta0 = 2LL * (k - idx1) * (p[idx1] - p[k]);
    delta0 %= n;
    if (delta0 < 0) delta0 += n;

    if (delta0 == 0)
    {
        for (int i = 0; i < n; i++) cout << p[i] << (i+1==n ? '\n' : ' ');
        return 0;
    }


    ll target = delta0;

    bool found = false;


    for (int d = 1; d <= 20 && !found && d < n; d++)
    {
        for (int i = 0; i + d < n; i++)
        {
            if (i == k || i + d == k) continue;

            ll delta1 = 2LL * d * (p[i] - p[i + d]);
            delta1 %= n;
            if (delta1 < 0) delta1 += n;

            if (delta1 == target)
            {
                swap(p[i], p[i + d]);
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        cout << -1 << '\n';
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            cout << p[i] << (i+1==n ? '\n' : ' ');
        }
    }

    return 0;
}

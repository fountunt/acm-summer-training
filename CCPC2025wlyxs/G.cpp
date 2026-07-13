#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
#define endl "\n"

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,q;
    cin >> n >> q;

    vector<int> a(n);
    map<int,vector<int>> mp;
    for(int i = 0;i < n;i++)
    {
        cin >> a[i];
        mp[a[i]].push_back(i);
    }

    const int B = 500;
    map<pair<int,int>,ll> ans;

    while(q--)
    {
        int x,y;
        cin >> x >> y;

        if(x == y)
        {
            ll m = mp[x].size();
            cout << (m-1)*m/2 << endl;
            continue;
        }

        auto& mpx = mp[x];
        auto& mpy = mp[y];
        if(mpx.size() < B)
        {
            ll res = 0;
            for(auto p : mpx)
            {
                res += mpy.end() - lower_bound(mpy.begin(),mpy.end(),p);
            }
            cout << res << endl;
        }
        else if(mpy.size() < B)
        {
            ll res = 0;
            for(auto p : mpy)
            {
                res += lower_bound(mpx.begin(),mpx.end(),p) - mpx.begin();
            }
            cout << res << endl;
        }
        else
        {
            if(ans.count({x,y}))
            {
                cout << ans[{x,y}] << endl;
            }
            else
            {
                if(mpx.size() < mpy.size())
                {
                    ll res = 0;
                    for(auto p : mpx)
                    {
                        res += mpy.end() - lower_bound(mpy.begin(),mpy.end(),p);
                    }
                    cout << res << endl;
                    ans[{x,y}] = res;
                }
                else
                {
                    ll res = 0;
                    for(auto p : mpy)
                    {
                        res += lower_bound(mpx.begin(),mpx.end(),p) - mp[x].begin();
                    }
                    cout << res << endl;
                    ans[{x,y}] = res;
                }
            }
        }
    }

    return 0;
}

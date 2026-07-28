#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll gcdll(ll a,ll b)
{
    if(a < 0) a = -a;
    if(b < 0) b = -b;
    while(b)
    {
        ll r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while(T--)
    {
        int n,t;
        cin >> n >> t;

        vector<ll> S(t+1,0);
        for(int i = 0;i < n;i++)
        {
            ll a,b;
            cin >> a >> b;

            if(t == 0) continue;

            ll p = 1;
            for(int k = 1;k <= t;k++)
            {
                p *= b;
                S[k] += a * p;
            }
        }
        if(t == 0)
        {
            cout << 0 << "\n";
            continue;
        }

        bool infinity = false;
        for(int k = 1;k < t;k++)
        {
            if(S[k] != 0)
            {
                infinity = true;
                break;
            }
        }

        if(infinity)
        {
            cout << "infinity\n";
            continue;
        }

        if(S[t] == 0)
        {
            cout << 0 << "\n";
            continue;
        }

        ll num = S[t];
        if(t % 2 == 0) num = -num;

        ll den = t;
        ll g = gcdll(num,den);
        num /= g;
        den /= g;

        if(den == 1) cout << num << "\n";
        else cout << num << '/' << den << '\n';
    }
}

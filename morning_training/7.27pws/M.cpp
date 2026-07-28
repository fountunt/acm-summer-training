#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    int T;
    cin >> T;
    while(T--)
    {
        int n;
        string A,B;
        cin >> n >> A >> B;

        if(A.find('1') == string::npos)
        {
            cout << (B.find('1') == string::npos ? "YES" : "NO") << '\n';
            continue;
        }

        auto getK = [&](const string &s)
        {
            int k = 0,pref = 0;
            for(char c : s)
            {
                pref ^= (c - '0');
                k += pref;
            }
            return k;
        };

        int kA = getK(A);
        int kB = getK(B);

        if(kB == kA || kB == n - kA + 1) cout << "YES\n";
        else cout << "NO\n";
    }
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void solve()
{
    int n;
    cin >> n;
    string s;
    cin >> s;
    int N = (1 << n) - 1;
    vector<int> a(N+1,0),b(N+1,0);
    for(int i = 1;i <= N;i++) a[i] = s[i-1] - '0';

    int pos = -1;
    for(int i = 1;i <= N;i++)
    {
        if(a[i] == 0) continue;
        if(pos == -1)
        {
            pos = i;
            continue;
        }

        b[i] = pos;
        int z = i ^ pos;
        a[z] ^= 1;
        a[i] ^= 1;
        a[pos] ^= 1;
        if(z < i) pos = z;
        else pos = -1;
    }

    for(int i = 1;i <= N;i++)
    {
        cout << b[i] << " ";
    }
    cout << "\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while(T--) solve();
}

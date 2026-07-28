#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main()
{
    int n;
    cin >> n;
    vector<ll> A(n);
    for(int i = 0;i < n;i++) cin >> A[i];

    ll G = A[0];
    for(int i = 1;i < n;i++) G = gcd(G,A[i]);
    ll G_odd = G;
    while(G_odd % 2 == 0) G_odd /= 2;

    priority_queue<int,vector<int>,greater<int>> pq;
    for(ll x : A)
    {
        int cnt = 0;
        while(x % 2 == 0)
        {
            x /= 2;
            cnt++;
        }
        pq.push(cnt);
    }

    for(int t = 0;t < n-1;t++)
    {
        int x = pq.top();pq.pop();
        int y = pq.top();pq.pop();
        pq.push(min(x,y)+1);
    }

    int K = pq.top();
    ll ans = G_odd << K;
    cout << ans << "\n";
}

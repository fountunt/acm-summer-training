#include <bits/stdc++.h>

using namespace std;

const int maxn = 1e6 + 7;

int m[maxn], A[maxn], prime[maxn], nxt[maxn];

int main() {
    for (int p = 2; p < maxn; ++p) {
        if (prime[p] == 0) {
            for (int x = p; x < maxn; x += p) {
                if (prime[x] == 0) prime[x] = p;
            }
        }
        if (prime[p / prime[p]] == prime[p]) nxt[p] = nxt[p / prime[p]];
        else nxt[p] = p / prime[p];
        m[p] = m[nxt[p]] + 1;
        A[p] = A[p / prime[p]] + 1;
    }
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        cout << m[n] + A[n] - 1 << "\n";
    }
    return 0;
}

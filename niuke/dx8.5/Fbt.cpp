#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    int n = (int)s.size();

    unsigned pred[26] = {0};
    bool involved[26] = {false};

    vector<int> Z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        int zi = 0;
        if (i <= r) zi = min(r - i + 1, Z[i - l]);
        while (i + zi < n && s[zi] == s[i + zi]) zi++;
        Z[i] = zi;
        if (i + zi - 1 > r) {
            l = i;
            r = i + zi - 1;
        }

        if (zi == n - i) {
            cout << 0 << "\n";
            return 0;
        }
        int a = s[zi] - 'a', b = s[i + zi] - 'a';
        pred[b] |= (1u << a);
        involved[a] = involved[b] = true;
    }

    int idx[26];
    fill(idx, idx + 26, -1);
    vector<int> letters;
    for (int i = 0; i < 26; i++)
        if (involved[i]) {
            idx[i] = (int)letters.size();
            letters.push_back(i);
        }
    int m = (int)letters.size();

    unsigned pcomp[26] = {0};
    for(int x = 0;x < 26;x++)
        if(idx[x] >= 0)
            for(int y = 0;y < 26;y++)
                if((pred[x] >> y) & 1u && idx[y] >= 0)
                    pcomp[x] |= (1u << idx[y]);

    int indeg[26] = {0};
    for(int x : letters)
        for(int y = 0;y < 26;y++)
            if((pred[x] << y) & 1u) indeg[x]++;
    queue<int> q;
    int cnt = 0;
    for(int x : letters) if(indeg[x] == 0) q.push(x);
    while(!q.empty()){
        int u = q.front();q.pop();cnt++;
        for(int v : letters)
            if((pred[v] >> u) & 1u)
                if(--indeg[v] == 0) q.push(v);
    }
    if(cnt < m){
        cout << 0 << "\n";
        return 0;
    }

    unsigned full = (1u << m) - 1;
    vector<unsigned> dp(full + 1,0);
    dp[0] = 1;
    for(unsigned mask = 0;mask <= full;mask++){
        unsigned dv = dp[mask];
        if(dv == 0) continue;
        unsigned rem = full ^ mask;
        while(rem){
            int t = __builtin_ctz(rem);
            rem &= rem - 1;
            int x = letters[t];
            if((pcomp[x] & mask) == pcomp[x]){
                dp[mask | (1u << t)] += dv;
            }
        }
    }
    unsigned ans = dp[full];

    for(int k = m + 1;k <= 26;k++) ans *= (unsigned)k;

    cout << ans << "\n";
    return 0;
}

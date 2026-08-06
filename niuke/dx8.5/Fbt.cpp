#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    int n = (int)s.size();

    unsigned pred[26] = {0};
    bool involved[26] = {false};

    vector<int> Z(n,0);
    int l = 0,r = 0;
    for(int i = 1;i < n;i++){
        int zi = 0;
        if(i <= r) zi = min(r - i + 1,Z[i-l]);
        while(i + zi < n && s[zi] == s[i + zi]) zi++;
        Z[i] = zi;
        if(i + zi - 1 > r){l = i;r = i + zi - 1;}

        if(zi == n - i) {cout << 0 << "\n";return 0;}
        int a = s[zi] - 'a',b = s[i + zi] - 'a';
        pred[b] |= (1u << a);
        involved[a] = involved[b] = true;
    }
}

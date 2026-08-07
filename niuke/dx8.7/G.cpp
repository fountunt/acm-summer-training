#include <bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    set<int> s;
    for(int i = 0; i < n; i++){ int x; cin >> x; s.insert(x); }
    cout << ((int)s.size() <= 3 ? "YES" : "NO") << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        long long k;
        string s;
        cin >> n >> k >> s;
        int m = 2 * n;
        long long red = 0, blue = 0;
        for (int i = 0; i < m; ++i) {
            if (s[i] == '1') {
                if (s[(i + 1) % m] == '0') {
                    if (i % 2 == 0) {
                        ++red;
                    } else {
                        ++blue;
                    }
                } else {
                    if (i % 2 == 0) {
                        ++blue;
                    } else {
                        ++red;
                    }
                }
            }
        }
        cout << red << ' ' << blue << '\n';
    }
    return 0;
}

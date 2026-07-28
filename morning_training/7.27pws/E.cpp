#include <bits/stdc++.h>
using namespace std;

struct Hint {
    int x;
    string t;
};

int N, M;
vector<Hint> hints;

string solve_without(int ignore) {
    string s(M, '\0');

    vector<Hint> sorted = hints;
    sort(sorted.begin(), sorted.end(), [](const Hint& a, const Hint& b) {
        return a.x < b.x;
    });

    int lastEnd = 0;
    bool ok = true;

    for (int i = 0; i < N && ok; ++i) {
        if (i == ignore) continue;

        int start = sorted[i].x - 1;
        int len = sorted[i].t.size();

        if (start > lastEnd + 1) {
            ok = false;
            break;
        }

        for (int j = 0; j < len; ++j) {
            int pos = start + j;
            if (pos >= M) {
                ok = false;
                break;
            }
            if (s[pos] != '\0' && s[pos] != sorted[i].t[j]) {
                ok = false;
                break;
            }
            s[pos] = sorted[i].t[j];
        }

        lastEnd = max(lastEnd, start + len - 1);
    }

    if (!ok) return "";

    for (int i = 0; i < M; ++i) {
        if (s[i] == '\0') return "";
    }

    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    hints.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> hints[i].x >> hints[i].t;
    }

    set<string> solutions;

    string res = solve_without(-1);
    if (!res.empty()) solutions.insert(res);

    for (int i = 0; i < N; ++i) {
        res = solve_without(i);
        if (!res.empty()) solutions.insert(res);
    }

    if (solutions.empty()) {
        cout << -1 << '\n';
    } else if (solutions.size() == 1) {
        cout << *solutions.begin() << '\n';
    } else {
        cout << -2 << '\n';
    }

    return 0;
}

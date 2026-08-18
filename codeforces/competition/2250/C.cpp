#include <bits/stdc++.h>
using namespace std;

const int N = 5000;

int n;
int l[N + 10], r[N + 10], u[N + 10], v[N + 10];

bool check(int m) {
	int j = 1;

	for (int i = 1; i <= n && j <= m; ++i) {
		int x = m - j + 1;
		if ((j < l[i] || j > r[i]) && (x < u[i] || x > v[i])) ++j;
	}

	return j == m + 1;
}

void work() {
	cin >> n;
	for (int i = 1; i <= n; ++i) cin >> l[i] >> r[i] >> u[i] >> v[i];

	for (int m = n; m >= 1; --m) {
		if (!check(m)) continue;
		cout << m << '\n';
		return;
	}

	cout << 0 << '\n';
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

	int T; cin >> T;
	while (T--) work();

	return 0;
}

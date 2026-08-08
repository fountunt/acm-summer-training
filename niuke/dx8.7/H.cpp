// Problem H. Modulo Triples
// Idea: 0<=y<z 且 x=y+z 时必有 x mod z = y，故尽量构造 x=y+z。
//   A=N+N/2, B=2N+N/2。
//   N 奇：先 (3N-1, 0, A)（3N-1=2A，mod A 余 0）；
//          再 i=1..N/2： (A+i, 2i, A-i), (B+i-1, 2i-1, B-i)。
//   N 偶：先 (B, N, A)（B=A+N，mod A 余 N）；
//          再 i=1..N/2-1：(B+i, 2i, B-i), (A+i+1, 2i+1, A-i)；
//          最后 (A+1, 0, 1)。
//   按区间核对 0..3N-1 恰好各出现一次。
// 复杂度: 时间 O(N)，空间 O(1)
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    long long N;
    if (!(cin >> N)) return 0;
    long long A = N + N / 2;
    long long B = 2 * N + N / 2;

    if (N % 2 == 1) {
        cout << 3 * N - 1 << ' ' << 0 << ' ' << A << '\n';
        for (long long i = 1; i <= N / 2; ++i) {
            cout << A + i << ' ' << 2 * i << ' ' << A - i << '\n';
            cout << B + i - 1 << ' ' << 2 * i - 1 << ' ' << B - i << '\n';
        }
    } else {
        cout << B << ' ' << N << ' ' << A << '\n';
        for (long long i = 1; i < N / 2; ++i) {
            cout << B + i << ' ' << 2 * i << ' ' << B - i << '\n';
            cout << A + i + 1 << ' ' << 2 * i + 1 << ' ' << A - i << '\n';
        }
        cout << A + 1 << ' ' << 0 << ' ' << 1 << '\n';
    }
    return 0;
}

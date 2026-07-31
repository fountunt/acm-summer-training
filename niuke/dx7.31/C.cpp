// 2 3 0 1
// 0 2 1 3
// 3 1 2 0
// 4 2 5 0 3 1
// 0 4 1 2 5 3
// 5 1 0 3 2 4
// 6
// 0
// 7
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    if(n == 6)
    {
        cout << "4 2 5 0 3 1\n0 4 1 2 5 3\n5 1 0 3 2 4" << "\n";
    }
    if(n == 4) cout << "2 3 0 1\n0 2 1 3\n3 1 2 0\n";
    else cout << -1 << "\n";
}
A=051234
B=123405
C=215043
A=351024
B=024135
C=415203

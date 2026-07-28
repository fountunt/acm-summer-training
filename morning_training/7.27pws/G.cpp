#include <bits/stdc++.h>
using namespace std;
using ll = unsigned long long;

int main()
{
	ll n,a,b;
	cin >> n >> a >> b;
	if(a != b)
	{
		cout << "NO" << "\n";
		return 0;
	}

	if((((n | n << 1) & ((n | n << 1) << 1)) == (n << 1)) || (((n & n << 1) | ((n & n << 1) << 1)) == (n << 1)))
	{
		cout << "YES" << "\n";
	}
	else cout << "NO" << "\n";
}

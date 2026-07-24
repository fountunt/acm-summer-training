#include <bits/stdc++.h>

using namespace std;

const int N=22,P=998244353;

int n;
int p[N],pre[N];
int f[1<<N];


int main(){
	cin >> n;
	for(int i=0; i<n; i++) cin >> p[i],p[i]--;

	int cnt=0;
	for(int i=0; i<n; i++)
		for(int j=i+1; j<n; j++)
			if(p[i]>p[j])
				pre[i]|=1<<j,cnt++;

	if(cnt==0){
		int ans=1;
		for(int i=1; i<=n; i++) ans=1ll*ans*i%P;
		cout << ans << endl;
		return 0;
	}

	int all=(1<<n)-1;
	f[0]=1;

	for(int i=0; i<all; i++)
		if(f[i])
			for(int j=0; j<n; j++)
				if(!(i>>j&1)&&(pre[j]&i)==pre[j]){
					int nxt=i|(1<<j);
					f[nxt]=(f[nxt]+f[i])%P;
				}

	cout << 2ll*f[all]%P << endl;
	return 0;
}
//状压dp
//拓扑图

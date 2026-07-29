#include<bits/stdc++.h>
using namespace std;
int n;
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        if(n==1){
            puts("1 ");
            continue;
        }
        int m=(n+1)/2+1;
        if(n&1) printf("1 %d 2 ",m);
        else printf("1 %d ",m);
        for(int i=m+1;i<=n;i++)
            printf("%d %d ",2*m-i,i);
        puts("");
    }
}

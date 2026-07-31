#include <bits/stdc++.h>
using namespace std;

bool isPerm(const vector<int>& v, int m) {
    vector<bool> vis(m, false);
    for (int x : v) {
        if (vis[x]) return false;
        vis[x] = true;
    }
    return true;
}

// a,b高位在前，len位base进制加法，c高位在前
bool add(const vector<int>& a, const vector<int>& b, vector<int>& c, int len, int base) {
    c.resize(len);
    int carry = 0;
    // 从最低位（数组末尾）开始算
    for (int i = len - 1; i >= 0; i--) {
        int s = a[i] + b[i] + carry;
        c[i] = s % base;
        carry = s / base;
    }
    return carry == 0; // 最高位无溢出
}

bool columnNoDup(const vector<int>&a,const vector<int>&b,const vector<int>&c)
{
    int len=a.size();
    for(int i=0;i<len;i++)
    {
        int x=a[i],y=b[i],z=c[i];
        if(x==y || x==z || y==z)
            return false;
    }
    return true;
}

void print(const vector<int>& num) {
    for(int d : num) cout << d;
    cout << "\n";
}

int main()
{
    const int BASE = 8;
    const int LEN  = 8;
    // 固定 B = 0 1 2 3 4 5 6 7（高位在前）
    vector<int> B = {0,1,2,3,4,5,6,7};

    vector<int> A(LEN);
    for(int i=0;i<LEN;i++) A[i]=i;

    long long cnt=0;
    do{
        vector<int> C;
        bool ok = add(A,B,C,LEN,BASE);
        if(ok)
        {
            if(isPerm(C,BASE) && columnNoDup(A,B,C))
            {
                cout<<"找到解！\n";
                cout<<"A=";print(A);
                cout<<"B=";print(B);
                cout<<"C=";print(C);
            }
        }
        cnt++;
    }while(next_permutation(A.begin(),A.end()));

    cout<<"全部40320个A检索完毕，无满足条件的解\n";
    return 0;
}
// 找到解！
// A=153024
// B=012345
// C=205413
// 找到解！
// A=245103
// B=012345
// C=301452
// 找到解！
// A=251403
// B=012345
// C=304152
// 找到解！
// A=451023
// B=012345
// C=503412
// 找到解！
// A=451032
// B=012345
// C=503421

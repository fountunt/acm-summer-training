// ============================================================
// 待验证程序 std.cpp —— 你的算法代码
// 与 bf 读同样的输入,输出同样的格式
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int a[26];

int main(){
    string s1,s2;
    cin >> s1 >> s2;

    for(int i = 0;i < 26;i++) a[i] = -1;

    int len1 = s1.size();
    int len2 = s2.size();

    if(len1 == 1 || len2 == 1){
        cout << -1 << "\n";
        return 0;
    }

    for(int i = 0;i < len2-1;i++){
        int idx = s2[i] - 'a';
        a[idx] = i;
    }

    bool ok = false;
    int l = len1,r = -1;
    for(int i = 1;i < len1;i++){    // 前半必须非空,i 从 1 开始
        int idx = s1[i] - 'a';
        if(a[idx] != -1){
            ok = true;
            if(i + len2 - a[idx] < l + len2 - r){
                l = i;
                r = a[idx];
            }
        }
    }

    if(!ok){
        cout << -1 << "\n";
        return 0;
    }

    for(int i = 0;i < l;i++){
        cout << s1[i];
    }
    for(int i = r;i < len2;i++){
        cout << s2[i];
    }
    cout << "\n";
}

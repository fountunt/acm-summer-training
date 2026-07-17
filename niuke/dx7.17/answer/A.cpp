#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin >> n;
    while(n--){
        string s;
        cin >> s;
        bool f1 = false, f2 = false,f3 = false; //false 不是正常人
        if(s.size()!=8) f1 = true;
        for(int i=0;i<8;i++){
            if(i % 2 == 1){
                if(s[i] != 'a' && s[i] != 'e' && s[i] != 'i' && s[i] != 'o' && s[i] != 'u'){
                    f2 = true;
                }
            }else{
                if(s[i] == 'a'||s[i] == 'e'||s[i] == 'i'||s[i] == 'o'||s[i] == 'u'){
                    f3 = true;
                }
            }
        }
        if(f1 || f2 || f3){
            cout << "Well-Being" << '\n';
        }else{
            cout << "Suspected Virus" << '\n';
        }
    }
}

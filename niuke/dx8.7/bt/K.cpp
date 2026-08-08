#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;cin >> n;
    cin.ignore();

    vector<vector<string>> words(n);
    vector<int> p(n,0);

    for(int i = 0;i < n;i++){
        string line;
        getline(cin,line);
        stringstream ss(line);
        string w;
        while(ss >> w) words[i].push_back(w);
    }

    auto abbr = [&](int i){
        string s;
        for(int j = 0;j < (int)words[i].size();j++){
            if(j < p[i]) s += words[i][j];
            else s += words[i][j][0];
        }
        return s;
    };

    while(true){
        map<string,int> cnt;
        vector<string> cur(n);
        for(int i = 0;i < n;i++){
            cur[i] = abbr(i);
            cnt[cur[i]]++;
        }
        bool done = true;
        for(int i = 0;i < n;i++){
            if(cnt[cur[i]] > 1){
                done = false;
                if(p[i] < (int)words[i].size()) p[i]++;
            }
        }
        if(done) break;
    }

    for(int i = 0;i < n;i++){
        cout << abbr(i) << "\n";
    }
}

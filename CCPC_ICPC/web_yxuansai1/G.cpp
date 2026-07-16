#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;

    set<pair<int,int>> st;
    for(int i = 0;i < m;i++)
    {
        int a, b;
        cin >> a >> b;
        st.insert({a, b});
    }

    for(int i = 1;i < n;i++)
    {
        if(st.find({i, i + 1}) == st.end())
        {
            cout << "No";
            return 0;
        }
    }
    cout << "Yes";
    return 0;
}
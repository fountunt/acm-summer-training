#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    cout << fixed << setprecision(12);

    while(T--)
    {
        int n;
        cin >> n;

        double d = 0.011;
        double half = 4.5*d;

        vector<pair<double,double>> pts;

        for(int i = 0;i < 10 && (int)pts.size() < n;i++)
        {
            for(int j = 0;j < 10 && (int)pts.size() < n;j++)
            {
                double x = -half + i * d;
                double y = -half + j * d;
                pts.push_back({x,y});
            }
        }

        cout << 2 * n << "\n";

        for(auto[x,y] : pts) cout << x << ' ' << y << " 0.0\n";
        for(auto[x,y] : pts) cout << x << ' ' << y << " 1.0\n";
    }
}

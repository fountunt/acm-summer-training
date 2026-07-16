#include <bits/stdc++.h>
using namespace std;

void solve()
{
    int s;
    cin >> s;

    // teamname -> (problemid -> [(time, result)])
    map<string, map<char, vector<pair<int,int>>>> mp;

    for (int i = 0; i < s; i++)
    {
        string name, res;
        char pid;
        int t;
        cin >> name >> pid >> t >> res;

        int r;
        if (res == "Accepted") r = 1;
        else if (res == "Rejected") r = 2;
        else r = 0;

        mp[name][pid].push_back({t, r});
    }

    vector<pair<string, pair<int,int>>> best, worst;

    for (auto &[name, probs] : mp)
    {
        int bs = 0, bp = 0;
        int ws = 0, wp = 0;

        for (auto &[pid, v] : probs)
        {
            sort(v.begin(), v.end());

            // --- Best for this team ---
            int cnt = 0, bpen = INT_MAX;
            bool solve = false;
            for (auto &[t, r] : v)
            {
                if (r == 0)
                {
                    solve = true;
                    bpen = min(bpen, t + 20 * cnt);
                    cnt++;
                }
                else if (r == 1)
                {
                    solve = true;
                    bpen = min(bpen, t + 20 * cnt);
                    break;
                }
                else cnt++;
            }
            if (solve) { bs++; bp += bpen; }

            // --- Worst for this team ---
            bool hasAC = false;
            for (auto &[t, r] : v) if (r == 1) { hasAC = true;break; }
            if (hasAC)
            {
                ws++;
                cnt = 0;
                for (auto &[t, r] : v)
                {
                    if (r == 2 || r == 0) cnt++;
                    if (r == 1) { wp += t + 20 * cnt; break; }
                }
            }
        }

        best.push_back({name, {bs, bp}});
        worst.push_back({name, {ws, wp}});
    }

    vector<string> ans;
    for (int i = 0; i < (int)best.size(); i++)
    {
        auto [sT, pT] = best[i].second;
        bool ok = true;
        for (int j = 0; j < (int)worst.size(); j++)
        {
            if (best[i].first == worst[j].first) continue;
            auto [sO, pO] = worst[j].second;
            if (sO > sT || (sO == sT && pO < pT)) { ok = false;break; }
        }
        if (ok) ans.push_back(best[i].first);
    }

    sort(ans.begin(), ans.end());
    for (int i = 0; i < (int)ans.size(); i++)
    {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) solve();
    return 0;
}
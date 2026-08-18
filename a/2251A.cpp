#include <bits/stdc++.h>
using namespace std;

// 每个请求的阶段状态机
enum {
    ST_NOT = 0,        // 未到达
    ST_P_PRE_TODO,     // 可调度 P PRE(local)
    ST_P_PRE_INF,      // P PRE 在途
    ST_WAIT_UP1,       // 等输入阶段 UP 传输完成
    ST_P_PROC_TODO,    // 可调度 P PROC(remote)
    ST_P_PROC_INF,     // P PROC 在途
    ST_WAIT_DOWN1,     // 等输入阶段 DOWN 传输完成
    ST_P_POST_TODO,    // 可调度 P POST(local)
    ST_P_POST_INF,     // P POST 在途
    ST_OUT_TODO,       // ready,可调度 D PRE(local),当前迭代 iter
    ST_D_PRE_INF,      // D PRE 在途
    ST_WAIT_UPN,       // 等输出 UP 传输完成
    ST_D_PROC_TODO,    // 可调度 D PROC(remote)
    ST_D_PROC_INF,     // D PROC 在途
    ST_WAIT_DOWNN,     // 等输出 DOWN 传输完成
    ST_D_POST_TODO,    // 可调度 D POST(local)
    ST_D_POST_INF,     // D POST 在途
    ST_FINISHED        // 已 FIN
};

struct Req {
    int remote = -1;      // 分配的远程机
    long long lin = 0;    // 输入长度
    int stage = ST_NOT;
    int iter = 0;         // 已完成的输出迭代数
    bool finished = false;
    int seen = 0;         // 本帧去重标记
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto readLine = []() -> string {
        string l;
        if (!getline(cin, l)) return string();
        if (!l.empty() && l.back() == '\r') l.pop_back();
        return l;
    };

    // ---- 启动配置:系统参数 + 评分参数 + 任务时间表(时间表可忽略,只消费输入) ----
    string line = readLine();
    istringstream s1(line);
    int K;
    double S, latency, bandwidth;
    int bytes_per_token, num_layers;
    s1 >> K >> S >> latency >> bandwidth >> bytes_per_token >> num_layers;

    line = readLine();
    istringstream s2(line);
    double SLO1, SLO2, tp_UB, tp_base, dist_base, w_tp, w_c;
    s2 >> SLO1 >> SLO2 >> tp_UB >> tp_base >> dist_base >> w_tp >> w_c;

    line = readLine();
    int N = stoi(line);
    // 任务时间表:用于计算最优输出组大小(输入阶段组固定为 1)
    struct TableCol {
        vector<int> bs;
        vector<double> val;
    };
    TableCol dec_pre, dec_proc, dec_post;
    {
        vector<pair<int, double>> raw[3];
        for (int i = 0; i < N; ++i) {
            line = readLine();
            istringstream ts(line);
            int bs;
            double a, b, c, d, e, f;
            ts >> bs >> a >> b >> c >> d >> e >> f;
            if (d >= 0) raw[0].push_back({bs, d});
            if (e >= 0) raw[1].push_back({bs, e});
            if (f >= 0) raw[2].push_back({bs, f});
        }
        for (int t = 0; t < 3; ++t) {
            sort(raw[t].begin(), raw[t].end());
            TableCol &col = (t == 0 ? dec_pre : (t == 1 ? dec_proc : dec_post));
            for (auto &p : raw[t]) { col.bs.push_back(p.first); col.val.push_back(p.second); }
        }
    }
    auto lookup = [](const TableCol &tc, int x) -> double {
        // 精确/线性插值;低于最小用首值,高于最大用末值
        if (tc.bs.empty()) return 1.0;
        if (x <= tc.bs.front()) return tc.val.front();
        if (x >= tc.bs.back()) return tc.val.back();
        size_t lo = 0, hi = tc.bs.size();
        while (lo + 1 < hi) {
            size_t mid = (lo + hi) / 2;
            if (tc.bs[mid] <= x) lo = mid;
            else hi = mid;
        }
        double x0 = tc.bs[lo], x1 = tc.bs[lo + 1];
        double y0 = tc.val[lo], y1 = tc.val[lo + 1];
        return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
    };
    // 最优输出组大小:每 token 成本 = (decode_pre + decode_proc + decode_post + 2×传输)/m
    int G = 1;
    {
        double bestCost = 1e300;
        for (int m = 1; m <= 4096; ++m) {
            double dpre = lookup(dec_pre, m);
            double dproc = lookup(dec_proc, m);
            double dpost = lookup(dec_post, m);
            double trans = latency + 8.0 * m * bytes_per_token / (bandwidth * 1e6);
            double cost = (dpre + dproc + dpost + 2.0 * trans) / m;
            if (cost < bestCost - 1e-12) { bestCost = cost; G = m; }
        }
    }

    vector<Req> req;
    vector<bool> remoteBusy(K, false);
    bool localBusy = false;
    int frameId = 0;

    // 每个"可调度"阶段一个 FIFO 队列,惰性出队(head 指针)
    vector<int> qPPre;  size_t hPPre = 0;
    vector<int> qPProc; size_t hPProc = 0;
    vector<int> qPPost; size_t hPPost = 0;
    vector<int> qDPre;  size_t hDPre = 0;
    vector<int> qDProc; size_t hDProc = 0;
    vector<int> qDPost; size_t hDPost = 0;

    while (true) {
        line = readLine();
        if (line.empty()) break;   // EOF
        if (line == "END") break;  // 结束

        line = readLine();         // 事件数
        int e = stoi(line);        // 事件数
        for (int j = 0; j < e; ++j) {
            line = readLine();
            istringstream ss(line);
            string type;
            ss >> type;
            if (type == "ARR") {
                int rid;
                long long lin;
                ss >> rid >> lin;
                if ((int)req.size() <= rid) req.resize(rid + 1);
                req[rid].lin = lin;
                req[rid].stage = ST_P_PRE_TODO;
                qPPre.push_back(rid);
            } else if (type == "TDN") {
                string server, w1, w2;
                ss >> server >> w1 >> w2;
                if (w1 == "P" && w2 == "PRE") {
                    int remote, rid;
                    double dur;
                    ss >> remote >> rid >> dur;
                    req[rid].stage = ST_WAIT_UP1;
                    localBusy = false;
                } else if (w1 == "P" && w2 == "PROC") {
                    int ls, le, remote, rid;
                    double dur;
                    ss >> ls >> le >> remote >> rid >> dur;
                    req[rid].stage = ST_WAIT_DOWN1;
                    remoteBusy[remote] = false;
                } else if (w1 == "P" && w2 == "POST") {
                    int remote, rid;
                    double dur;
                    ss >> remote >> rid >> dur;
                    req[rid].stage = ST_OUT_TODO;
                    req[rid].iter = 0;
                    qDPre.push_back(rid);
                    localBusy = false;
                } else if (w1 == "D" && w2 == "PRE") {
                    int minus1, m;
                    double dur;
                    ss >> minus1 >> m;
                    for (int k = 0; k < m; ++k) {
                        int rid;
                        ss >> rid;
                        req[rid].stage = ST_WAIT_UPN;
                    }
                    ss >> dur;
                    localBusy = false;
                } else if (w1 == "D" && w2 == "PROC") {
                    int remote, m;
                    double dur;
                    ss >> remote >> m;
                    for (int k = 0; k < m; ++k) {
                        int rid;
                        ss >> rid;
                        req[rid].stage = ST_WAIT_DOWNN;
                    }
                    ss >> dur;
                    remoteBusy[remote] = false;
                } else { // D POST
                    int minus1, m;
                    double dur;
                    ss >> minus1 >> m;
                    vector<int> rids(m);
                    for (int k = 0; k < m; ++k) ss >> rids[k];
                    ss >> dur;
                    for (int rid : rids) {
                        Req &r = req[rid];
                        ++r.iter;
                        if (r.finished) r.stage = ST_FINISHED;
                        else { r.stage = ST_OUT_TODO; qDPre.push_back(rid); }
                    }
                    localBusy = false;
                }
            } else if (type == "XDN") {
                string dir, kind;
                int remote, m;
                long long size;
                ss >> dir >> remote >> size >> kind >> m;
                if (kind == "PRE") {
                    int rid;
                    ss >> rid;
                    if (dir == "UP") { req[rid].stage = ST_P_PROC_TODO; qPProc.push_back(rid); }
                    else { req[rid].stage = ST_P_POST_TODO; qPPost.push_back(rid); }
                } else { // DEC
                    vector<int> rids(m);
                    for (int k = 0; k < m; ++k) ss >> rids[k];
                    if (dir == "UP") {
                        for (int rid : rids) {
                            req[rid].stage = ST_D_PROC_TODO; qDProc.push_back(rid);
                        }
                    } else {
                        for (int rid : rids) {
                            req[rid].stage = ST_D_POST_TODO; qDPost.push_back(rid);
                        }
                    }
                }
            } else if (type == "FIN") {
                int rid;
                ss >> rid;
                Req &r = req[rid];
                r.finished = true;
                if (r.stage != ST_D_POST_INF) r.stage = ST_FINISHED;
            }
        }

        // ---- 调度 ----
        vector<string> out;
        bool localUsed = false;
        vector<bool> remoteUsed(K, false);
        ++frameId;

        // ============ local: 优先级 P POST > P PRE > D POST(组) > D PRE(组) ============
        // 1) P POST:单个,完成输入阶段(降 TDR)
        while (hPPost < qPPost.size()) {
            int id = qPPost[hPPost];
            Req &r = req[id];
            if (r.finished || r.stage != ST_P_POST_TODO) { ++hPPost; continue; }
            if (localBusy || localUsed) break;
            out.push_back("E P POST " + to_string(r.remote) + " " + to_string(id));
            r.stage = ST_P_POST_INF;
            ++hPPost;
            localBusy = true;
            localUsed = true;
            break;
        }
        // 2) P PRE:单个,启动输入;remote 选负载最少的机器
        if (!localBusy && !localUsed) {
            while (hPPre < qPPre.size()) {
                int id = qPPre[hPPre];
                Req &r = req[id];
                if (r.finished || r.stage != ST_P_PRE_TODO) { ++hPPre; continue; }
                int best = 0;
                {
                    // 按"该 remote 未完成请求的 Lin 总和"选负载最轻的机器
                    vector<long long> load(K, 0);
                    for (auto &rr : req)
                        if (rr.remote >= 0 && !rr.finished) load[rr.remote] += rr.lin;
                    for (int k = 1; k < K; ++k)
                        if (load[k] < load[best]) best = k;
                }
                r.remote = best;
                out.push_back("E P PRE " + to_string(best) + " " + to_string(id));
                r.stage = ST_P_PRE_INF;
                ++hPPre;
                localBusy = true;
                localUsed = true;
                break;
            }
        }
        // 3) D POST:组(同一 remote 的待完成请求合成一组,让 token 产出更均匀)
        if (!localBusy && !localUsed) {
            int groupRemote = -1;
            for (size_t j = hDPost; j < qDPost.size(); ++j) {
                int id = qDPost[j];
                if (!req[id].finished && req[id].stage == ST_D_POST_TODO) {
                    groupRemote = req[id].remote;
                    break;
                }
            }
            if (groupRemote >= 0) {
                vector<int> mem;
                for (size_t j = hDPost; j < qDPost.size() && (int)mem.size() < G; ++j) {
                    int id = qDPost[j];
                    Req &r = req[id];
                    if (r.finished || r.stage != ST_D_POST_TODO) continue;
                    if (r.remote != groupRemote) continue;
                    if (r.seen == frameId) continue;   // 同一请求只收集一次
                    r.seen = frameId;
                    mem.push_back(id);
                }
                if (!mem.empty()) {
                    string s = "E D POST -1 " + to_string(mem.size());
                    for (int id : mem) { s += " " + to_string(id); req[id].stage = ST_D_POST_INF; }
                    out.push_back(s);
                    localBusy = true;
                    localUsed = true;
                }
            }
        }
        // 4) D PRE:组(同一 remote 的 ready 请求合成一组,降低组周期、让 token 更连续)
        if (!localBusy && !localUsed) {
            int groupRemote = -1;
            for (size_t j = hDPre; j < qDPre.size(); ++j) {
                int id = qDPre[j];
                if (!req[id].finished && req[id].stage == ST_OUT_TODO) {
                    groupRemote = req[id].remote;
                    break;
                }
            }
            if (groupRemote >= 0) {
                vector<int> mem;
                for (size_t j = hDPre; j < qDPre.size() && (int)mem.size() < G; ++j) {
                    int id = qDPre[j];
                    Req &r = req[id];
                    if (r.finished || r.stage != ST_OUT_TODO) continue;
                    if (r.remote != groupRemote) continue;
                    if (r.seen == frameId) continue;   // 同一请求只收集一次
                    r.seen = frameId;
                    mem.push_back(id);
                }
                if (!mem.empty()) {
                    string s = "E D PRE -1 " + to_string(mem.size());
                    for (int id : mem) { s += " " + to_string(id); req[id].stage = ST_D_PRE_INF; }
                    out.push_back(s);
                    localBusy = true;
                    localUsed = true;
                }
            }
        }

        // ============ remote: 优先级 P PROC > D PROC(组) ============
        for (int k = 0; k < K; ++k) {
            if (remoteBusy[k] || remoteUsed[k]) continue;
            // P PROC:该 remote 的第一个输入处理任务(降 TDR)
            int pid = -1;
            for (size_t j = hPProc; j < qPProc.size(); ++j) {
                int id = qPProc[j];
                Req &r = req[id];
                if (r.finished || r.stage != ST_P_PROC_TODO) continue;
                if (r.remote != k) continue;
                pid = id;
                break;
            }
            if (pid >= 0) {
                out.push_back("C" + to_string(k) + " P PROC 0 " + to_string(num_layers) +
                              " " + to_string(k) + " " + to_string(pid));
                req[pid].stage = ST_P_PROC_INF;
                remoteBusy[k] = true;
                remoteUsed[k] = true;
                continue;
            }
            // D PROC:该 remote 的待处理请求合成一组(同请求去重,大小 ≤ G)
            vector<int> mem;
            for (size_t j = hDProc; j < qDProc.size() && (int)mem.size() < G; ++j) {
                int id = qDProc[j];
                Req &r = req[id];
                if (r.finished || r.stage != ST_D_PROC_TODO) continue;
                if (r.remote != k) continue;
                if (r.seen == frameId) continue;   // 同一请求只收集一次
                r.seen = frameId;
                mem.push_back(id);
            }
            if (!mem.empty()) {
                string s = "C" + to_string(k) + " D PROC " + to_string(k) + " " + to_string(mem.size());
                for (int id : mem) { s += " " + to_string(id); req[id].stage = ST_D_PROC_INF; }
                out.push_back(s);
                remoteBusy[k] = true;
                remoteUsed[k] = true;
            }
            // 推进 head:跳过已分配(finished 或 stage 已变化)的条目
            while (hDProc < qDProc.size() &&
                   (req[qDProc[hDProc]].finished || req[qDProc[hDProc]].stage != ST_D_PROC_TODO)) ++hDProc;
        }

        cout << out.size() << '\n';
        for (auto &s : out) cout << s << '\n';
        cout << flush;
    }
    return 0;
}

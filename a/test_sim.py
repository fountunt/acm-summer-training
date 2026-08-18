# 模拟 Codeforces 2251A 交互器,驱动 2251A.exe,并校验其输出协议合法性。
# 用法: python test_sim.py [K] [R] [seed] [mode]
#   mode: 1=示例评分档(dist_base=0) 2=宽松档(dist_base=0.6) 默认 1
import sys, subprocess, heapq, random, os, math

K = int(sys.argv[1]) if len(sys.argv) > 1 else 2
R = int(sys.argv[2]) if len(sys.argv) > 2 else 6
SEED = int(sys.argv[3]) if len(sys.argv) > 3 else 1
MODE = int(sys.argv[4]) if len(sys.argv) > 4 else 1
EXE = sys.argv[5] if len(sys.argv) > 5 else "./2251A.exe"
random.seed(SEED)

S = 1.0
latency = 2.0
bandwidth = 1.0
bytes_per_token = 125000
num_layers = 16
MAXB = 64

if MODE == 1:
    # 示例评分参数:dist_base=0(等待组件为 0/1)
    SLO1, SLO2, tp_UB, tp_base, dist_base, w_tp, w_c = \
        30.0, 15.0, 0.0625, 0.022222222, 0.0, 0.5, 0.5
elif MODE == 2:
    SLO1, SLO2, tp_UB, tp_base, dist_base, w_tp, w_c = \
        30.0, 15.0, 0.0625, 0.022222222, 0.6, 0.5, 0.5
elif MODE == 3:
    # 宽松档:等待目标更现实
    SLO1, SLO2, tp_UB, tp_base, dist_base, w_tp, w_c = \
        200.0, 50.0, 0.0625, 0.022222222, 0.6, 0.5, 0.5
else:
    # 任务时长随组大小超线性增长(检验最优组大小 G 逻辑)
    SLO1, SLO2, tp_UB, tp_base, dist_base, w_tp, w_c = \
        200.0, 50.0, 0.0625, 0.022222222, 0.6, 0.5, 0.5

def dpre_t(bs): return 1.0 + 0.01 * bs
def dproc_t(bs): return 4.0 + 0.01 * bs * bs
def dpost_t(bs): return 1.0 + 0.01 * bs

# 任务时间表:batch_size 1..MAXB
def row(bs):
    if MODE == 4:
        return f"{bs} 3.000000000 10.000000000 2.000000000 {dpre_t(bs):.9f} {dproc_t(bs):.9f} {dpost_t(bs):.9f}"
    return f"{bs} 3.000000000 10.000000000 2.000000000 1.000000000 4.000000000 1.000000000"
table = "\n".join(row(bs) for bs in range(1, MAXB + 1))

def tt(batch, which):
    # 输入阶段任务时长(组大小 = lin,固定)
    if which == "pre":
        return 3.0
    if which == "proc":
        return 10.0
    if which == "post":
        return 2.0
    # 输出阶段任务时长(组大小 = m)
    if MODE == 4:
        if which == "dpre":
            return dpre_t(batch)
        if which == "dproc":
            return dproc_t(batch)
        return dpost_t(batch)
    if which == "dpre":
        return 1.0
    if which == "dproc":
        return 4.0
    return 1.0

def transfer_time(nbytes):
    return latency + 8.0 * nbytes / (bandwidth * 1e6)

# 请求生成
reqs = {}
arr_times = []
for i in range(R):
    lin = random.randint(1, 16)
    lout = random.randint(1, 12)
    arr = i * 0.7
    reqs[i] = {"lin": lin, "lout": lout, "arr": arr, "remote": -1,
               "stage": 0, "iter": 0, "busy": False, "finished": False,
               "pending_fin": False, "ready_time": None, "token_times": []}
    arr_times.append((arr, i))

# stage:0 未ARR 1 等P PRE 2 等UP1 3 等P PROC 4 等DOWN1 5 等P POST
#       6 等D PRE(ready) 7 等UPn 8 等D PROC 9 等DOWNN 10 等D POST 11 finished

# 事件堆:(time, seq, kind, data);kind: ARR/TDN/XDN;seq 保证稳定顺序
heap = []
seqc = 0

def push(t, kind, data):
    global seqc
    heapq.heappush(heap, (t, seqc, kind, data))
    seqc += 1

for arr, i in arr_times:
    push(arr, "ARR", i)

up_q = []   # 输入/输出 UP 传输 FIFO: (排队完成时刻链)
down_q = []

local_busy = False
remote_busy = [False] * K

viol = []

def check(cond, msg):
    if not cond:
        viol.append(msg)
        if os.environ.get("TRACE"):
            print(f"[VIOL-DBG] t={t} evs={evs}")
            print(f"[VIOL-DBG] assigns={assigns}")
            for i in range(min(5, R)):
                print(f"[VIOL-DBG] req{i}={reqs[i]}")

proc = subprocess.Popen([EXE], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)

def send(line):
    proc.stdin.write(line + "\n")

# 启动配置
send(f"{K} 1.000000000 2.000000000 1.000000000 125000 {num_layers}")
send(f"{SLO1:.9f} {SLO2:.9f} {tp_UB:.9f} {tp_base:.9f} {dist_base:.9f} {w_tp:.9f} {w_c:.9f}")
send(str(MAXB))
for bs in range(1, MAXB + 1):
    send(row(bs))
proc.stdin.flush()

def read_response():
    line = proc.stdout.readline()
    if not line:
        return None
    n = int(line.strip())
    lines = []
    for _ in range(n):
        lines.append(proc.stdout.readline().strip())
    return n, lines

def queue_transfer(tdn_time, direction, nbytes, kind, remote, rids):
    # 传输在 tdn_time 排队;FIFO
    q = up_q if direction == "UP" else down_q
    start = tdn_time
    if q and q[-1][0] > start:
        start = q[-1][0]
    dur = transfer_time(nbytes)
    fin = start + dur
    q.append((fin, direction, nbytes, kind, remote, rids))
    push(fin, "XDN", (direction, kind, remote, rids))

# 主循环
t = 0.0
frames = 0
finished_cnt = 0
while heap:
    # 收集同一时刻的所有事件(容差 1e-7)
    t = heap[0][0]
    if os.environ.get("TRACE2"):
        print(f"[TRACE2] next t={t} heap={len(heap)} upq={up_q} downq={down_q}", flush=True)
    evs = []
    while heap and abs(heap[0][0] - t) < 1e-7:
        _, _, kind, data = heapq.heappop(heap)
        evs.append((kind, data))
    evs.sort(key=lambda x: x[0])  # 同帧顺序:ARR/TDN/XDN/FIN 按类型稳定即可

    # 应用事件(推进状态),并构造帧文本
    ev_lines = []
    for kind, data in evs:
        if kind == "ARR":
            rid = data
            reqs[rid]["stage"] = 1
            ev_lines.append(f"ARR {rid} {reqs[rid]['lin']}")
        elif kind == "TDN":
            server, task, rid = data
            if server == "E":
                local_busy = False
            else:
                remote_busy[int(server[1:])] = False
            r = reqs[rid]
            r["busy"] = False
            w1s, w2s = task.split()[0], task.split()[1]
            if w1s == "P" and w2s == "PRE":
                r["stage"] = 2
            elif w1s == "P" and w2s == "PROC":
                r["stage"] = 4
            elif w1s == "P" and w2s == "POST":
                r["stage"] = 6
                if r["ready_time"] is None:
                    r["ready_time"] = t
            elif w1s == "D" and w2s == "PRE":
                parts = task.split()
                m = int(parts[3])
                for rid in parts[4:4 + m]:
                    reqs[int(rid)]["stage"] = 7
            elif w1s == "D" and w2s == "PROC":
                parts = task.split()
                m = int(parts[3])
                for rid in parts[4:4 + m]:
                    reqs[int(rid)]["stage"] = 9
            else:  # D POST
                parts = task.split()
                m = int(parts[3])
                for rid in parts[4:4 + m]:
                    rr = reqs[int(rid)]
                    rr["token_times"].append(t)
                    if rr["pending_fin"]:
                        rr["finished"] = True
                        rr["stage"] = 11
                        ev_lines.append(f"FIN {int(rid)}")
                    else:
                        rr["stage"] = 6
            ev_lines.append(f"TDN {server} {task} 1.000000000")
        elif kind == "XDN":
            direction, kkind, remote, rids = data
            if len(set(rids)) != len(rids):
                print(f"[DUP-XDN] t={t} {direction} {kkind} rids={rids}", flush=True)
            for rid in rids:
                r = reqs[rid]
                if kkind == "PRE":
                    if direction == "UP":
                        r["stage"] = 3
                    else:
                        r["stage"] = 5
                else:
                    if direction == "UP":
                        r["stage"] = 8
                    else:
                        r["stage"] = 10
            kind_s = "PRE" if kkind == "PRE" else "DEC"
            size = sum(reqs[rid]["lin"] if kkind == "PRE" else 1 for rid in rids) * bytes_per_token
            m = len(rids)
            ev_lines.append(f"XDN {direction} {remote} {size} {kind_s} {m} " + " ".join(map(str, rids)))
    lines = [f"{t:.9f}", str(len(ev_lines))] + ev_lines
    frames += 1

    # 发送帧
    for l in lines:
        send(l)
    proc.stdin.flush()

    # 读响应并校验
    resp = read_response()
    check(resp is not None, f"frame@{t}: 程序提前退出")
    if resp is None:
        break
    n, assigns = resp
    if os.environ.get("TRACE"):
        print(f"[TRACE] t={t} evs={evs} resp_n={n} assigns={assigns}", flush=True)
        if os.environ.get("TRACE2"):
            print(f"[Q] up_tail={up_q[-1][0] if up_q else None} down_tail={down_q[-1][0] if down_q else None} "
                  f"up_len={len(up_q)} down_len={len(down_q)}", flush=True)
    check(0 <= n <= K + 1, f"frame@{t}: n={n} 超出范围")
    used_rids = set()
    for a in assigns:
        toks = a.split()
        server = toks[0]
        w1, w2 = toks[1], toks[2]
        task_spec = " ".join(toks[1:])
        # 解析
        if server == "E":
            check(not local_busy, f"frame@{t}: local 忙仍分配 {a}")
            local_busy = True
            srv = "E"
        else:
            k = int(server[1:])
            check(0 <= k < K, f"frame@{t}: 非法 server {server}")
            check(not remote_busy[k], f"frame@{t}: C{k} 忙仍分配 {a}")
            remote_busy[k] = True
            srv = f"C{k}"
        if w1 == "P" and w2 == "PRE":
            remote, rid = int(toks[3]), int(toks[4])
            r = reqs[rid]
            check(rid not in used_rids, f"frame@{t}: rid {rid} 重复")
            used_rids.add(rid)
            check(0 <= remote < K, f"frame@{t}: P PRE remote 越界")
            check(r["stage"] == 1, f"frame@{t}: P PRE 前置非法 stage={r['stage']} rid={rid}")
            r["remote"] = remote
            r["busy"] = True
            tdn_t = t + S + tt(r["lin"], "pre")
            push(tdn_t, "TDN", (srv, task_spec, rid))
            queue_transfer(tdn_t, "UP", r["lin"] * bytes_per_token, "PRE", remote, [rid])
        elif w1 == "P" and w2 == "PROC":
            ls, le, remote, rid = int(toks[3]), int(toks[4]), int(toks[5]), int(toks[6])
            r = reqs[rid]
            check(rid not in used_rids, f"frame@{t}: rid {rid} 重复")
            used_rids.add(rid)
            check(remote == r["remote"], f"frame@{t}: P PROC remote 不一致")
            check(ls == 0 and le == num_layers, f"frame@{t}: piece 范围非法")
            check(r["stage"] == 3, f"frame@{t}: P PROC 前置非法 stage={r['stage']} rid={rid}")
            r["busy"] = True
            tdn_t = t + S + tt(r["lin"], "proc")
            push(tdn_t, "TDN", (srv, task_spec, rid))
            queue_transfer(tdn_t, "DOWN", r["lin"] * bytes_per_token, "PRE", remote, [rid])
        elif w1 == "P" and w2 == "POST":
            remote, rid = int(toks[3]), int(toks[4])
            r = reqs[rid]
            check(rid not in used_rids, f"frame@{t}: rid {rid} 重复")
            used_rids.add(rid)
            check(remote == r["remote"], f"frame@{t}: P POST remote 不一致")
            check(r["stage"] == 5, f"frame@{t}: P POST 前置非法 stage={r['stage']} rid={rid}")
            r["busy"] = True
            tdn_t = t + S + tt(r["lin"], "post")
            push(tdn_t, "TDN", (srv, task_spec, rid))
        elif w1 == "D" and w2 == "PRE":
            minus1, m = int(toks[3]), int(toks[4])
            rids = [int(x) for x in toks[5:5 + m]]
            check(minus1 == -1, f"frame@{t}: D PRE 缺 -1")
            check(m >= 1, f"frame@{t}: D PRE m<1")
            for rid in rids:
                r = reqs[rid]
                check(rid not in used_rids, f"frame@{t}: rid {rid} 重复")
                used_rids.add(rid)
                check(not r["finished"], f"frame@{t}: 已 FIN 请求被调度")
                check(r["stage"] == 6, f"frame@{t}: D PRE 前置非法 stage={r['stage']} rid={rid}")
                r["busy"] = True
                tdn_t = t + S + tt(m, "dpre")
            push(tdn_t, "TDN", (srv, task_spec, rids[0]))
            # 每个 distinct remote 一条 UP,长度 = 该 remote 成员数
            by_remote = {}
            for rid in rids:
                by_remote.setdefault(reqs[rid]["remote"], []).append(rid)
            for kk, mem in by_remote.items():
                queue_transfer(tdn_t, "UP", len(mem) * bytes_per_token, "DEC", kk, mem)
        elif w1 == "D" and w2 == "PROC":
            remote, m = int(toks[3]), int(toks[4])
            rids = [int(x) for x in toks[5:5 + m]]
            check(m >= 1, f"frame@{t}: D PROC m<1")
            for rid in rids:
                r = reqs[rid]
                check(rid not in used_rids, f"frame@{t}: rid {rid} 重复")
                used_rids.add(rid)
                check(r["remote"] == remote, f"frame@{t}: D PROC remote 不一致")
                check(r["stage"] == 8, f"frame@{t}: D PROC 前置非法 stage={r['stage']} rid={rid}")
                r["busy"] = True
                tdn_t = t + S + tt(m, "dproc")
            push(tdn_t, "TDN", (srv, task_spec, rids[0]))
            queue_transfer(tdn_t, "DOWN", m * bytes_per_token, "DEC", remote, rids)
        elif w1 == "D" and w2 == "POST":
            minus1, m = int(toks[3]), int(toks[4])
            rids = [int(x) for x in toks[5:5 + m]]
            check(minus1 == -1, f"frame@{t}: D POST 缺 -1")
            check(m >= 1, f"frame@{t}: D POST m<1")
            if os.environ.get("TRACE"):
                for rid in rids:
                    print(f"[DPOST] rid={rid} iter={reqs[rid]['iter']} lout={reqs[rid]['lout']} "
                          f"stage={reqs[rid]['stage']} fin={reqs[rid]['finished']}", flush=True)
            for rid in rids:
                r = reqs[rid]
                check(rid not in used_rids, f"frame@{t}: rid {rid} 重复")
                used_rids.add(rid)
                check(not r["finished"], f"frame@{t}: 已 FIN 请求被调度")
                check(r["stage"] == 10, f"frame@{t}: D POST 前置非法 stage={r['stage']} rid={rid}")
                r["busy"] = True
                r["iter"] += 1
                if r["iter"] == r["lout"]:
                    r["pending_fin"] = True
                tdn_t = t + S + tt(m, "dpost")
            push(tdn_t, "TDN", (srv, task_spec, rids[0]))

proc.stdin.write("END\n")
proc.stdin.flush()
proc.stdin.close()
proc.wait(timeout=30)

# 汇总
all_fin = all(reqs[i]["finished"] for i in range(R))
print(f"K={K} R={R} seed={SEED} frames={frames} all_finished={all_fin} violations={len(viol)}")
if viol:
    for v in viol[:20]:
        print("  VIOL:", v)

# ---- 评分 ----
def clamp(x, base, target):
    return max(0.0, min(1.0, (x - base) / (target - base)))

if all_fin:
    total_tokens = sum(reqs[i]["lout"] for i in range(R))
    first_arr = min(reqs[i]["arr"] for i in range(R))
    last_token = max(max(reqs[i]["token_times"]) for i in range(R))
    tp = total_tokens / (last_token - first_arr)
    tdr = sum(reqs[i]["ready_time"] - reqs[i]["arr"] for i in range(R)) / R
    gaps = []
    for i in range(R):
        ts = reqs[i]["token_times"]
        for a, b in zip(ts, ts[1:]):
            gaps.append(b - a)
    tpot = sum(gaps) / len(gaps) if gaps else 0.0
    excess_tdr = max(0.0, (tdr - SLO1) / SLO1)
    excess_tpot = max(0.0, (tpot - SLO2) / SLO2)
    dist = math.sqrt(excess_tdr ** 2 + excess_tpot ** 2)
    comp_tp = clamp(tp, tp_base, tp_UB)
    if dist_base > 0:
        comp_w = max(0.0, 1.0 - dist / dist_base)
    else:
        comp_w = 1.0 if dist == 0.0 else 0.0
    score = 1000.0 * (w_tp * comp_tp + w_c * comp_w)
    print(f"SCORE={score:.1f}  tp={tp:.4f}(comp {comp_tp:.3f}) tdr={tdr:.2f} tpot={tpot:.2f} "
          f"dist={dist:.4f}(comp {comp_w:.3f}) last_token={last_token:.1f}")
    sys.exit(1 if (viol or not all_fin) else 0)
sys.exit(1 if (viol or not all_fin) else 0)

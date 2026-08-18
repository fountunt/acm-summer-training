import sys, itertools, random

def brute(n, p):
    """p: 1-indexed permutation of length n; lamp (i, p[i]) lit at t=0.
    Returns earliest time all cells lit, or -1."""
    t = [[-1]*n for _ in range(n)]
    for i in range(n):
        t[i][p[i]-1] = 0
    cnt = n
    cur = 0
    while cnt < n*n:
        nxt = []
        for i in range(n):
            ti = t[i]
            for j in range(n):
                if ti[j] < 0:
                    c = 0
                    if i > 0 and t[i-1][j] >= 0: c += 1
                    if i+1 < n and t[i+1][j] >= 0: c += 1
                    if j > 0 and ti[j-1] >= 0: c += 1
                    if j+1 < n and ti[j+1] >= 0: c += 1
                    if c >= 2:
                        nxt.append((i, j))
        if not nxt:
            return -1
        cur += 1
        for i, j in nxt:
            t[i][j] = cur
        cnt += len(nxt)
    return cur


def gen_separable(n):
    """Generate all separable permutations of size n (as lists of ints 1..n)."""
    results = []
    def rec(k):
        # returns list of separable permutations of size k
        if k == 1:
            return [[1]]
        out = []
        # split k into >= 2 parts
        def splits(total, parts):
            if total == 0:
                yield parts
                return
            for first in range(1, total + 1):
                yield from splits(total - first, parts + [first])
        for parts in splits(k, []):
            if len(parts) < 2:
                continue
            # generate each part's permutation
            per_part = [rec(sz) for sz in parts]
            for combo in itertools.product(*per_part):
                # plus: concatenate shifted
                plus = []
                acc = 0
                for part in combo:
                    plus.extend([x + acc for x in part])
                    acc += len(part)
                out.append(plus)
                # minus: first part gets highest values
                minus = []
                acc = k
                for part in combo:
                    minus.extend([x + acc - len(part) for x in part])
                    acc -= len(part)
                out.append(minus)
        # dedupe
        seen = set()
        res = []
        for p in out:
            key = tuple(p)
            if key not in seen:
                seen.add(key)
                res.append(p)
        return res
    return rec(n)


def solve_stack(p):
    """Port of F.cpp stack algorithm (five-tuple + dm flattening)."""
    n = len(p)
    st = []  # dicts
    for i in range(1, n+1):
        v = p[i-1]
        b = {'sign': 0, 'lo': v, 'hi': v, 'R0': i, 'C0': v, 'H': 1,
             'f': 0, 'A': i-v, 'B': v-i, 'C': i+v, 'D': -(i+v),
             'dmf': 0, 'dmA': i-v, 'dmB': v-i, 'dmC': i+v, 'dmD': -(i+v)}
        st.append(b)
        while len(st) >= 2:
            P, Q = st[-2], st[-1]
            if P['hi'] + 1 == Q['lo']:
                s = 1
            elif Q['hi'] + 1 == P['lo']:
                s = -1
            else:
                break
            def ctf(X):
                if X['sign'] == s:
                    return X['dmf'], X['dmA'], X['dmB'], X['dmC'], X['dmD']
                return X['f'], X['A'], X['B'], X['C'], X['D']
            P_f, P_A, P_B, P_C, P_D = ctf(P)
            Q_f, Q_A, Q_B, Q_C, Q_D = ctf(Q)
            mf = max(P_f, Q_f); mA = max(P_A, Q_A); mB = max(P_B, Q_B)
            mC = max(P_C, Q_C); mD = max(P_D, Q_D)
            N = {'sign': s, 'R0': P['R0'], 'H': P['H'] + Q['H']}
            if s == 1:
                N['lo'] = P['lo']; N['hi'] = Q['hi']; N['C0'] = P['C0']
                d1 = (N['H']-1) + N['C0'] - N['R0']
                d2 = (N['H']-1) + N['R0'] - N['C0']
                N['f'] = max(mf, mA + d1, mB + d2)
                N['A'] = max(mA, 2*d2 + mB)
                N['B'] = max(mB, 2*d1 + mA)
                N['C'] = max(mC, 2*(N['C0']+N['H']-1) + mA, 2*(N['R0']+N['H']-1) + mB)
                N['D'] = max(mD, mA - 2*N['R0'], mB - 2*N['C0'])
            else:
                N['lo'] = Q['lo']; N['hi'] = P['hi']; N['C0'] = Q['C0']
                base = N['R0'] + N['C0']
                N['f'] = max(mf, mC - base, base + 2*(N['H']-1) + mD)
                N['A'] = max(mA, mC - 2*N['C0'], 2*(N['R0']+N['H']-1) + mD)
                N['B'] = max(mB, mC - 2*N['R0'], 2*(N['C0']+N['H']-1) + mD)
                N['C'] = max(mC, 2*(base + 2*(N['H']-1)) + mD)
                N['D'] = max(mD, mC - 2*base)
            N['dmf'] = mf; N['dmA'] = mA; N['dmB'] = mB; N['dmC'] = mC; N['dmD'] = mD
            st.pop(); st.pop(); st.append(N)
    if len(st) != 1:
        return -1
    return st[0]['f']


if __name__ == '__main__':
    # samples
    samples = [
        (4, [1,2,3,4], 3),
        (4, [2,1,4,3], 4),
        (4, [2,4,1,3], -1),
    ]
    for n, p, exp in samples:
        b = brute(n, p)
        s = solve_stack(p)
        print(f"n={n} p={p} brute={b} stack={s} expected={exp}",
              "OK" if b == exp == s else "MISMATCH")

    # exhaustive separable test
    for n in range(2, 8):
        perms = gen_separable(n)
        bad = 0
        for p in perms:
            b = brute(n, p)
            s = solve_stack(p)
            if b != s:
                bad += 1
                print(f"MISMATCH n={n} p={p} brute={b} stack={s}")
                if bad > 5:
                    break
        print(f"n={n}: {len(perms)} separable perms tested, mismatches={bad}")

    # random non-separable check
    random.seed(123)
    bad = 0
    for n in range(2, 9):
        for _ in range(200):
            p = list(range(1, n+1))
            random.shuffle(p)
            b = brute(n, p)
            s = solve_stack(p)
            if b != s:
                bad += 1
                print(f"MISMATCH random n={n} p={p} brute={b} stack={s}")
                break
        if bad:
            break
    print("random non-separable check done, mismatches =", bad)

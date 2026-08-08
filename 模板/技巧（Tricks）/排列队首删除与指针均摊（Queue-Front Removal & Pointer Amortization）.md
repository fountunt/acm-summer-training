# 排列队首删除与指针均摊（Queue-Front Removal & Pointer Amortization）

## 概念

有一类博弈/构造题：两个排列（或两个有序列表）P、Q，每轮取**两边各自的队首**比较，删除其一（被删者从两份列表中同时移除，剩余相对顺序不变）。要判断能否按某种顺序删到只剩目标，并输出删除顺序。

**关键观察**：
1. 状态只需记 `(i, j)` = 两边第一个未删者的下标。
2. 删除 P 队首时，P 指针会**跳**到下一个未删位置（中间可能跳过已被 Q 侧删除的选手）——指针只前移，**每个位置最多被跳过/扫过一次，整体均摊 O(n)**。
3. 别每轮从开头重扫找队首（会 O(n²) 超时），要用"dead 标记 + 单方向光标"。

---

## 通用模板：双列表同步删除的模拟 — O(n)

```cpp
// P、Q 是两个排列（1..n），x 为目标幸存者；返回删除顺序，无法完成返回空
vector<int> remove_to_keep(int n, int x,
                           const vector<int>& P, const vector<int>& Q,
                           const vector<int>& posP, const vector<int>& posQ){
    vector<char> dead(n+1, 0);
    vector<int> order;
    int pi = 1, qi = 1;
    while (true) {
        while (pi <= n && dead[P[pi]]) ++pi;   // 光标只前移，跳过已删
        while (qi <= n && dead[Q[qi]]) ++qi;
        if (pi > n || qi > n) return {};       // 越界：无解
        int u = P[pi], v = Q[qi];
        if (u == v) {                          // 两边队首相同：僵局或只剩 x
            if (u == x) break;                 // 只剩冠军，结束
            return {};                         // 僵局，无解
        }
        int take;
        if (u == x) take = v;                  // x 已到队首 → 删另一边
        else if (v == x) take = u;
        else if (posQ[u] < posP[v]) take = u;  // 删"对象位置更靠前"的队首
        else take = v;
        order.push_back(take);
        dead[take] = 1;
    }
    return order;
}
```

> 复杂度 O(n)：两个光标各自只前进，每个位置至多被扫一次；别用"每轮重找队首"。

---

## 判定技巧：前缀集合不相等 ⟺ 前缀最大值 == i

这类题常有一个充要条件："P 的前 i 个位置与 Q 的前 i 个位置**不是同一群选手**（对所有 i<n）"。O(n) 判定：

```
s[i] = posQ(P[i])          // P[i] 在 Q 中的位置
对 i=1..n-1，维护 pmax = max(s[1..i])
若某时刻 pmax == i  ⟺  P 前 i 个恰好占了 Q 前 i 个位置（成闭环）→ 无解
```

因为 s 是排列，`max(s[1..i]) ≥ i` 恒成立，所以"≤ i" 即 "== i"。

---

## 应用示例：两排列队首博弈，目标 x 夺冠（牛客暑期多校 D 题）

模型：每轮两边第一个未淘汰者比赛，淘汰其一；若两边队首相同则僵局。问能否使 x 夺冠并输出淘汰顺序。

**充要条件（已暴力验证）**：
1. 对每个 `i < n`：`{P1..Pi} ≠ {Q1..Qi}`（用上面"前缀最大值==i"O(n) 判断）；
2. 不存在 `y ≠ x` 同时满足 `posP(y) > posP(x)` 且 `posQ(y) > posQ(x)`（即右下角区域只有 x）。

**构造（贪心）**：冠军 x 到队首后始终删另一边；否则删"对象初始位置更靠前"的队首（`posQ(u) vs posP(v)` 小者）。该贪心保持两条充要条件始终成立，从而永不相撞、最终只剩 x。

```cpp
// 条件 2：右下角区域只有 x
for (int w = 1; w <= n; ++w)
    if (posP[w] > posP[x] && posQ[w] > posQ[x]) { /* NO */ }
// 条件 1：无前缀闭环
int pmax = 0;
for (int i = 1; i < n; ++i) {
    pmax = max(pmax, posQ[P[i]]);
    if (pmax == i) { /* NO */ }
}
// 之后用上面的 remove_to_keep 贪心模拟即可
```

复杂度：O(n)，空间 O(n)。

---

## 要点

| 要点 | 说明 |
|------|------|
| 指针只前移 | 用 dead 标记 + 单调光标，每轮"找队首"均摊 O(1)，总 O(n) |
| 千万别重扫 | 每轮从开头找队首是 O(n²)，必超时 |
| 队首相同 | 可能是"只剩目标"（结束）也可能是僵局（无解），按目标是否=队首区分 |
| 前缀闭环判定 | `max(posQ(P[1..i])) == i` ⟺ P、Q 前 i 个是同群选手 |
| 识别特征 | 两个排列/列表、反复取队首比较或删除、问能否删到只剩某个元素 |

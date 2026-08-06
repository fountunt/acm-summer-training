# Z 函数（扩展 KMP）

## 概念

对于字符串 `s`，定义 `Z[i]` = `s` 与它的后缀 `s[i..]` 的**最长公共前缀长度**（`i` 从 1 开始）。
利用「Z-box」在 O(n) 内求出全部 `Z[i]`。

**主要用途**：
- 判断一个串是否包含另一个串（模式匹配，等价于 KMP）。
- **比较串与它的每个后缀**：`s` 与 `s[i..]` 首次不同的位置就是 `Z[i]`（若 `Z[i] < n-i`）。
  这是许多字符串题（如本模板来源的「字母表」题）的关键一步。
- 求周期 / 边界结构。

---

## 代码 — O(n)

```cpp
// 输入 s，输出 Z[0..n-1]，其中 Z[i] 是 s 与后缀 s[i..] 的 LCP 长度
vector<int> z_function(const string& s) {
    int n = (int)s.size();
    vector<int> Z(n, 0);
    int l = 0, r = 0;               // 当前 Z-box [l, r]（r 为已匹配的最右端点）
    for (int i = 1; i < n; ++i) {
        if (i <= r) Z[i] = min(r - i + 1, Z[i - l]);  // 在 box 内，直接利用已算好的 Z
        while (i + Z[i] < n && s[Z[i]] == s[i + Z[i]]) ++Z[i];  // 暴力扩展
        if (i + Z[i] - 1 > r) { l = i; r = i + Z[i] - 1; }      // 更新 box
    }
    return Z;
}
```

> 边界：`Z[0]` 习惯上定义为 0（也有定义为 n 的，按需处理）。若某后缀是整个串的前缀，即 `Z[i] == n - i`，则 `s[i..]` 是 `s` 的一个真前缀。

---

## 经典应用：比较 s 与所有后缀 / 提取"首次不同"约束

很多问题需要判断 `s` 是否比它的每个后缀都小（Lyndon 条件）。利用 Z 函数：

```cpp
// 对每个 p，比较 s 与后缀 s[p..]，返回首次不同的位置；若后缀是前缀则返回 -1
// 需要 s 与 s[p..] 在位置 k = Z[p] 首次不同，比较 s[k] 与 s[p+k] 即可
void compare_suffixes(const string& s) {
    int n = (int)s.size();
    vector<int> Z = z_function(s);
    for (int p = 1; p < n; ++p) {
        if (Z[p] == n - p) {           // s[p..] 是 s 的前缀 → s 比该后缀大，条件失败
            // 处理无解情况
        } else {
            // s[Z[p]] != s[p + Z[p]]，首次不同在此处
            // 若要 s < s[p..]，需要字母 s[Z[p]] 排在 s[p+Z[p]] 之前（依据具体题目的序）
        }
    }
}
```

---

## 模板题应用示例

> 来源：牛客多校「字母表」。给定串 s，统计有多少种字母表使 s 是 Lyndon 串。
> 思路：Lyndon ⟺ s 比所有真后缀都小。用 Z 函数收集「字母 c1 必须排在 c2 前」的约束，
> 转成偏序后数线性扩展（配合子集 DP，见 DP 模板「子集DP统计线性扩展」）。

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; cin >> s;
    int n = (int)s.size();
    vector<int> Z = z_function(s);

    unsigned pred[26] = {0};          // pred[x]：必须排在 x 前面的字母集合
    for (int p = 1; p < n; ++p) {
        if (Z[p] == n - p) { cout << 0 << "\n"; return 0; }   // 后缀是前缀 → 无解
        int a = s[Z[p]] - 'a', b = s[p + Z[p]] - 'a';
        pred[b] |= (1u << a);                                  // a 必须在 b 前
    }
    // 之后交给"数线性扩展"的子集 DP……
}
```

---

## 注意事项

| 要点 | 说明 |
|------|------|
| Z-box 更新 | 只有当 `i + Z[i] - 1 > r` 时才移动 `l, r`，保证摊还 O(n) |
| `Z[0]` | 约定为 0；若需用它比较，注意特判 |
| 判断后缀为前缀 | `Z[p] == n - p` ⟺ `s[p..]` 是 `s` 的前缀 |
| 与 KMP 的关系 | 求模式匹配时 Z 函数与 KMP 等价，二者可互相替代 |
| 存 Z 数组 | 求 Z 时用到 `Z[i-l]`，需要保留已算出的 Z 值 |

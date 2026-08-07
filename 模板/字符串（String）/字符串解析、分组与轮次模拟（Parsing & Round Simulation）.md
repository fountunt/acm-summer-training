# 字符串解析、分组与轮次模拟

## 概念

很多字符串题的核心并不在字符串算法（KMP / Z / AC 自动机），而在四件小事：
1. **正确解析输入**——尤其 `cin >> int` 与 `getline` 混用时残留换行符的坑；
2. **字符串加工**——按空格拆词、提取首字母、大小写处理；
3. **分组判重**——用 `map` / `set` 统计出现次数，找出「共用同一串」的冲突；
4. **轮次模拟**——反复迭代直到满足终止条件，且每轮所有对象「同时」更新。

> 本文以牛客多校「机构代号 D-Mail」为贯穿例子：给若干机构名，初始缩写取各单词首字母，
> 每轮把仍与其他机构冲突的缩写「完整写出下一个单词」，直到所有缩写两两不同。

---

## 常用函数速查

| 类别 | 函数 | 说明 |
|------|------|------|
| 输入 | `getline(cin, s)` | 读一整行（含空格） |
| 输入 | `cin.ignore()` | 丢弃残留换行符（`cin >> n` 后必须加） |
| 输入 | `stringstream ss(s); ss >> w` | 按空白符拆词 |
| 字符分类 | `isupper(c)` / `islower(c)` | 是否大写 / 小写字母 |
| 字符分类 | `isdigit(c)` / `isalpha(c)` / `isspace(c)` | 数字 / 字母 / 空白 |
| 字符转换 | `toupper(c)` / `tolower(c)` | 转大写 / 小写（注意返回 `int`，赋回 `char` 前要转） |
| 子串 | `s.substr(pos, len)` | 截取子串 |
| 查找 | `s.find(t)` | 找子串位置，失败返回 `string::npos` |
| 转换 | `stoi(s)` / `stoll(s)` / `to_string(x)` | 字符串 ↔ 数字 |
| 增删 | `s += c` / `s.erase(pos, len)` | 追加 / 删除 |
| 分组 | `map<string,int> cnt` | 统计每个串出现次数 |
| 分组 | `map<string, vector<int>>` | 记录每个串对应哪些下标 |
| 去重 | `set<string> st` | 统计不同串的个数 |

---

## 1. 逐行读入（避开换行残留坑）

`cin >> n` 不会吃掉行尾换行符，紧接着 `getline` 会先读到一行空串。
**务必先 `cin.ignore()`**（或先 `getline` 掉空行），否则第一行数据丢失、下标错位。

```cpp
int n;
cin >> n;
cin.ignore();            // 丢弃 n 后面的换行，否则第一个 getline 读到空行
for (int i = 0; i < n; ++i) {
    string line;
    getline(cin, line);  // 读一整行
    // ...处理
}
```

---

## 2. 按空格拆词

```cpp
stringstream ss(line);
string w;
while (ss >> w) words.push_back(w);   // 自动跳过任意连续空白、前后空格
```

---

## 3. 首字母缩写 / 单词级加工

首字母 = 单词第一个字符；「完整写出单词」= 直接拼接整个单词。
**单字母单词完整写出时等于它自己的首字母**，缩写自然不变，无需特判。

```cpp
// 前 p 个单词完整写出，其余取首字母
string build(const vector<string>& w, int p) {
    string s;
    for (int j = 0; j < (int)w.size(); ++j)
        s += (j < p) ? w[j] : string(1, w[j][0]);
    return s;
}
```

---

## 4. 分组判重

```cpp
map<string, int> cnt;
for (int i = 0; i < n; ++i) cnt[cur[i]]++;
// 冲突 ⟺ cnt[cur[i]] > 1
```

需要「每个串对应哪些下标」时改用 `map<string, vector<int>> mp; mp[s].push_back(i);`。

> ⚠️ 字符串按字典序比较，**大小写敏感**：`"Na"` 与 `"NA"` 是不同的串。

---

## 5. 轮次模拟模式（K 题核心）

**模式**：每个对象带一个「进度」状态 `p[i]`；每轮先按当前状态生成结果、统计冲突，
再**同时**推进所有冲突对象的进度；直到无冲突。

```cpp
#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    vector<vector<string>> words(n);   // 每个机构的单词序列
    vector<int> p(n, 0);               // p[i]: 已完整写出的单词个数（进度）

    for(int i = 0; i < n; i++){
        string line;
        getline(cin, line);
        stringstream ss(line);
        string w;
        while(ss >> w) words[i].push_back(w);
    }

    auto abbr = [&](int i){            // 当前缩写
        string s;
        for(int j = 0; j < (int)words[i].size(); j++)
            s += (j < p[i]) ? words[i][j] : string(1, words[i][j][0]);
        return s;
    };

    while(true){
        map<string, int> cnt;
        vector<string> cur(n);
        for(int i = 0; i < n; i++){ cur[i] = abbr(i); cnt[cur[i]]++; }
        bool done = true;
        for(int i = 0; i < n; i++){
            if(cnt[cur[i]] > 1){                 // 本轮与其他机构共用缩写
                done = false;
                if(p[i] < (int)words[i].size()) p[i]++;   // 同时细化（进度封顶防越界）
            }
        }
        if(done) break;
    }

    for(int i = 0; i < n; i++) cout << abbr(i) << "\n";
}
```

**两个关键点**：
- **同时更新**：先统计完所有缩写、再统一推进进度——保证「本轮全部更新完成后才检查下一轮冲突」。
  不能边更新边检查，否则会把同一轮内先后发生的变化误判成下一轮的结果。
- **单字母单词**：进度照样 +1，缩写因为「完整写出 = 首字母」而自然不变，不用特判。

---

## 注意事项

| 要点 | 说明 |
|------|------|
| `cin >> n` 后必须 `cin.ignore()` | 否则 `getline` 先读到空行，第一行数据丢失/错位 |
| 拆词用 `stringstream` | 自动处理任意连续空格、前后空格 |
| 分组判重用 `map` | 按字典序比较，**大小写敏感** |
| 轮次「同时更新」 | 先统计、后推进，勿在统计循环里修改状态 |
| 输出按输入顺序 | 按 `i` 顺序输出，不要按 `map` 的分组顺序输出 |
| 进度封顶 | 若题目保证有限轮结束，以「无冲突」为退出条件；`p[i]` 封顶防止越界 |
| 字符函数注意 | `isupper(c)` 等参数传 `unsigned char` 更安全（避免负值 UB） |

---

## 变体与延伸

- 若每轮只让「冲突组」整体细化，且细化方式复杂，可改为记录 `p[i]` 而非从缩写反推，**永远不要靠字符串本身反推进度**（单字母单词、同名首字母都会使反推不可行）。
- 若统计的是「不同串个数」，用 `set<string>` 即可；若还要输出每个串的首次出现位置，用 `map<string,int>`。
- 该模式（进度状态 + 轮次同时推进）也适用于**模拟类 / 拓扑推进类**题目，不限于字符串。

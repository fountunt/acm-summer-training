#include<bits/stdc++.h>
#define all(v) v.begin(),v.end()
#define unq(v) v.erase(unique(all(v)),v.end())
#define pb push_back
#define For4(i,l,r,k) for(ll i=(l);i<=(r);i+=(k))
#define For3(i,l,r) For4(i,l,r,1)
#define overload4_for(a,b,c,d,e,...) e
#define For(...) overload4_for(__VA_ARGS__, For4, For3)(__VA_ARGS__)
#define Forr4(i,l,r,k) for(ll i=(l);i>=(r);i-=(k))
#define Forr3(i,l,r) Forr4(i,l,r,1)
#define overload4_forr(a,b,c,d,e,...) e
#define Forr(...) overload4_forr(__VA_ARGS__, Forr4, Forr3)(__VA_ARGS__)
#define dbg(a) cout<<#a<<"="<<a<<endl
#define YES std::cout << "YES\n"
#define NO std::cout << "NO\n"
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using a2 = array<ll, 2>;
using a3 = array<ll, 3>;
using a4 = array<ll, 4>;
using a5 = array<ll, 5>;

const int MOD = 1e9+7;
const ll INF = 1e18;
const ld EPS = 1e-12;
const ld PI = acos(-1.0);

struct Edge {
    int to;
    long long weight;
};

struct KEdge {
    int u, v;
    long long w;
    
    // 按权值升序排序，便于 Kruskal 贪心选择
    bool operator<(const KEdge& other) const {
        return w < other.w;
    }
};

//=================== 位运算性质与常用技巧 ===============================
// (a + b) & 1 = (a ^ b) & 1
// a & (a ^ b) = a & ~b
// a | b = (a ^ b) | (a & b)
// a | b =  a + b  − (a & b)
// a ^ b = (a | b) − (a & b)
// a + b = (a ^ b) + 2 * (a & b)​
// 取第 k 位  ：(x >> k) & 1
// 改第 k 位 1： x |  (1 << k)
// 改第 k 位 0： x & ~(1 << k)
// 第 k 位翻转： x ^  (1 << k)
// 找最低位 1 ： x & −x
// 删最低位 1 ： x & (x−1)

//=================== 内置函数说明 =======================================
// __builtin_popcount 统计二进制中 1 的个数
// __builtin_clz      前导 0（最高位前面有多少个 0）
// __builtin_ctz      后导 0（最低位连续 0 的个数）

//=================== 随机数生成器 =======================================
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
// mt19937 rng(114514);
ull base = rng();

//=================== Log2 计算 ==========================================
int Log2(int x) { // 计算log2(x)，向下取整
    return 31 - __builtin_clz(x);
}
int CLog2(int x) { // 计算log2(x)，但是向上取整
    return Log2(x) + (__builtin_popcount(x) != 1);
}

//=================== 快速幂 =============================================
ll Qpower(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

//=================== 组合数 (预处理阶乘与逆元) ==========================
struct Comb {
    int n;
    vector<ll> fact, invFact;

    Comb(int n) : n(n), fact(n + 1), invFact(n + 1) {
        fact[0] = 1;
        For(i, 1, n) fact[i] = fact[i - 1] * i % MOD;
        invFact[n] = Qpower(fact[n], MOD - 2);
        Forr(i, n - 1, 0) invFact[i] = invFact[i + 1] * (i + 1) % MOD;
    }

    // 查询 C(n, m) % MOD
    ll C(int n, int m) const {
        if (m < 0 || m > n) return 0;
        return fact[n] * invFact[m] % MOD * invFact[n - m] % MOD;
    }

    // 查询 A(n, m) % MOD 排列数
    ll A(int n, int m) const {
        if (m < 0 || m > n) return 0;
        return fact[n] * invFact[n - m] % MOD;
    }
};

//=================== 组合数 (杨辉三角) ==================================
struct PComb {
    int n;
    vector<vector<ll>> C;

    PComb(int n) : n(n), C(n + 1, vector<ll>(n + 1, 0)) {
        For(i, 0, n) {
            C[i][0] = 1;
            For(j, 1, i) {
                C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % MOD;
            }
        }
    }

    ll q(int n, int m) const {
        if (m < 0 || m > n) return 0;
        return C[n][m];
    }
};

//=================== 卢卡斯定理 (Lucas) =================================
struct Lucas {
    int p; // 模数，必须为质数  MOD <= 10^5
    vector<ll> fact, invFact;

    Lucas(int p = MOD) : p(p), fact(p), invFact(p) {
        fact[0] = 1;
        For(i, 1, p - 1) fact[i] = fact[i - 1] * i % p;
        invFact[p - 1] = Qpower(fact[p - 1], p - 2);
        Forr(i, p - 2, 0) invFact[i] = invFact[i + 1] * (i + 1) % p;
    }

    // 小范围 C(n, m) % p
    ll C_small(ll n, ll m) const {
        if (m < 0 || m > n) return 0;
        return fact[n] * invFact[m] % p * invFact[n - m] % p;
    }

    // 卢卡斯定理主函数，支持 ll 范围的 n, m
    ll query(ll n, ll m) const {
        if (m == 0) return 1;
        if (m < 0 || m > n) return 0;
        return query(n / p, m / p) * C_small(n % p, m % p) % p;
    }
};

//=================== 分数运算 ===========================================
struct Frac {
    ll a, b; // a 为分子，b 为分母 (a/b)

    // 约分 + 符号修正
    void fix() {
        if (b < 0) a = -a, b = -b;
        ll g = gcd(abs(a), b);
        if (g) a /= g, b /= g;
    }

    // 构造函数
    Frac(ll _a = 0, ll _b = 1) : a(_a), b(_b) { fix(); }

    // 重载运算符 (竞赛常用写法)
    Frac operator + (const Frac& o) const { return Frac(a * o.b + o.a * b, b * o.b); }
    Frac operator - (const Frac& o) const { return Frac(a * o.b - o.a * b, b * o.b); }
    Frac operator * (const Frac& o) const { return Frac(a * o.a, b * o.b); }
    Frac operator / (const Frac& o) const { return Frac(a * o.b, b * o.a); }

    // 比较运算符 (交叉相乘比较，注意溢出)
    bool operator < (const Frac& o) const { return a * o.b < o.a * b; }
    bool operator == (const Frac& o) const { return a == o.a && b == o.b; }
    bool operator > (const Frac& o) const { return o < *this; }
    bool operator <= (const Frac& o) const { return !(*this > o); }
    bool operator >= (const Frac& o) const { return !(*this < o); }

    // 快速转浮点数
    double to_double() const { return (double)a / b; }

    // 格式化输出
    void print() const {
        if (b == 1) cout << a;
        else cout << a << "/" << b;
    }
};

//=================== 自动取模类 Z =======================================
struct Z {
    int x;

    // 构造函数：自动将负数或超过 MOD 的数规整到 [0, MOD - 1]
    Z(ll _x = 0) {
        _x %= MOD;
        if (_x < 0) _x += MOD;
        x = _x;
    }

    // 费马小定理求逆元：x^(MOD-2) % MOD (要求 MOD 为质数)
    Z inv() const { return Qpower(x, MOD - 2); }

    // 四则运算重载
    Z operator + (const Z& o) const { return Z(x + o.x >= MOD ? x + o.x - MOD : x + o.x); }
    Z operator - (const Z& o) const { return Z(x - o.x < 0 ? x - o.x + MOD : x - o.x); }
    Z operator * (const Z& o) const { return Z(1LL * x * o.x % MOD); }
    Z operator / (const Z& o) const { return *this * o.inv(); } // 除以一个数等于乘以它的逆元

    // 复合赋值运算符
    Z& operator += (const Z& o) { return *this = *this + o; }
    Z& operator -= (const Z& o) { return *this = *this - o; }
    Z& operator *= (const Z& o) { return *this = *this * o; }
    Z& operator /= (const Z& o) { return *this = *this / o; }

    // 负号 (取反)
    Z operator - () const { return Z(x == 0 ? 0 : MOD - x); }

    // 比较与输出
    bool operator == (const Z& o) const { return x == o.x; }
    bool operator != (const Z& o) const { return x != o.x; }
    
    // 支持直接 cin >> a 或 cout << a
    friend ostream& operator << (ostream& os, const Z& a) { return os << a.x; }
    friend istream& operator >> (istream& is, Z& a) { ll t; is >> t; a = Z(t); return is; }
};

//=================== 平面几何基础 =======================================
int sign(ld x)
{
    if (fabs(x) < EPS) return 0;
    return x < 0 ? -1 : 1;
}
struct PT {
    ld x, y;
    PT(ld x = 0, ld y = 0) : x(x), y(y) {}

    PT operator + (PT b) const { return {x + b.x, y + b.y}; }
    PT operator - (PT b) const { return {x - b.x, y - b.y}; }
    PT operator * (ld k) const { return {x * k, y * k}; }
    PT operator / (ld k) const { return {x / k, y / k}; }

    bool operator < (PT b) const {
        if (sign(x - b.x) != 0) return sign(x - b.x) < 0;
        return sign(y - b.y) < 0;
    }
    bool operator == (PT b) const {return sign(x - b.x) == 0 && sign(y - b.y) == 0;}

    // ================= 补充：旋转操作 =================
    // 1. 逆时针旋转 rad 弧度
    PT rot(ld rad) const {return {x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)};}
    // 2. 逆时针旋转 90 度 (方向向量的向左法向量)
    PT rot90() const {return {-y, x};}
    // 3. 顺时针旋转 90 度 (方向向量的向右法向量)
    PT rot90_clockwise() const {return {y, -x};}
    // 4. 点 p 绕着当前点 (this) 逆时针旋转 rad 弧度
    PT rot_around(PT p, ld rad) const {return *this + (p - *this).rot(rad);}
};

// 基础向量操作
ld dot(PT a, PT b) { return a.x * b.x + a.y * b.y; }
ld cross(PT a, PT b) { return a.x * b.y - a.y * b.x; }
ld cross(PT o, PT a, PT b) { return cross(a - o, b - o); } // oa x ob

ld len(PT a) { return sqrt(dot(a, a)); }
ld dist(PT a, PT b) { return len(a - b); }

// 1. 点 p 到线段 ab 的距离
ld dist_to_segment(PT p, PT a, PT b) {
    if (sign(dot(b - a, p - a)) < 0) return dist(p, a);
    if (sign(dot(a - b, p - b)) < 0) return dist(p, b);
    return fabs(cross(a, b, p)) / dist(a, b);
}

// 2. 点 p 到直线 ab 的距离（补充：如果你需要计算直线距离）
ld dist_to_line(PT p, PT a, PT b) {
    return fabs(cross(a, b, p)) / dist(a, b);
}

// 3. 判断点 p 是否在线段 ab 上
bool on_segment(PT p, PT a, PT b) {
    return sign(cross(a, p, b)) == 0 && sign(dot(a - p, b - p)) <= 0;
}

// 4. 判断点 p 是否在直线 ab 上
bool on_line(PT p, PT a, PT b) {
    return sign(cross(a, p, b)) == 0;
}

// 5. 判线段 ab 和 cd 是否相交
bool seg_intersect(PT a, PT b, PT c, PT d) {
    ld c1 = cross(a, b, c), c2 = cross(a, b, d);
    ld c3 = cross(c, d, a), c4 = cross(c, d, b);
    if (sign(c1) * sign(c2) < 0 && sign(c3) * sign(c4) < 0) return true;
    return on_segment(c, a, b) || on_segment(d, a, b) ||
           on_segment(a, c, d) || on_segment(b, c, d);
}

// 6. 判断直线 ab 和直线 cd 是否相交（不平行/不重合）
bool line_intersect(PT a, PT b, PT c, PT d) {
    return sign(cross(b - a, d - c)) != 0;
}

// 7. 求直线 ab 和直线 cd 的交点（前提：必须先用 line_intersect 确保相交！）
PT line_intersection(PT a, PT b, PT c, PT d) {
    PT u = a - c;
    ld t = cross(d - c, u) / cross(b - a, d - c);
    return a + (b - a) * t;
}

//==================== 静态凸包(Andrew 算法) 需要PT(<)(==) 叉积 =======
// 返回按【逆时针顺序】排列的凸包顶点集合
// 参数 tight:
//   - tight = true  : 严格凸包 (边上【不包含】共线的中间点)
//   - tight = false : 非严格凸包 (边上【包含】共线的中间点)
vector<PT> convex_hull(vector<PT>& pts, bool tight = true) {
    int n = pts.size();
    if (n <= 2) return pts;

    // 1. 字典序排序 (先 x 后 y)
    sort(pts.begin(), pts.end());

    vector<PT> hull(2 * n);
    int k = 0; // 单调栈指针

    // 2. 求下凸壳
    for (int i = 0; i < n; ++i) {
        while (k >= 2) {
            ld c = cross(hull[k - 2], hull[k - 1], pts[i]);
            // 如果严格凸包，转向 <= 0 就弹栈；非严格凸包，转向 < 0 才弹栈
            if (tight ? sign(c) <= 0 : sign(c) < 0) k--;
            else break;
        }
        hull[k++] = pts[i];
    }

    // 3. 求上凸壳
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t) {
            ld c = cross(hull[k - 2], hull[k - 1], pts[i]);
            if (tight ? sign(c) <= 0 : sign(c) < 0) k--;
            else break;
        }
        hull[k++] = pts[i];
    }

    // 4. 调整大小，弹掉末尾与起点重复的点
    hull.resize(k - 1);
    return hull;
}

// =================== 旋转卡壳 (求凸包直径/平面最远点对) ===============
// 返回最远点对的坐标 pair<PT, PT>
pair<PT, PT> rotating_calipers_pair(const vector<PT>& hull) {
    int n = hull.size();
    if (n < 2) return {PT(0, 0), PT(0, 0)};
    if (n == 2) return {hull[0], hull[1]};

    ld max_d2 = -1;
    pair<PT, PT> best_pair;
    int j = 1;

    for (int i = 0; i < n; ++i) {
        int next_i = (i + 1) % n;

        while (cross(hull[i], hull[next_i], hull[(j + 1) % n]) > 
               cross(hull[i], hull[next_i], hull[j])) {
            j = (j + 1) % n;
        }

        ld d1 = dot(hull[i] - hull[j], hull[i] - hull[j]);
        if (d1 > max_d2) {
            max_d2 = d1;
            best_pair = {hull[i], hull[j]};
        }

        ld d2 = dot(hull[next_i] - hull[j], hull[next_i] - hull[j]);
        if (d2 > max_d2) {
            max_d2 = d2;
            best_pair = {hull[next_i], hull[j]};
        }
    }

    return best_pair;
}

//==================== 线段树 ============================================
//==================== 1. 自定义维护的信息 (Info) ========================
struct Info {
    ll sum = 0;      // 区间和
    ll mx = -1e18;   // 区间最大值
    ll mn = 1e18;    // 区间最小值

    // 成员重载 + 运算符（合并左右儿子）
    Info operator+(const Info& b) const {
        return {
            sum + b.sum,
            max(mx, b.mx),
            min(mn, b.mn)
        };
    }
};

//==================== 2. 延迟标记 Tag (默认注释) ========================
/* [Uncomment for Range Update]
struct Tag {
    ll add = 0; // 区间加标记

    bool empty() const { return add == 0; }
    void apply(const Tag& t) { add += t.add; }
    void clear() { add = 0; }
};
*/

//==================== 3. 线段树主体类 ===================================
struct SegmentTree {
    int n;
    vector<Info> tree;
    /* vector<Tag> tag; */ // [Uncomment for Range Update]

    SegmentTree(int n) : n(n), tree(4 * n + 5) /*, tag(4 * n + 5)*/ {}

    /* [Uncomment for Range Update]
    void apply(int u, int len, const Tag& t) {
        if (t.empty()) return;
        tree[u].sum += t.add * len;
        tree[u].mx += t.add;
        tree[u].mn += t.add;
        tag[u].apply(t);
    }

    void pushdown(int u, int l, int r) {
        if (tag[u].empty()) return;
        int mid = (l + r) >> 1;
        apply(2 * u, mid - l + 1, tag[u]);
        apply(2 * u + 1, r - mid, tag[u]);
        tag[u].clear();
    }
    */

    void pushup(int u) {
        tree[u] = tree[2 * u] + tree[2 * u + 1];
    }

    template<typename T>
    void build(int u, int l, int r, const vector<T>& v) {
        if (l == r) {
            tree[u] = {v[l], v[l], v[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(2 * u, l, mid, v);
        build(2 * u + 1, mid + 1, r, v);
        pushup(u);
    }

    // 单点修改 (将 pos 位置的值修改为 val)
    void modify_PT(int u, int l, int r, int pos, ll val) {
        if (l == r) {
            tree[u] = {val, val, val};
            return;
        }
        /* pushdown(u, l, r); */ // [Uncomment for Range Update]
        int mid = (l + r) >> 1;
        if (pos <= mid) modify_PT(2 * u, l, mid, pos, val);
        else modify_PT(2 * u + 1, mid + 1, r, pos, val);
        pushup(u);
    }

    /* [Uncomment for Range Update]
    // 区间修改 (将 [ql, qr] 应用标记 t)
    void modify_range(int u, int l, int r, int ql, int qr, const Tag& t) {
        if (ql <= l && r <= qr) {
            apply(u, r - l + 1, t);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) modify_range(2 * u, l, mid, ql, qr, t);
        if (qr > mid) modify_range(2 * u + 1, mid + 1, r, ql, qr, t);
        pushup(u);
    }
    */

    // 区间查询 [ql, qr]
    Info query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[u];
        /* pushdown(u, l, r); */ // [Uncomment for Range Update]
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(2 * u, l, mid, ql, qr);
        if (ql > mid) return query(2 * u + 1, mid + 1, r, ql, qr);
        return query(2 * u, l, mid, ql, qr) + query(2 * u + 1, mid + 1, r, ql, qr);
    }
};

//=================== 线性基 =============================================
struct LinearBasis {
    static const int B = 60; // 适用于 64 位整数范围 (最大 2^60 - 1)
    long long p[B + 1]{};    // 主线性基
    long long d[B + 1]{};    // 对角化重构后的线性基（用于求第 k 小）
    int cnt = 0;             // 重构后线性基的主元数量
    bool zero = false;       // 是否存在线性相关的元素（即能否异或出 0）
    // ull mask[B+1];        // mask[i]：构成p[i]的原始下标集合

    // 1. 插入元素：若插入成功返回 true；若已被线性表示（异或出 0）返回 false
    bool insert(long long x/*, ull id*/) {
        // ull cur_mask = id;
        for (int i = B; i >= 0; --i) {
            if (!(x >> i)) continue;
            if (!p[i]) { p[i] = x; /*mask[i]=cur_mask;*/ return true; }
            x ^= p[i];
            // cur_mask ^= mask[i]; // 关键！标记同步异或
        }
        zero = true;
        return false;
    }

    // 2. 判断 x 是否可由当前集合的子集异或表示
    bool check(long long x) const {
        for (int i = B; i >= 0; --i) {
            if (!(x >> i)) continue;
            if (!p[i]) return false;
            x ^= p[i];
        }
        return true;
    }

    // 3. 查询子集最大异或和
    long long getMax() const {
        long long res = 0;
        for (int i = B; i >= 0; --i) res = max(res, res ^ p[i]);
        return res;
    }

    // 4. 查询子集最小异或和（考虑能否凑成 0）
    long long getMin() const {
        if (zero) return 0;
        for (int i = 0; i <= B; ++i) if (p[i]) return p[i];
        return 0;
    }

    // 5. 重构线性基（高斯消元对角化，为 getKth 做准备）
    void rebuild() {
        cnt = 0;
        for (int i = 0; i <= B; ++i) {
            for (int j = 0; j < i; ++j) {
                if ((p[i] >> j) & 1) p[i] ^= p[j];
            }
        }
        for (int i = 0; i <= B; ++i) {
            if (p[i]) d[cnt++] = p[i];
        }
    }

    // 6. 查询第 k 小异或和（从 1 开始，需先调用 rebuild）
    long long getKth(long long k) const {
        if (zero) k--;
        if (k == 0) return 0;
        if (cnt < 62 && k >= (1ULL << cnt)) return -1; // 超出可能表示的数量
        long long res = 0;
        for (int i = 0; i < cnt; ++i) {
            if ((k >> i) & 1) res ^= d[i];
        }
        return res;
    }

    // 7. 合并另一个线性基
    void merge(const LinearBasis& other) {
        for (int i = B; i >= 0; --i) {
            if (other.p[i]) insert(other.p[i]);
        }
        if (other.zero) zero = true;
    }

    // ull get_combination(ll x){
    //     ull res=0;
    //     for(int i=B;i>=0;i--){
    //         if((x>>i)&1){
    //             x ^= p[i];
    //             res ^= mask[i];
    //         }
    //     }
    //     if(x!=0) return 0; // 无法拼凑
    //     return res;
    // }
};

//=================== KMP 字符串匹配 =====================================
// 1. 求前缀函数 pi 数组 (模式串 T 自己和自己匹配)
vector<int> get_pi(const string& T) {
    int n = T.length();
    vector<int> pi(n, 0);
    int j = 0;
    
    // i 从 1 开始，一往无前
    For(i, 1, n - 1) {
        // 不匹配时，j 顺着 pi 数组连续跳跃
        while (j > 0 && T[i] != T[j]) {
            j = pi[j - 1];
        }
        // 匹配成功，前缀长度加 1
        if (T[i] == T[j]) {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}

// 2. KMP 主匹配函数 (返回所有成功匹配的起始索引列表)
vector<int> kmp_search(const string& S, const string& T) {
    vector<int> positions;
    if (T.empty()) return positions;
    
    vector<int> pi = get_pi(T);
    int n = S.length();
    int m = T.length();
    int j = 0; // 模式串 T 的指针
    
    // i 扫描主串 S，绝不回头
    For(i, 0, n - 1) {
        // 主串与模式串失配，j 向前跳跃
        while (j > 0 && S[i] != T[j]) {
            j = pi[j - 1];
        }
        // 匹配成功，模式串指针后移
        if (S[i] == T[j]) {
            j++;
        }
        // 完全匹配成功一个 T 串
        if (j == m) {
            positions.push_back(i - m + 1); // 记录主串中的起始索引 (0-indexed)
            j = pi[j - 1];                  // 关键：允许重叠匹配，让 j 跳跃寻找下一个可能
        }
    }
    return positions;
}

//=================== Dijkstra============================================

// n: 点数, s: 源点, adj: 邻接表, p: 路径还原数组
vector<long long> dijkstra(int n, int s, const vector<vector<Edge>>& adj, vector<int>& p) {
    vector<long long> dist(n + 1, INF);
    p.assign(n + 1, -1);
    
    // {dist, node} 小顶堆
    using PLI = pair<long long, int>;
    priority_queue<PLI, vector<PLI>, greater<PLI>> pq;

    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue; // 懒惰删除：跳过过时状态

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long w = edge.weight;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                p[v] = u; // 记录前驱节点
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// 还原从 s 到 t 的最短路径
vector<int> Rpath(int s, int t, const vector<int>& p) {
    vector<int> path;
    for (int v = t; v != -1; v = p[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    if (path.empty() || path[0] != s) return {}; // 不可达
    return path;
}

//=================== SPFA ===============================================
// 返回 true 表示存在负环，false 表示正常求出最短路
bool spfa(int n, int s, const vector<vector<Edge>>& adj, vector<long long>& dist) {
    dist.assign(n + 1, INF);
    vector<int> cnt(n + 1, 0);      // 记录松弛次数，用于判负环
    vector<bool> in_queue(n + 1, false);
    queue<int> q;

    dist[s] = 0;
    q.push(s);
    in_queue[s] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = false;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long w = edge.weight;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                cnt[v] = cnt[u] + 1;
                
                if (cnt[v] >= n) return true; // 存在从 s 可达的负环

                if (!in_queue[v]) {
                    q.push(v);
                    in_queue[v] = true;
                }
            }
        }
    }
    return false; // 无负环
}

//=================== Folyd ==============================================
void floyd(int n, vector<vector<long long>>& d) {
    // d 需提前初始化：d[i][i] = 0，无边处 d[i][j] = INF
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (d[i][k] < INF && d[k][j] < INF) {
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
    }
}

//=================== 并查集 (DSU) =======================================
struct DSU {
    vector<int> fa;
    // vector<bool> has_cycle; // has_cycle[i] 表示以 i 为根的连通块内是否存在环
    //// int block_cnt;
    DSU(int n) {
        fa.resize(n + 1);
        // has_cycle.assign(n + 1, false);
        //// block_cnt = n;
        for (int i = 1; i <= n; i++) fa[i] = i;
    }
    int find(int x) {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) {
            // has_cycle[x] = true; // 同一连通块内再连边，该连通块成环
            return false;
        }
        fa[y] = x;
        // has_cycle[x] = has_cycle[x] || has_cycle[y]; 
        //// block_cnt--;
        return true;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
    // bool node_in_cycle(int x) {
    //     return has_cycle[find(x)];
    // }
    //// int get_block() const { return block_cnt; }
};

//=================== Kruskal 算法 =======================================
// n: 点数, edges: 所有的无向边列表
// 返回: {MST 总权值, MST 选中的边集}
// 若图不连通（无法选出 n-1 条边），总权值返回 -1
pair<long long, vector<KEdge>> kruskal(int n, vector<KEdge>& edges) {
    sort(edges.begin(), edges.end());
    
    DSU dsu(n);
    long long total_weight = 0;
    vector<KEdge> mst_edges;
    int cnt = 0; // 记录已加入 MST 的边数

    for (const auto& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            total_weight += edge.w;
            mst_edges.push_back(edge);
            cnt++;
            if (cnt == n - 1) break; // 已经选够 n - 1 条边，提前终止
        }
    }

    if (cnt != n - 1) return {-1, {}}; // 连通块不唯一，图不连通
    return {total_weight, mst_edges};
}

//=================== Prim ===============================================
// n: 点数, adj: 邻接表
// 返回: MST 的总权值，若不连通返回 -1
long long prim(int n, const vector<vector<Edge>>& adj) {
    vector<long long> min_edge(n + 1, INF); // 连通块到节点 i 的最小边权
    vector<bool> vis(n + 1, false);        // 是否已包含在 MST 中
    
    // {weight, u} 小顶堆
    using PLI = pair<long long, int>;
    priority_queue<PLI, vector<PLI>, greater<PLI>> pq;

    // 从 1 号点开始构建
    min_edge[1] = 0;
    pq.push({0, 1});

    long long total_weight = 0;
    int cnt = 0;

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (vis[u]) continue;

        vis[u] = true;
        total_weight += w;
        cnt++;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long weight = edge.weight;

            // 如果 v 还没加入 MST，且找到更小边权，则更新
            if (!vis[v] && weight < min_edge[v]) {
                min_edge[v] = weight;
                pq.push({min_edge[v], v});
            }
        }
    }

    if (cnt != n) return -1; // 不连通
    return total_weight;
}

//=================== 可撤销并查集 (RDSU) ================================
struct RDSU {
    vector<int> fa;
    vector<int> sz;
    int block_cnt;

    struct History {
        int x, y;
        bool merged;
    };
    vector<History> history;

    RDSU(int n) {
        fa.resize(n + 1);
        sz.assign(n + 1, 1);
        block_cnt = n;
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int x) {
        while (x != fa[x]) x = fa[x];
        return x;
    }

    bool unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) {
            history.push_back({x, y, false});
            return false; // 返回 false 证明这条边封闭了一个环
        }

        if (sz[x] < sz[y]) swap(x, y);

        history.push_back({x, y, true});
        fa[y] = x;
        sz[x] += sz[y];
        block_cnt--;
        return true;
    }

    void undo() {
        if (history.empty()) return;
        auto [x, y, merged] = history.back();
        history.pop_back();

        if (merged) {
            fa[y] = y;
            sz[x] -= sz[y];
            block_cnt++;
        }
    }

    int get_time() const { return (int)history.size(); }

    void rollback_to(int time_stamp) {
        while ((int)history.size() > time_stamp) {
            undo();
        }
    }

    bool connected(int x, int y) { return find(x) == find(y); }
    int get_block() const { return block_cnt; }
    int get_size(int x) { return sz[find(x)]; }
};

//=================== 基础欧拉筛 =========================================
struct PrimeSieve {
    int n;
    vector<int> primes;     // 存储所有筛出的质数
    vector<int> minp;       // minp[i] 记录数字 i 的最小质因数
    vector<bool> is_prime;  // is_prime[i] 表示 i 是否为质数

    PrimeSieve(int limit) : n(limit), minp(limit + 1, 0), is_prime(limit + 1, true) {
        sieve();
    }

    void sieve() {
        if (n < 0) return;
        if (n >= 0) is_prime[0] = false;
        if (n >= 1) is_prime[1] = false;

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                minp[i] = i;
            }
            for (int p : primes) {
                if (i * p > n) break;
                is_prime[i * p] = false;
                minp[i * p] = p;
                if (i % p == 0) break;
            }
        }
    }

    // 快捷查询：判断 x 是否为质数
    bool check(int x) const {
        if (x < 0 || x > n) return false;
        return is_prime[x];
    }

    // 利用 minp 进行 O(log x) 的质因数分解
    vector<int> get_factors(int x) const {
        vector<int> factors;
        while (x > 1 && x <= n) {
            factors.push_back(minp[x]);
            x /= minp[x];
        }
        return factors;
    }
};

//=================== 欧拉函数筛 =========================================
struct PhiSieve {
    int n;
    vector<int> primes;
    vector<int> minp;
    vector<int> phi;        // phi[i] 表示 i 的欧拉函数值
    vector<long long> sum_phi; // sum_phi[i] 表示 phi 的前缀和
    vector<bool> is_prime;

    PhiSieve(int limit) : n(limit), minp(limit + 1, 0), phi(limit + 1, 0), sum_phi(limit + 1, 0), is_prime(limit + 1, true) {
        sieve();
    }

    void sieve() {
        if (n < 0) return;
        if (n >= 0) is_prime[0] = false;
        if (n >= 1) {
            is_prime[1] = false;
            phi[1] = 1;
        }

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                minp[i] = i;
                phi[i] = i - 1; // 质数 p 的 phi(p) = p - 1
            }
            for (int p : primes) {
                if (i * p > n) break;
                is_prime[i * p] = false;
                minp[i * p] = p;

                if (i % p == 0) {
                    phi[i * p] = phi[i] * p; // p 包含在 i 的质因子中
                    break;
                } else {
                    phi[i * p] = phi[i] * (p - 1); // p 与 i 互质
                }
            }
        }

        // 预处理前缀和
        for (int i = 1; i <= n; ++i) {
            sum_phi[i] = sum_phi[i - 1] + phi[i];
        }
    }

    // 查询 x 的欧拉函数值 phi(x)
    int get_phi(int x) const {
        if (x < 1 || x > n) return 0;
        return phi[x];
    }

    // 查询 [1, x] 的欧拉函数前缀和
    long long get_phi_sum(int x) const {
        if (x < 1) return 0;
        if (x > n) x = n;
        return sum_phi[x];
    }

    // 查询区间 [l, r] 的欧拉函数和
    long long get_phi_sum(int l, int r) const {
        if (l > r) return 0;
        return get_phi_sum(r) - get_phi_sum(l - 1);
    }
};

//=================== 莫比乌斯函数筛 =====================================
struct MuSieve {
    int n;
    vector<int> primes;
    vector<int> minp;
    vector<int> mu;         // mu[i] 表示 i 的莫比乌斯函数值 (-1, 0, 1)
    vector<long long> sum_mu;  // sum_mu[i] 表示 mu 的前缀和
    vector<bool> is_prime;

    MuSieve(int limit) : n(limit), minp(limit + 1, 0), mu(limit + 1, 0), sum_mu(limit + 1, 0), is_prime(limit + 1, true) {
        sieve();
    }

    void sieve() {
        if (n < 0) return;
        if (n >= 0) is_prime[0] = false;
        if (n >= 1) {
            is_prime[1] = false;
            mu[1] = 1;
        }

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                minp[i] = i;
                mu[i] = -1; // 质数的 mu 值为 -1
            }
            for (int p : primes) {
                if (i * p > n) break;
                is_prime[i * p] = false;
                minp[i * p] = p;

                if (i % p == 0) {
                    mu[i * p] = 0; // 含有 p^2 平方因子，值为 0
                    break;
                } else {
                    mu[i * p] = -mu[i]; // 增加了一个不同的质因子，符号反转
                }
            }
        }

        // 预处理前缀和（数论分块常配合 sum_mu 使用）
        for (int i = 1; i <= n; ++i) {
            sum_mu[i] = sum_mu[i - 1] + mu[i];
        }
    }

    // 查询 x 的莫比乌斯函数值 mu(x)
    int get_mu(int x) const {
        if (x < 1 || x > n) return 0;
        return mu[x];
    }

    // 查询 [1, x] 的莫比乌斯函数前缀和
    long long get_mu_sum(int x) const {
        if (x < 1) return 0;
        if (x > n) x = n;
        return sum_mu[x];
    }

    // 查询区间 [l, r] 的莫比乌斯函数和
    long long get_mu_sum(int l, int r) const {
        if (l > r) return 0;
        return get_mu_sum(r) - get_mu_sum(l - 1);
    }
};

//=================== 组合数学筛 (三合一) ================================
struct MathSieve {
    int n;
    vector<int> primes;
    vector<int> minp;
    vector<bool> is_prime;
    vector<int> phi;
    vector<int> mu;

    MathSieve(int limit) : n(limit), minp(limit + 1, 0), is_prime(limit + 1, true),
                           phi(limit + 1, 0), mu(limit + 1, 0) {
        sieve();
    }

    void sieve() {
        if (n < 1) return;
        is_prime[0] = is_prime[1] = false;
        phi[1] = 1;
        mu[1] = 1;

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                minp[i] = i;      // 基础筛功能：记录最小质因子
                phi[i] = i - 1;   // 欧拉函数
                mu[i] = -1;       // 莫比乌斯函数
            }
            for (int p : primes) {
                if (i * p > n) break;
                is_prime[i * p] = false;
                minp[i * p] = p;   // 基础筛功能

                if (i % p == 0) {
                    phi[i * p] = phi[i] * p;
                    mu[i * p] = 0; // 包含 p^2
                    break;
                } else {
                    phi[i * p] = phi[i] * (p - 1);
                    mu[i * p] = -mu[i];
                }
            }
        }
    }

    // 💡 欧拉筛自带的基础功能：直接调用！
    bool check(int x) const { return is_prime[x]; }

    // 💡 欧拉筛自带的基础功能：极速质因数分解 O(log x)
    vector<int> get_factors(int x) const {
        vector<int> factors;
        while (x > 1 && x <= n) {
            factors.push_back(minp[x]);
            x /= minp[x];
        }
        return factors;
    }
};

// ================== Berlekamp-Massey ==================================
struct BerlekampMassey {
    // 逆元直接调用 Qpower
    ll inv(ll n) const {
        return Qpower((n % MOD + MOD) % MOD, MOD - 2);
    }

    /**
     * @brief 求解最短线性递推式
     * @param s 已知数列的前若干项 [f0, f1, f2, ...]
     * @return vector<ll> 递推系数 c，直接适配 kitamasa
     */
    vector<ll> solve(const vector<ll>& s) const {
        vector<ll> C = {1};
        vector<ll> B = {1};
        int L = 0;
        int m = 1;
        ll b = 1;

        for (size_t i = 0; i < s.size(); ++i) {
            ll d = (s[i] % MOD + MOD) % MOD;
            for (int j = 1; j <= L; ++j) {
                d = (d - C[j] * s[i - j] % MOD + MOD) % MOD;
            }

            if (d == 0) {
                m++;
            } else {
                vector<ll> T = C;
                ll scale = d * inv(b) % MOD;

                if (C.size() < B.size() + m) {
                    C.resize(B.size() + m, 0);
                }

                for (size_t j = 0; j < B.size(); ++j) {
                    C[j + m] = (C[j + m] - scale * B[j] % MOD + MOD) % MOD;
                }

                if (2 * L <= static_cast<int>(i)) {
                    L = i + 1 - L;
                    B = T;
                    b = d;
                    m = 1;
                } else {
                    m++;
                }
            }
        }

        vector<ll> c(L);
        for (int i = 1; i <= L; ++i) {
            c[L - i] = (MOD - C[i]) % MOD;
        }
        return c;
    }
};

//=================== Kitamasa 算法 ======================================
// k: 递推阶数, mod: 模数
// c: 递推系数, f0: 前k项值, n: 目标项下标
ll kitamasa(int k, const vector<ll>& c, const vector<ll>& f0, ll n) {
    auto mul = [&](const vector<ll>& a, const vector<ll>& b) {
        vector<ll> res(2 * k, 0);
        for (int i = 0; i < k; ++i)
            for (int j = 0; j < k; ++j)
                res[i + j] = (res[i + j] + a[i] * b[j]) % MOD;
        for (int i = 2 * k - 2; i >= k; --i)
            for (int j = 1; j <= k; ++j)
                res[i - j] = (res[i - j] + res[i] * c[k - j]) % MOD;
        res.resize(k);
        return res;
    };

    if (n < k) return f0[n];
    vector<ll> res(k, 0), x(k, 0);
    res[0] = 1; x[1 % k] = 1;
    for (ll i = n; i > 0; i >>= 1) {
        if (i & 1) res = mul(res, x);
        x = mul(x, x);
    }
    ll ans = 0;
    for (int i = 0; i < k; ++i) ans = (ans + res[i] * f0[i]) % MOD;
    return ans;
}

//=================== 矩阵乘法 ===========================================
#define Matrix vector<vector<ll>>

Matrix mul(const Matrix &a, const Matrix &b) {
    ll n = a.size(), p = b.size(), m = b[0].size();
    Matrix res(n, vector<ll>(m, 0));
    for (ll i = 0; i < n; i++) {
        for (ll k = 0; k < p; k++) {
            if (!a[i][k]) continue;
            for (ll j = 0; j < m; j++) {
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return res;
}

Matrix matrix_pow(Matrix a, ll k)
{
    ll n = a.size();
    Matrix res(n, vector<ll>(n, 0));
    for (ll i = 0; i < n; i++) res[i][i] = 1;

    while (k) {
        if (k & 1) res = mul(res, a);
        a = mul(a, a);
        k >>= 1;
    }
    return res;
}

//=================== Bostan-Mori 算法 ===================================
// 计算 [x^k] (P(x) / Q(x)) % MOD
// 要求: Q[0] = 1
/**
 * Bostan-Mori 算法：求常系数线性递推 / 生成函数特定项
 * 计算 [x^k] (P(x) / Q(x)) % MOD
 * 
 * 约束条件：
 * 1. Q[0] = 1
 * 2. MOD <= 1.07 * 10^9 (若更大，需调小步长 15，或更换 ull 为 __int128)
 * 
 */

// 假设 modInv 是求逆元的函数，如费马小定理 qpow(Q[0], MOD - 2)
// long long invQ0 = modInv(Q[0], MOD); 
// for (int &p : P) p = (1LL * p * invQ0) % MOD;
// for (int &q : Q) q = (1LL * q * invQ0) % MOD;
// // 规范化完成！现在的 Q[0] 绝对是 1 了
// long long ans = bostan_mori(P, Q, k, MOD);

ll bostan_mori(vector<int> P, vector<int> Q, ll k, int MOD) {
    while (k > 0) {
        int q_sz = Q.size();
        
        // 1. 构造共轭分母 Q(-x)
        vector<int> negQ(q_sz);
        for (int i = 0; i < q_sz; i++) {
            negQ[i] = (i & 1) ? (MOD - Q[i]) % MOD : Q[i];
        }

        int rem = k & 1; // 等价于 k % 2
        int max_deg_P = P.size() - 1 + q_sz - 1;
        vector<int> P_next(max_deg_P / 2 + 1, 0);
        
        // 2. 提取 P(x) * Q(-x) 的同奇偶项
        for (int m = rem; m <= max_deg_P; m += 2) {
            int start_j = max(0, m - q_sz + 1);
            int end_j = min((int)P.size() - 1, m);
            if (start_j > end_j) continue;
            
            ull sum = 0;
            for (int j = start_j; j <= end_j; j++) {
                sum += (ull)P[j] * negQ[m - j];
                // 极限位运算优化：每累加 16 次取模一次
                if (((j - start_j) & 15) == 15) sum %= MOD;
            }
            P_next[m >> 1] = sum % MOD;
        }

        int max_deg_Q = q_sz - 1 + q_sz - 1;
        vector<int> Q_next(max_deg_Q / 2 + 1, 0);
        
        // 3. 提取 Q(x) * Q(-x) 的偶数项
        for (int m = 0; m <= max_deg_Q; m += 2) {
            int start_j = max(0, m - q_sz + 1);
            int end_j = min(q_sz - 1, m);
            if (start_j > end_j) continue;
            
            ull sum = 0;
            for (int j = start_j; j <= end_j; j++) {
                sum += (ull)Q[j] * negQ[m - j];
                if (((j - start_j) & 15) == 15) sum %= MOD;
            }
            Q_next[m >> 1] = sum % MOD;
        }

        // 4. 压缩前导 0
        while (P_next.size() > 1 && P_next.back() == 0) P_next.pop_back();
        while (Q_next.size() > 1 && Q_next.back() == 0) Q_next.pop_back();

        P = move(P_next);
        Q = move(Q_next);
        k >>= 1; // 等价于 k /= 2
    }
    
    return P[0];
}

//=================== 类欧几里得算法 =====================================
const int P = 998244353;
const int i2 = 499122177, i6 = 166374059;

struct Node {
    int f, g, h;
    Node() : f(0), g(0), h(0) {}
    Node(int f, int g, int h) : f(f), g(g), h(h) {}
};

// f: sum floor((ai+b)/c)
// g: sum i*floor((ai+b)/c)
// h: sum floor((ai+b)/c)^2
Node solve(int a, int b, int c, int n) {
    if (a == 0) {
        int v = b / c;
        return Node(
            1LL * v * (n + 1) % P,
            1LL * v * n % P * (n + 1) % P * i2 % P,
            1LL * v * v % P * (n + 1) % P
        );
    }
    if (a >= c || b >= c) {
        int ac = a / c, bc = b / c;
        Node sub = solve(a % c, b % c, c, n);
        int s1 = 1LL * n * (n + 1) % P * i2 % P;
        int s2 = 1LL * n * (n + 1) % P * (2 * n + 1) % P * i6 % P;
        int f = (sub.f + 1LL * ac * s1 % P + 1LL * bc * (n + 1) % P) % P;
        int g = (sub.g + 1LL * ac * s2 % P + 1LL * bc * s1 % P) % P;
        int h = (sub.h + 1LL * ac * ac % P * s2 % P + 1LL * bc * bc % P * (n + 1) % P 
                 + 2LL * ac * bc % P * s1 % P + 2LL * ac * sub.g % P + 2LL * bc * sub.f % P) % P;
        return Node(f, g, h);
    }
    int m = (1LL * a * n + b) / c;
    Node sub = solve(c, c - b - 1, a, m - 1);
    int f = (1LL * n * m % P - sub.f + P) % P;
    int g = (1LL * n * (n + 1) % P * m % P - sub.f - sub.h + P + P) % P * i2 % P;
    int h = (1LL * n * m % P * (m + 1) % P - 2LL * sub.g % P - 2LL * sub.f % P - f + P + P + P) % P;
    return Node(f, g, h);
}

//=================== AC自动机 (实时查询匹配位置) ========================
struct AC_Realtime {
    static const int MAX_NODE = 200005;
    static const int SIGMA = 26;

    int tr[MAX_NODE][SIGMA];
    int fail[MAX_NODE];
    int last[MAX_NODE];         // 后缀链接：直接指向 fail 链上最近的真实单词节点
    vector<int> words[MAX_NODE];// 记录在当前节点结尾的模式串 ID (支持重复串)
    int tot;

    void clear() {
        for (int i = 0; i <= tot; i++) {
            fail[i] = last[i] = 0;
            words[i].clear();
            for (int j = 0; j < SIGMA; j++) tr[i][j] = 0;
        }
        tot = 0;
    }

    AC_Realtime() { tot = 0; }

    // 1. 插入模式串
    void insert(const string& s, int id) {
        int u = 0;
        for (char c : s) {
            int v = c - 'a';
            if (!tr[u][v]) tr[u][v] = ++tot;
            u = tr[u][v];
        }
        words[u].push_back(id); // 记下结尾在节点 u 的模式串编号
    }

    // 2. 构建 AC 自动机 (同时递推计算 last 指针)
    void build() {
        queue<int> q;
        for (int i = 0; i < SIGMA; i++) {
            if (tr[0][i]) q.push(tr[0][i]);
        }

        while (!q.empty()) {
            int u = q.front(); q.pop();

            for (int i = 0; i < SIGMA; i++) {
                int v = tr[u][i];
                if (v) {
                    fail[v] = tr[fail[u]][i];

                    // 【核心：last 指针递推】
                    // 如果 fail[v] 本身是单词，last 直接连 past；否则继承 fail[v] 的 last
                    if (!words[fail[v]].empty()) last[v] = fail[v];
                    else last[v] = last[fail[v]];

                    q.push(v);
                } else tr[u][i] = tr[fail[u]][i]; // Trie 图补边
            }
        }
    }

    /**
     * 3. 扫描文本串并打印/记录匹配位置
     * @param text 文本串
     * @param len_map 存每个 pattern_id 的字符串长度，用于计算起始下标 [start, end]
     */
    void query(const string& text, const vector<int>& len_map) {
        int u = 0;
        for (int i = 0; i < text.length(); i++) {
            u = tr[u][text[i] - 'a'];

            // 沿着 last 链只跳有效单词节点，避开所有过渡空节点
            for (int j = u; j; j = last[j]) {
                for (int id : words[j]) {
                    int pattern_len = len_map[id];
                    int start_pos = i - pattern_len + 1;
                    int end_pos = i;
                    cout << "模式串 #" << id << " 匹配成功！"
                         << " 位置: [" << start_pos << ", " << end_pos << "]\n";
                }
            }
        }
    }
} ac_realtime;

//=================== AC自动机 (拓扑排序统计频次) ========================
struct AC_Frequency {
    static const int MAX_NODE = 200005;
    static const int SIGMA = 26;

    int tr[MAX_NODE][SIGMA];
    int fail[MAX_NODE];
    int in[MAX_NODE];    // Fail 树节点入度
    int val[MAX_NODE];   // 节点直接命中次数
    int pos[MAX_NODE];   // 第 i 个模式串对应的 Trie 树结尾编号
    int tot;

    void clear() {
        for (int i = 0; i <= tot; i++) {
            fail[i] = in[i] = val[i] = pos[i] = 0;
            for (int j = 0; j < SIGMA; j++) tr[i][j] = 0;
        }
        tot = 0;
    }

    AC_Frequency() { tot = 0; }

    // 1. 插入模式串
    void insert(const string& s, int id) {
        int u = 0;
        for (char c : s) {
            int v = c - 'a';
            if (!tr[u][v]) tr[u][v] = ++tot;
            u = tr[u][v];
        }
        pos[id] = u;
    }

    // 2. 构建 AC 自动机 (维护 Fail 树入度)
    void build() {
        queue<int> q;
        for (int i = 0; i < SIGMA; i++) {
            if (tr[0][i]) q.push(tr[0][i]);
        }

        while (!q.empty()) {
            int u = q.front(); q.pop();

            for (int i = 0; i < SIGMA; i++) {
                int v = tr[u][i];
                if (v) {
                    fail[v] = tr[fail[u]][i];
                    in[fail[v]]++; // 记录 Fail 树父节点入度
                    q.push(v);
                } else {
                    tr[u][i] = tr[fail[u]][i]; // Trie 图补边
                }
            }
        }
    }

    // 3. 扫描文本：纯打卡记账，时间严格 O(|Text|)，一步 fail 都不跳
    void query(const string& text) {
        int u = 0;
        for (char c : text) {
            u = tr[u][c - 'a'];
            val[u]++;
        }
    }

    // 4. 拓扑排序：自底向上沿 Fail 树汇总结算，时间严格 O(节点数)
    void topo() {
        queue<int> q;
        for (int i = 1; i <= tot; i++) {
            if (in[i] == 0) q.push(i); // 叶子节点入队
        }

        while (!q.empty()) {
            int u = q.front(); q.pop();

            int p = fail[u];
            val[p] += val[u]; // 子节点账目汇总给父节点

            if (--in[p] == 0) {
                q.push(p);
            }
        }
    }

    // 5. 查表获取第 id 个模式串出现的总次数
    int get_ans(int id) {
        return val[pos[id]];
    }
} ac_freq;

//=================== AC自动机 (显式建树 DFS 树形 DP) ====================
// 显式建树 + DFS 树形 DP 模板
struct AC_Explicit {
    static const int MAX_NODE = 200005;
    static const int SIGMA = 26;

    int tr[MAX_NODE][SIGMA];
    int fail[MAX_NODE];
    int pos[MAX_NODE];
    vector<int> G[MAX_NODE]; // 显式建树：邻接表保存 Fail 树边
    int tot;

    // 额外业务数据（以位置合并为例）
    vector<int> direct_pos[MAX_NODE]; // 直接落点
    vector<int> query_ans[MAX_NODE];  // 仅有效查询节点保留备份
    bool is_query[MAX_NODE];

    void clear() {
        for (int i = 0; i <= tot; i++) {
            fail[i] = pos[i] = is_query[i] = 0;
            G[i].clear();
            direct_pos[i].clear();
            query_ans[i].clear();
            for (int j = 0; j < SIGMA; j++) tr[i][j] = 0;
        }
        tot = 0;
    }

    AC_Explicit() { tot = 0; }

    void insert(const string& s, int id) {
        int u = 0;
        for (char c : s) {
            int v = c - 'a';
            if (!tr[u][v]) tr[u][v] = ++tot;
            u = tr[u][v];
        }
        pos[id] = u;
        is_query[u] = true;
    }

    void build() {
        queue<int> q;
        for (int i = 0; i < SIGMA; i++) if (tr[0][i]) q.push(tr[0][i]);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < SIGMA; i++) {
                int v = tr[u][i];
                if (v) {
                    fail[v] = tr[fail[u]][i];
                    q.push(v);
                } else {
                    tr[u][i] = tr[fail[u]][i];
                }
            }
        }

        // 【显式建树】建图：从 fail[i] 连一条有向边到子节点 i
        for (int i = 1; i <= tot; i++) {
            G[fail[i]].push_back(i);
        }
    }

    void run(const string& text) {
        int u = 0;
        for (int i = 1; i <= (int)text.length(); i++) {
            u = tr[u][text[i - 1] - 'a'];
            direct_pos[u].push_back(i);
        }
    }

    // DFS 遍历 Fail 树自底向上合并复杂数据
    vector<int> dfs(int u) {
        vector<int> cur = move(direct_pos[u]);

        for (int v : G[u]) {
            vector<int> sub = dfs(v);
            if (sub.empty()) continue;

            if (cur.empty()) {
                cur = move(sub);
            } else {
                // 线性归并合并，避免 sort 开销
                vector<int> merged(cur.size() + sub.size());
                merge(cur.begin(), cur.end(), sub.begin(), sub.end(), merged.begin());
                cur = move(merged);
            }
        }

        if (is_query[u]) {
            query_ans[u] = cur; // 按需备份，防 MLE
        }
        return cur;
    }
} ac_explicit;

int main()
{
    return 0;
}
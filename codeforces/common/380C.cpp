#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int full,open,closed;
    Node() : full(0),open(0),closed(0) {}
    Node(int f,int o,int c) : full(f),open(o),closed(c) {}
    Node(char ch)
    {
        full = 0;
        open = (ch == '(');
        closed = (ch == ')');
    }
};

Node merge(Node l,Node r)
{
    int match = min(l.open,r.closed);
    return Node
    (
        l.full + r.full + match * 2,
        l.open + r.open - match,
        l.closed + r.closed - match
    );
}

const int N = 1000010;
Node seg[N * 4];
string s;

void build(int id,int l,int r)
{
    if(l == r)
    {
        seg[id] = Node(s[l]);
        return;
    }
    int mid = (l + r) >> 1;
    build(id << 1,l,mid);
    build(id << 1 | 1,mid + 1,r);
    seg[id] = merge(seg[id << 1],seg[id << 1 | 1]);
}

Node query(int id ,int l,int r,int ql,int qr)
{
    if(ql <= l && r <= qr) return seg[id];
    int mid = (l+r) >> 1;
    if(qr <= mid) return query(id << 1,l,mid,ql,qr);
    if(ql > mid) return query(id << 1 | 1,mid + 1,r,ql,qr);
    return merge
    (
        query(id << 1,l,mid,ql,qr),
        query(id << 1 | 1,mid + 1,r,ql,qr)
    );
}

int main()
{
    cin >> s;
    int n = s.size();
    s = " " + s;

    build(1, 1, n);

    int m;cin >> m;

    for(int i = 0;i < m;i++)
    {
        int l,r;
        cin >> l >> r;
        cout << query(1,1,n,l,r).full << "\n";
    }

    return 0;
}
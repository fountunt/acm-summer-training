#include <iostream>
#include <vector>
#include <utility>
using namespace std;

enum Poker {
    HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND,
    STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH
};

int bsr(int a) {
    return 31 - __builtin_clz(a);
}

int highBits(int a, int k) {
    int b = 0;
    for (int i = 0; i < k; ++i) {
        if (!a) return -1;
        b |= 1 << (31 - __builtin_clz(a));
        a &= ~b;
    }
    return b;
}

const char RANKS_STR[16] = "0123456789TJQKA";
const char SUITS_STR[5] = "CDHS";

int parseCard(const char *str) {
    int r, s;
    for (s = 0; s < 4; ++s) if (SUITS_STR[s] == str[1]) break;
    if (s < 4) {
        for (r = 2; r < 15; ++r) if (RANKS_STR[r] == str[0]) break;
    } else {
        for (s = 0; s < 4; ++s) if (SUITS_STR[s] == str[0]) break;
        for (r = 2; r < 15; ++r) if (RANKS_STR[r] == str[1]) break;
    }
    return (r - 2) << 2 | s;
}//牌型转码

int readCard() {
    static char buf[3];
    cin >> buf;
    return parseCard(buf);
}

pair<Poker, int> poker(const vector<int>& cards)
{
    int a = 0, bs[4] = {}, cs[15] = {}, ds[5] = {};
    for (int card : cards)
    {
        int r = (card >> 2) + 2;
        int s = card & 3;
        a |= bs[s] |= 1 << r;
        ++cs[r];
    }
    for (int r = 2; r < 15; ++r) ds[cs[r]] |= 1 << r;

    // STRAIGHT_FLUSH
    {
        int straightFlush = 0;
        for (int s = 0; s < 4; ++s)
            straightFlush |= bs[s] & bs[s] << 1 & bs[s] << 2 & bs[s] << 3
                           & (bs[s] << 4 | bs[s] >> 14 << 5);
        if (straightFlush) return make_pair(STRAIGHT_FLUSH, bsr(straightFlush));
    }

    // FOUR_OF_A_KIND
    if (ds[4]) {
        int r4 = bsr(ds[4]);
        return make_pair(FOUR_OF_A_KIND, r4 << 4 | bsr(a ^ (1 << r4)));
    }

    // FULL_HOUSE
    if (ds[3]) {
        int r3 = bsr(ds[3]);
        int d = (ds[3] ^ (1 << r3)) | ds[2];
        if (d) {
            int r2 = bsr(d);
            return make_pair(FULL_HOUSE, r3 << 4 | r2);
        }
    }

    // FLUSH
    {
        int flush = -1;
        for (int s = 0; s < 4; ++s) {
            int h = highBits(bs[s], 5);
            if (flush < h) flush = h;
        }
        if (flush >= 0) return make_pair(FLUSH, flush);
    }

    // STRAIGHT
    {
        int straight = a & a << 1 & a << 2 & a << 3 & (a << 4 | a >> 14 << 5);
        if (straight) return make_pair(STRAIGHT, bsr(straight));
    }

    // THREE_OF_A_KIND
    if (ds[3]) {
        int r3 = bsr(ds[3]);
        return make_pair(THREE_OF_A_KIND, r3 << 16 | highBits(a ^ (1 << r3), 2));
    }

    // TWO_PAIR / ONE_PAIR
    if (ds[2]) {
        int r2 = bsr(ds[2]);
        int d = ds[2] ^ (1 << r2);
        if (d) {
            int r22 = bsr(d);
            return make_pair(TWO_PAIR, r2 << 8 | r22 << 4 | bsr(a ^ (1 << r2) ^ (1 << r22)));
        }
        return make_pair(ONE_PAIR, r2 << 16 | highBits(a ^ (1 << r2), 3));
    }

    // HIGH_CARD
    return make_pair(HIGH_CARD, highBits(a, 5));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int numCases;
    while (cin >> numCases)
    {
        for (int caseId = 1; caseId <= numCases; ++caseId)
        {
            int A[8];
            for (int i = 0; i < 8; ++i) A[i] = readCard();

            bool used[52] = {};
            for (int i = 0; i < 8; ++i) used[A[i]] = true;

            int ans = 1;
            for (int y = 0; y < 52; ++y) if (!used[y])
            {
                auto you = poker({A[4], A[5], A[6], A[7], y});
                int mx = -1;
                for (int x = 0; x < 52; ++x) if (!used[x] && y != x)
                {
                    auto me = poker({A[0], A[1], A[2], A[3], x});
                    if (me > you) { mx = 1; break; }
                    else if (me == you) { mx = 0; }
                }
                if (mx < ans) ans = mx;
                if (ans < 0) break;
            }

            if (ans > 0) cout << "WoYaoYanPai\n";
            else if (ans < 0) cout << "GeiWoCaPiXie\n";
            else cout << "PaiMeiYouWenTi\n";
        }
    }
    return 0;
}

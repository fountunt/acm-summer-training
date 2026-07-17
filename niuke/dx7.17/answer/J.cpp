#include<bits/stdc++.h>
using namespace std;
using ll = long long;

struct Card {
    int rank;
    int suit;
};

int rank_val(char c) {
    if(c >= '2' && c <= '9') return c - '0';
    if(c == 'T') return 10;
    if(c == 'J') return 11;
    if(c == 'Q') return 12;
    if(c == 'K') return 13;
    if(c == 'A') return 14;
    return 0;
}

int suit_val(char c) {
    if(c == 'C') return 0;
    if(c == 'D') return 1;
    if(c == 'H') return 2;
    if(c == 'S') return 3;
    return 0;
}

Card parse(string s) {
    return {rank_val(s[0]), suit_val(s[1])};
}

int get_id(Card c) {
    return c.suit * 13 + (c.rank - 2);
}

Card get_card(int id) {
    return {id % 13 + 2, id / 13};
}

uint32_t make_sig(int type, int v1=0, int v2=0, int v3=0, int v4=0, int v5=0) {
    return (type << 20) | (v1 << 16) | (v2 << 12) | (v3 << 8) | (v4 << 4) | v5;
}

uint32_t eval_hand(Card c1, Card c2, Card c3, Card c4, Card c5) {
    Card hand[5] = {c1, c2, c3, c4, c5};
    for(int i = 0; i < 5; ++i) {
        for(int j = i + 1; j < 5; ++j) {
            if(hand[i].rank < hand[j].rank) {
                swap(hand[i], hand[j]);
            }
        }
    }

    bool is_flush = true;
    for(int i = 1; i < 5; ++i) {
        if(hand[i].suit != hand[0].suit) is_flush = false;
    }

    bool is_straight = false;
    int straight_high = 0;

    if(hand[0].rank == hand[1].rank + 1 &&
       hand[1].rank == hand[2].rank + 1 &&
       hand[2].rank == hand[3].rank + 1 &&
       hand[3].rank == hand[4].rank + 1) {
        is_straight = true;
        straight_high = hand[0].rank;
    } else if(hand[0].rank == 14 && hand[1].rank == 5 &&
              hand[2].rank == 4 && hand[3].rank == 3 && hand[4].rank == 2) {
        is_straight = true;
        straight_high = 5;
    }

    if(is_straight && is_flush) return make_sig(8, straight_high);

    int counts[15] = {0};
    for(int i = 0; i < 5; ++i) counts[hand[i].rank]++;

    int quad = 0, trips = 0, pair1 = 0, pair2 = 0;
    for(int i = 14; i >= 2; --i) {
        if(counts[i] == 4) quad = i;
        else if(counts[i] == 3) trips = i;
        else if(counts[i] == 2) {
            if(pair1 == 0) pair1 = i;
            else pair2 = i;
        }
    }

    if(quad) {
        int kicker = 0;
        for(int i = 0; i < 5; ++i) if(hand[i].rank != quad) kicker = hand[i].rank;
        return make_sig(7, quad, kicker);
    }
    if(trips && pair1) {
        return make_sig(6, trips, pair1);
    }
    if(is_flush) {
        return make_sig(5, hand[0].rank, hand[1].rank, hand[2].rank, hand[3].rank, hand[4].rank);
    }
    if(is_straight) {
        return make_sig(4, straight_high);
    }
    if(trips) {
        int k[2], idx = 0;
        for(int i = 0; i < 5; ++i) if(hand[i].rank != trips) k[idx++] = hand[i].rank;
        return make_sig(3, trips, k[0], k[1]);
    }
    if(pair1 && pair2) {
        int kicker = 0;
        for(int i = 0; i < 5; ++i) if(hand[i].rank != pair1 && hand[i].rank != pair2) kicker = hand[i].rank;
        return make_sig(2, pair1, pair2, kicker);
    }
    if(pair1) {
        int k[3], idx = 0;
        for(int i = 0; i < 5; ++i) if(hand[i].rank != pair1) k[idx++] = hand[i].rank;
        return make_sig(1, pair1, k[0], k[1], k[2]);
    }

    return make_sig(0, hand[0].rank, hand[1].rank, hand[2].rank, hand[3].rank, hand[4].rank);
}

void solve()
{
    string s;
    vector<Card> my(4), op(4);
    bool used[52] = {false};

    for(int i = 0; i < 4; ++i) {
        cin >> s;
        my[i] = parse(s);
        used[get_id(my[i])] = true;
    }
    for(int i = 0; i < 4; ++i) {
        cin >> s;
        op[i] = parse(s);
        used[get_id(op[i])] = true;
    }

    uint32_t my_vals[52] = {0};
    uint32_t op_vals[52] = {0};

    for(int i = 0; i < 52; ++i) {
        if(used[i]) continue;
        Card c = get_card(i);
        my_vals[i] = eval_hand(my[0], my[1], my[2], my[3], c);
        op_vals[i] = eval_hand(op[0], op[1], op[2], op[3], c);
    }

    int overall = 2;
    for(int i = 0; i < 52; ++i) {
        if(used[i]) continue;

        int my_best = -2;
        for(int j = 0; j < 52; ++j) {
            if(used[j] || i == j) continue;

            int cmp = 0;
            if(my_vals[j] > op_vals[i]) cmp = 1;
            else if(my_vals[j] < op_vals[i]) cmp = -1;

            my_best = max(my_best, cmp);
        }
        overall = min(overall, my_best);
    }

    if(overall == 1) cout << "WoYaoYanPai\n";
    else if(overall == -1) cout << "GeiWoCaPiXie\n";
    else cout << "PaiMeiYouWenTi\n";
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;

    while(t--)
    solve();

    return 0;
}

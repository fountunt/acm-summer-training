#!/bin/bash
# ============================================================
# 极简对拍:gen 造数据,bf 与 std 各跑一遍,diff 不一致即停
# 用法: bash duipai.sh [轮数,默认 1000]
#   先在同一目录写好 gen.cpp / bf.cpp / std.cpp
# ============================================================

CNT=${1:-1000}

g++ -std=c++17 -O2 -o gen gen.cpp
g++ -std=c++17 -O2 -o bf  bf.cpp
g++ -std=c++17 -O2 -o std std.cpp

for ((i=1; i<=CNT; i++)); do
    ./gen > in
    ./std < in > std.out
    ./bf  < in > bf.out

    if diff -bB std.out bf.out > /dev/null; then
        echo "test $i: OK"
    else
        echo "test $i: WA"
        echo "---- input (in) ----"
        cat in
        echo "---- your answer (std.out) ----"
        cat std.out
        echo "---- expected (bf.out) ----"
        cat bf.out
        exit 1
    fi
done

echo "All $CNT tests passed."

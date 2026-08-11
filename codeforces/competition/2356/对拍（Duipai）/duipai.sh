#!/bin/bash
# ============================================================
# 对拍脚本 duipai.sh
# gen 造数据,bf 与 std 各跑一遍,输出不一致即停止并打印现场
# 用法: bash duipai.sh [轮数,默认 1000]
#   bash duipai.sh         # 跑 1000 轮
#   bash duipai.sh 10000   # 跑 1 万轮
# ============================================================

CNT=${1:-1000}

# 编译(Windows 下生成 .exe;文件名保持一致)
g++ -O2 -o gen gen.cpp
g++ -O2 -o bf  bf.cpp
g++ -O2 -o std std.cpp

for ((i=1; i<=CNT; i++)); do
    ./gen > data.in
    ./bf  < data.in > bf.out
    ./std < data.in > std.out

    if ! diff -bB bf.out std.out > /dev/null; then
        echo "WA at test $i"
        echo "---- input (data.in) ----"
        cat data.in
        echo "---- your answer (std.out) ----"
        cat std.out
        echo "---- expected (bf.out) ----"
        cat bf.out
        exit 1
    fi

    # 进度提示,不要每轮都打,刷屏
    if (( i % 100 == 0 )); then
        echo "pass $i"
    fi
done

echo "All $CNT tests passed."

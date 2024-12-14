# mahjong

tablemake.cpp : 自分用．機能は果たしているものの出力の可読性が低すぎる．実行環境によるが，メモリがそこそこ厳しそう．

tablemake.html : 視覚的に使い物になるものを作成中．

## 概要

- 14人参加12節開催の麻雀(1試合4人制の個人戦競技)のリーグ戦組み合わせ表の計算プログラム.
- リーグ戦において同じ人と当たりすぎる(その一方で, ある人とは当たらなさすぎる)ような組み合わせだと良くないのでなるべく均等になるようにする.

## 作成の動機

- ググっても14人12節では良い感じの組み合わせを出してくれるやつが見当たらなかったので自作することに.

## アルゴリズム

- initialize() : 初期組み合わせを乱数で適当に作る.

- violation(state) : ある組み合わせ state について『どれくらい同じ人と当たっているか』を評価する関数. ある人が同じ相手に当たった回数の閾値(今回だと 3)を超過したぶんの総和を返す.

- update(state) : 当たりすぎているところをあまり当たっていないところと入れ替える.

- climbing(first_state) : 初期状態 first_state から始めて, violation() が 0 になるか時間的な都合で打ち切るまで update() を繰り返す.

## 計算量

n == 14(参加人数)
m == 12(節数)

- violation() : $O(m * n^2)$
    - 全ての節(m)について、全てのプレイヤーのペア(n^2)を調べる

- update() : $O(m^2 * n^4)$
    - 1回の violation() 呼び出しに $O(m * n^2)$
    - 各スワップごとに violation() を呼び出してペナルティを計算

- climbing() : $O(T * update())$
    - $T$は自分で決めた update() の実行回数.
    - 全体で$O(T * m^2 * n^4)$ 

- simulated_annealing() : $O(K * R * m^2 * n^4)$
    - 焼きなまし法における温度更新回数 $K$ は、初期温度 (INITIAL_TEMPERATURE) と冷却率 (COOLING_RATE) に依存する。
    - 温度が $1$ 以下になるまでの更新回数を $K$ とすると、以下のように計算される：
        $K = \\left\\lceil \\log_{r} \\left( \\frac{1}{\\text{INITIAL\\_TEMPERATURE}} \\right) \\right\\rceil$
    - $R$ は各ステップでの update() の試行回数
    - 現在は $R = 10$ で設定

## 改善点
- アウトプットの UI を考える
- $m$, $n$ について一般化
# mahjong

tablemake.cpp : 自分用．機能は果たしているものの出力の可読性が低すぎる．実行環境によるが，メモリがそこそこ厳しそう．

tablemake.html : 視覚的に使い物になるものを作成中．←何でhtmlでやろうとしてたん？

## 概要

- 14人参加12節開催の麻雀(1試合4人制の個人戦競技)のリーグ戦組み合わせ表の計算プログラム.
- リーグ戦において同じ人と当たりすぎる(その一方で, ある人とは当たらなさすぎる)ような組み合わせだと良くないのでなるべく均等になるようにする.

## 作成の動機

- ググっても14人12節では良い感じの組み合わせを出してくれるやつが見当たらなかったので自作することに.

## アルゴリズム

- initialize() : 初期組み合わせを乱数で適当に作る.

- violation(state) : ある組み合わせ state について『どれくらい同じ人と当たっているか』を評価する関数. ある人が同じ相手に当たった回数の閾値(今回だと 3)を超過したぶんの総和を返す.

- update(state) : 当たりすぎているところをあまり当たっていないところと入れ替える.

- simulated_annealing(initial_state) : 初期状態 initial_state からスタートし、焼きなまし法を用いて最適な組み合わせを探索する.
    - 初期温度を設定し、温度が$1$を超える間下記を繰り返す
        - update() で入れ替え候補の状態を生成し、入れ替え前の状態とペナルティ値 violation() を比較する
        - ペナルティが改善される場合、または確率的に悪化を許容する条件（焼きなまし法の確率ルール）を満たす場合に candidate_state を採用※
        - ペナルティの暫定最小値を達成した状態を best_state に記録
        - 温度を冷却率 $r$ に従って冷却
    - best state を返す

※焼きなまし法は、局所最適解を避けるため、確率的にペナルティが悪化する変更も許容する

## 計算量

$n$ = 14(参加人数) \\
$m$ = 12(節数)

- violation() : $O(m * n^2)$
    - 全ての節(m)について、全てのプレイヤーのペア(n^2)を調べる

- update() : $O(m^2 * n^4)$
    - 1回の violation() 呼び出しに $O(m * n^2)$
    - 各スワップごとに violation() を呼び出してペナルティを計算

- climbing() : $O(T * update())$
    - $T$は自分で決めた update() の実行回数.
    - 全体で $O(T * m^2 * n^4)$ 

- simulated_annealing() : $O(K * R * m^2 * n^4)$
    - 焼きなまし法における温度更新回数 $K$ は、初期温度 INITIAL_TEMPERATURE と冷却率 $r$;COOLING_RATE に依存する。
    - 温度が $1$ 以下になるまでの更新回数を $K$ とすると、以下のように計算される：
        $K = \\left\\lceil \\log_{r} \\left( \\frac{1}{\\text{INITIAL\\_TEMPERATURE}} \\right) \\right\\rceil$
    - $R$ は各ステップでの update() の試行回数
    - 現在は $R = 10$ で設定

## 改善点
- アウトプットの UI を考える
- $m$, $n$ について一般化

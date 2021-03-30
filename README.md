# mahjong

tablemake.cpp : 自分用．機能は果たしているものの出力の可読性が低すぎる．実行環境によるが，メモリがそこそこ厳しそう．
tablemake.html : 視覚的に使い物になるものを作成中．

〇概要

・14人参加12節開催の麻雀(1試合4人制の個人戦競技)のリーグ戦組み合わせ表の計算プログラム.
・リーグ戦において同じ人と当たりすぎる(その一方で, ある人とは当たらなさすぎる)ような組み合わせだと良くないのでなるべく均等になるようにする.

〇作成の動機

・ググっても14人12節では良い感じの組み合わせを出してくれるやつが見当たらなかったので自作することに.

〇アルゴリズム

・initialize() : 初期組み合わせを乱数で適当に作る.

・violation(state) : ある組み合わせ state について『どれくらい同じ人と当たっているか』を評価する関数. ある人が同じ相手に当たった回数の閾値(今回だと 3)を超過したぶんの総和を返す.

・update(state) : 当たりすぎているところをあまり当たっていないところと入れ替える.

・climbing(first_state) : 初期状態 first_state から始めて, violation() が 0 になるか時間的な都合で打ち切るまで update() を繰り返す.

〇計算量

n == 14(参加人数)
m == 12(節数)

・violation() : O(n^2)

・update() : O(n^3 * m * O(violation()))

・climbing() : O(T * update()) // Tは自分で決めた update() の実行回数.

・全体でO(T * n^5 * m) // 今 T = 12でやってても中々渋い時間が掛かる. 最適解に辿り着かないこともある.

〇改善点

・時間的には余裕があるハズだけど結構な割合でｾｸﾞﾌｫる. どうして...

・もう少し見やすく書く.

・計算量改善(分からない). update() 内部の violation() 計算で全部を見る必要がないため，そこで落とせる雰囲気はある.

・n について一般化.

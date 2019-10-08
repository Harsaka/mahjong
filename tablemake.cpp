#include <bits/stdc++.h>
using namespace std;
#define REP(i, n) for(int i = 0; i < n; i++)
 
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vector<ll>>;

// n 人参加 m 節開催の 4 人制ゲームのリーグ戦の対戦表生成ツール.
// 同じ人と当たり過ぎないような組み合わせを出力する.

const ll n = 14; // 参加人数
const ll m = 12; // 節数

// 基本的に 1 節あたり 3 試合開催され, 余った 2 人は抜け番(休み)となる.
// 第 5 節のみ 2 試合開催となり抜け番が 6 人となる. 

// 抜け番の位置は予め決めてある.
vvll make_absent(){
    vvll absent(m, vll(n));
    REP(i, 4){
        REP(j, 2){
            absent[i][2 * i + j] = 1;
        }
    }
    REP(j, 6) absent[4][8 + j] = 1;
    REP(i, 7){
        REP(j, 2){
            absent[5 + i][2 * i + j] = 1;
        }
    }
    return absent;
}

// 最初に乱数で適当に卓組を初期化する.
vvll initialize(vvll &absent){
    vvll first_state(m, vll(n));
    ll tmp;
    REP(i, m){
        vll count(3); // 卓に座っている人数.
        vll used(3); // 満卓判定. 満卓なら座れない.
        REP(j, n){
            if(absent[i][j]){
                first_state[i][j] = -1;
                continue;
            }
            if(i == 4){
                tmp = rand() % 2;
                if(used[tmp]){
                    tmp = (tmp + 1) % 2;
                }
            }
            else{
                tmp = rand() % 3;
                while(used[tmp]){
                    tmp = (tmp + 1) % 3;
                }
            }
            first_state[i][j] = tmp;
            count[tmp]++;
            REP(k, 3){
                if(count[k] == 4){
                    used[k] = 1;
                }
            }
        }
    }
    return first_state;
}

// 卓組の情報 tablestate から直接対決の回数を計算しその vector を返す.
vvll matchcalc(vvll &tablestate){
    vvll res(n, vll(n));
    REP(i, m){
        REP(a, n){
            REP(b, n){
                if(b <= a || tablestate[i][a] == -1) continue;
                if(tablestate[i][a] == tablestate[i][b]){
                    res[a][b]++;
                    res[b][a]++;
                }
            }
        }
    }
    return res;
}

// 卓組の出力
void print_state(vvll &table_state){
    cout << "Which table each player is sitting at." << endl;
    cout << "    ";
    REP(i, m){
        cout << std::setw(3) << i + 1;
        cout << " ";
    }
    cout << endl;
    cout << "-------------------------------------------------------------" << endl;
    REP(j, n){
        cout << std::setw(3) << j + 1 << ":";
        REP(i, m){
            cout << std::setw(3) << table_state[i][j];
            cout << " ";
        }
        cout << endl;
    }
}

// 対戦回数の出力
void print_matching(vvll &match_count){
    REP(a, n){
        REP(b, n){
            cout << match_count[a][b] << " ";
        }
        cout << endl;
    }
}

// 同卓回数が 3 を超えた分を計算
ll violation(vvll &table_state){
    ll res = 0;
    vvll match = matchcalc(table_state);
    REP(a, n){
        REP(b, n){
            if(match[a][b] > 3){
                res += match[a][b] - 3;
            }
        }
    }
    return res;
}

// 現在の状態から violation が小さくなるような変更を行う.
vvll update(vvll &current_state){
    vvll curmatch = matchcalc(current_state);
    vvll next_state = current_state;
    ll provisional_violation = violation(current_state);
    vll ma, mi; // a とマッチングしすぎている選手とあまりマッチングしていない選手の添え字集合
    vvll res;
    REP(a, n){
        REP(idx, n){
            REP(x, 6){
                if(curmatch[a][idx] == 9 - x){
                    ma.push_back(idx);
                }
            }
        }
        REP(idx, n){
            REP(x, 3){
                if(curmatch[a][idx] == x){
                    mi.push_back(idx);
                }
            }
        }
        for(auto p : ma){
            for(auto q : mi){
                REP(i, m){
                    if(current_state[i][a] == -1 || current_state[i][p] == -1 || current_state[i][q] == -1) continue;
                    if(current_state[i][a] == current_state[i][p] && current_state[i][a] != current_state[i][q]){
                        next_state[i][p] = next_state[i][q];
                        next_state[i][q] = next_state[i][p];
                        if(violation(next_state) < provisional_violation){
                            provisional_violation = violation(next_state);
                            res = next_state;
                        }
                        next_state[i][p] = current_state[i][p];
                        next_state[i][q] = current_state[i][q];
                    }
                }
            }
        }
    }
    return res;
}

// 初期状態に対して update の実行
vvll climbing(vvll &first_state){
    vvll currentstate = first_state;
    REP(cnt, 15){
        currentstate = update(currentstate);
        //cout << violation(currentstate) << endl;
        if(violation(currentstate) == 0) break;
    }
    return currentstate;
}

int main(){
    cin.tie(0);
    ios::sync_with_stdio(false);

    std::srand( time(NULL) );

    vvll absent = make_absent(); // absent[j] : i 節目で j は抜け番.
    vvll first_state = initialize(absent); // first_state[i][j] : i 節目で j が座っている卓.

    vvll final_state = climbing(first_state); 
    vvll final_match_set = matchcalc(final_state); // final_match_set[a][b] : a と　b の直接対決の回数.
    
    print_state(final_state);
    cout << "-----------------------------" << endl;
    print_matching(final_match_set);
    cout << endl;
    return 0;
}
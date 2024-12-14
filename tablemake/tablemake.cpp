#include <bits/stdc++.h>
using namespace std;

// 定数定義
constexpr int NUM_PLAYERS = 14; // 参加人数
constexpr int NUM_ROUNDS = 12; // 節数
constexpr int MAX_TABLE_SIZE = 4; // 卓の最大人数
constexpr int MAX_MATCHES = 3; // 許容される最大対戦回数
constexpr double INITIAL_TEMPERATURE = 5000.0; // 焼きなまし法の初期温度を増加
constexpr double COOLING_RATE = 0.995; // 冷却率を緩やかに
constexpr int UPDATE_TRIALS = 10; // 各温度ステップで複数回の試行

using ll = long long;
using vll = vector<ll>;
using vvll = vector<vector<ll>>;
using P = pair<ll, ll>;

// 関数宣言
vvll make_absent();
vvll initialize(const vvll &absent);
vvll matchcalc(const vvll &table_state);
ll calculate_penalty(const vvll &table_state);
vvll update(const vvll &current_state);
vvll simulated_annealing(const vvll &initial_state);
void print_table(const vvll &table_state);
void print_matching(const vvll &match_set);

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    srand(time(nullptr));

    vvll absent = make_absent();
    vvll initial_state = initialize(absent);

    vvll final_state = simulated_annealing(initial_state);

    if (calculate_penalty(final_state) > 0) {
        cout << "Failed to find an optimal table arrangement.\n";
    } else {
        cout << "Success! Optimal table arrangement found.\n\n";
    }

    vvll final_match_set = matchcalc(final_state);
    print_table(final_state);
    cout << "-----------------------------\n";
    print_matching(final_match_set);

    return 0;
}

// 抜け番の位置を設定
vvll make_absent() {
    vvll absent(NUM_ROUNDS, vll(NUM_PLAYERS, 0));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 2; ++j) {
            absent[i][2 * i + j] = 1;
        }
    }
    for (int j = 0; j < 6; ++j) {
        absent[4][8 + j] = 1;
    }
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 2; ++j) {
            absent[5 + i][2 * i + j] = 1;
        }
    }
    return absent;
}

// 初期状態の生成
vvll initialize(const vvll &absent) {
    vvll first_state(NUM_ROUNDS, vll(NUM_PLAYERS, -1));
    for (int i = 0; i < NUM_ROUNDS; ++i) {
        vector<int> table_count(3, 0);
        for (int j = 0; j < NUM_PLAYERS; ++j) {
            if (absent[i][j]) continue;
            int table;
            do {
                table = rand() % 3;
            } while (table_count[table] >= MAX_TABLE_SIZE);
            first_state[i][j] = table;
            table_count[table]++;
        }
    }
    return first_state;
}

// 対戦回数を計算
vvll matchcalc(const vvll &table_state) {
    vvll match(NUM_PLAYERS, vll(NUM_PLAYERS, 0));
    for (int i = 0; i < NUM_ROUNDS; ++i) {
        for (int a = 0; a < NUM_PLAYERS; ++a) {
            for (int b = a + 1; b < NUM_PLAYERS; ++b) {
                if (table_state[i][a] != -1 && table_state[i][a] == table_state[i][b]) {
                    match[a][b]++;
                    match[b][a]++;
                }
            }
        }
    }
    return match;
}

// ペナルティ計算
ll calculate_penalty(const vvll &table_state) {
    vvll match = matchcalc(table_state);
    ll penalty = 0;
    for (int a = 0; a < NUM_PLAYERS; ++a) {
        for (int b = 0; b < NUM_PLAYERS; ++b) {
            if (match[a][b] > MAX_MATCHES) {
                penalty += (match[a][b] - MAX_MATCHES);
            }
        }
    }
    return penalty;
}

// 更新処理
vvll update(const vvll &current_state) {
    vvll next_state = current_state;
    int round = rand() % NUM_ROUNDS;
    int player_a = rand() % NUM_PLAYERS;
    int player_b = rand() % NUM_PLAYERS;

    if (next_state[round][player_a] != -1 && next_state[round][player_b] != -1) {
        swap(next_state[round][player_a], next_state[round][player_b]);
    }

    return next_state;
}

// 焼きなまし法
vvll simulated_annealing(const vvll &initial_state) {
    vvll current_state = initial_state;
    vvll best_state = initial_state;
    ll best_penalty = calculate_penalty(best_state);

    double temperature = INITIAL_TEMPERATURE;

    while (temperature > 1.0) {
        for (int trial = 0; trial < UPDATE_TRIALS; ++trial) { // 各温度ステップで複数回試行
            vvll candidate_state = update(current_state);
            ll current_penalty = calculate_penalty(current_state);
            ll candidate_penalty = calculate_penalty(candidate_state);

            if (candidate_penalty < current_penalty ||
                exp((current_penalty - candidate_penalty) / temperature) > (rand() / (double)RAND_MAX)) {
                current_state = candidate_state;
            }

            if (candidate_penalty < best_penalty) {
                best_state = candidate_state;
                best_penalty = candidate_penalty;
            }
        }

        temperature *= COOLING_RATE;
    }

    return best_state;
}

// 卓組の出力
void print_table(const vvll &table_state) {
    cout << "Table arrangement for each round:\n";
    for (int i = 0; i < NUM_ROUNDS; ++i) {
        cout << "Round " << i + 1 << ": ";
        for (int j = 0; j < NUM_PLAYERS; ++j) {
            cout << (table_state[i][j] == -1 ? "-" : to_string(table_state[i][j])) << " ";
        }
        cout << endl;
    }
}

// 対戦回数の出力
void print_matching(const vvll &match_set) {
    cout << "Match counts between players:\n";
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        for (int j = 0; j < NUM_PLAYERS; ++j) {
            cout << match_set[i][j] << " ";
        }
        cout << endl;
    }
}

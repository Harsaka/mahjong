#include <bits/stdc++.h>
using namespace std;
#define REP(i, n) for(int i = 0; i < n; i++)
#define REPR(i, n) for(int i = n - 1; i >= 0; i--)
#define FOR(i, m, n) for(int i = m; i <= n; i++)
#define FORR(i, m, n) for(int i = m; i >= n; i--)
#define SORT(v, n) sort(v, v+n);
#define VSORT(v) sort(v.begin(), v.end());
#define VSORTR(v) sort(v.rbegin(), v.rend());
#define ALL(v) (v).begin(),(v).end()
 
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vector<ll>>;
using P = pair<ll, ll>;


// 清一色14枚の和了判定
bool is_finished(string hand) {
    // 事前条件 : 手牌は14枚
    if (hand.size() > 14) {
        cout << "Ta-Hai" << endl;
        return 0;
    }
    else if (hand.size() < 14) {
        cout << "Sho-Hai" << endl;
        return 0;
    }
    REP(i, 14) {
        if (hand[i] - '0' < 1 || hand[i] - '0' > 9) {
            cout << "Input integers in [1, 9]" << endl;
            return 0;
        }
    }
    bool res = true;
    map<ll, ll> mp;
    REP(i, 14) {
        mp[(hand[i] - '0')]++;
    }
    for (auto x : mp) {
        if (x.second > 4) {
            cout << "!!IKASAMA!!: there are 4 cards of same number at most." << endl;
            return 0;
        }
    }
    return res;
}

int main(){
	cin.tie(0);
    ios::sync_with_stdio(false);

    cout << "Input your hand using number {1 ~ 9}:";
    string hand;
    cin >> hand;
    if (is_finished(hand)) {
        cout << "Finished" << endl;
    }
    else {
        cout << "Not finished" << endl;
    }
    return 0;
}
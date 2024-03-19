#include <iostream>
#include <vector>

#define MAX_N 50
#define MAX_M 13
using namespace std;
typedef pair<int, int> pii;

int N, M;

int ans = 987654321;
int arr[MAX_M];
int hcnt;
vector<pii> hpos, ppos;

int getDistance() {
    int ret = 0;
    
    for (pii pp : ppos) {
        int md = 987654321;

        for (int i = 0; i < M; i++) {
            md = min(md, abs(pp.first - hpos[arr[i]].first) + abs(pp.second - hpos[arr[i]].second));
        }

        ret += md;
    }
    return ret;
}

void backtrack(int depth, int start) {
    if (depth == M) {
        ans = min(ans, getDistance());
        return;
    }
    if (start == hcnt - 1) return;

    for (int i = start + 1; i < hcnt; i++) {
        arr[depth] = i;
        backtrack(depth + 1, i);
    }


}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int tmp; cin >> tmp;
            if (tmp == 1) ppos.push_back({ i,j });
            else if (tmp == 2) hpos.push_back({ i,j });
        }
    }
    hcnt = hpos.size();
    backtrack(0, -1);
    cout << ans;
    return 0;
}
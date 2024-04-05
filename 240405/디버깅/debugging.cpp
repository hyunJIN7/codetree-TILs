#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <tuple>

#define MAX_H 30
#define MAX_N 10

using namespace std;

int N, H;
bool grid[MAX_H][MAX_N];
vector<pair<int, int>> candi;
int ans = 10;

bool InRange(int x) {
    return 0 <= x && x < N;
}

bool Verify() {

    for (int j = 0; j < N; j++) {
        int id = j;
        for (int i = 0; i < H; i++) {
            if (grid[i][id]) {
                id++;
            }
            else if (id && grid[i][id - 1]) {
                id--;
            }
        }
        if (id != j) return false;
    }
    return true;
}

void FindMin(int id,int cnt) {
    if (Verify()) {
        ans = min(ans, cnt);
    }
    if (cnt > 3) return;

    for (int i = id; i < candi.size(); i++) {
        int y, x;
        tie(y, x) = candi[i];

        //선 겹침 
        if (InRange(x - 1) && grid[y][x - 1]) continue;
        if (InRange(x + 1) && grid[y][x + 1]) continue;

        grid[y][x] = true;
        FindMin(i +1, cnt + 1);
        grid[y][x] = false;
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int M;
    cin >> N >> M >> H;
    while (M--) {
        int a, b;
        cin >> a >> b;
        grid[a - 1][b - 1] = true;
    }

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j]) continue;
            candi.push_back({ i,j });
        }
    }

    FindMin(0, 0);

    if (ans > 3) ans = -1;
    cout << ans;

    return 0;
}
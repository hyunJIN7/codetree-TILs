#include <iostream>
#include <tuple>

#define MAX_N 100
#define MAX_K 100000
using namespace std;

int H, W, K;
int grid[MAX_N+1][MAX_N+1];

int v[MAX_K + 1], dir[MAX_K + 1], s[MAX_K + 1];//속력,방향,크기

int dy[4] = { -1,1,0,0 }, dx[4] = { 0,0,1,-1 };

bool InRange(int y, int x) {
    return 0 <= y && y < H && 0 <= x && x < W;
}

pair<int, int> GetNextPos(int y, int x, int id) {
    int dis = v[id];
    while (dis--) {
        if (!InRange(y + dy[dir[id]], x + dx[dir[id]])) {
            dir[id] = (dir[id] % 2) ? dir[id] - 1 : dir[id] + 1;
        }
        y += dy[dir[id]];
        x += dx[dir[id]];
    }
    return { y,x };
}

int GetNextY(int y, int id, int &d) {
    if (!dy[d]) return y;
    for (int i = 1; i <= v[id]; i++) {
        if (0 > y + dy[d] || H <= y + dy[d]) { //범위 밖이라면
            d = (d % 2 == 1) ? d - 1 : d + 1;
        }
        y += dy[d];
    }
    return y;
}

int GetNextX(int x, int id, int& d) {
    if (!dx[d]) return x;
    for (int i = 1; i <= v[id]; i++) {
        if (0 > x + dx[d] || W <= x + dx[d]) { //범위 밖이라면
            d = (d % 2==1) ? d - 1 : d + 1;
        }
        x += dx[d];
    }
    return x;
}

void Move() {
    int next_grid[MAX_N][MAX_N] = { 0, };
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (!grid[i][j]) continue;
            int id = grid[i][j];

            int ny, nx;
            tie(ny, nx) = GetNextPos(i, j, id);

            if (next_grid[ny][nx] > 0) {
                // 다른 곰팡이 존재
                int nid = next_grid[ny][nx];
                if (s[id] < s[nid]) id = nid;
            }
            next_grid[ny][nx] = id;
        }
    }

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            grid[i][j] = next_grid[i][j];
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> H >> W >> K;
    for (int i = 1; i <= K; i++) {
        int y, x, _v, d, _s;
        cin >> y >> x >> _v >> d >> _s;
        grid[y-1][x-1] = i;
        v[i] = _v;
        dir[i] = d-1;
        s[i] = _s;
    }

    int ans = 0;
    for (int j = 0; j < W; j++) {
        for (int i = 0; i < H; i++) {
            if (grid[i][j]>0) {
                //곰팡이 잡기 
                ans += s[grid[i][j]];
                grid[i][j] = 0;
                break;
            }
        }
        Move();
    }
    cout << ans;

    return 0;
}
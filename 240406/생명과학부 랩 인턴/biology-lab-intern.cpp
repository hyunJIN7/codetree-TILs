#include <iostream>

#define MAX_N 100
#define MAX_K 100000
using namespace std;

int H, W, K;
int grid[MAX_N+1][MAX_N+1];

int v[MAX_K + 1], dir[MAX_K + 1], s[MAX_K + 1];//속력,방향,크기

int dy[4] = { -1,1,0,0 }, dx[4] = { 0,0,1,-1 };

int GetNextY(int y, int id, int &d) {
    for (int i = 1; i <= v[id]; i++) {
        if (0 > y + dy[d] || H <= y + dy[d]) { //범위 밖이라면
            d = (d % 2 == 1) ? d - 1 : d + 1;
        }
        y += dy[d];
    }
    return y;
}

int GetNextX(int x, int id, int& d) {
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
            //방향은 원본 방향주고 범위 벗어나면 바꾸기
            int ny = GetNextY(i, id, dir[id]);
            int nx = GetNextX(j, id, dir[id]);

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
                //곰팡이 이동
                //Move();
                break;
            }
        }
        Move();
    }
    cout << ans;

    return 0;
}
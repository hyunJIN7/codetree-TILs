#include <iostream>
#include <queue>
#include <cstring>
#include <tuple>

#define MAX_N 20

using namespace std;

int N;
int grid[MAX_N][MAX_N];

int ry, rx;
int level = 2, cnt = 0;

int dis[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

int FindTarget() {
    memset(dis, 0, sizeof(dis));
    memset(vis, 0, sizeof(vis));

    int dy[4] = { -1,0,0,1 }, dx[4] = { 0,-1,1,0 };

    queue<pair<int, int> > q;
    q.push({ ry,rx });
    vis[ry][rx] = true;
    while (!q.empty()) {
        int y, x;
        tie(y, x) = q.front(); q.pop();

        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i], nx = x + dx[i];
            if (!InRange(ny, nx) || vis[ny][nx] || grid[ny][nx] > level)
                continue;
            if (0 < grid[ny][nx] && grid[ny][nx] < level) {
                grid[ny][nx] = 0;
                ry = ny, rx = nx;
                return dis[y][x] + 1;
            }
                
            vis[ny][nx] = true;
            dis[ny][nx] = dis[y][x] + 1;
            q.push({ ny,nx });
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 9) {
                grid[i][j] = 0;
                ry = i, rx = j;
            }
        }
    }

    int ans = 0;
    while (1) {
        int t = FindTarget();
        if (t == -1) break;
        ans += t;
        
        if (++cnt == level) {
            level++;
            cnt = 0;
        }
    }
    cout << ans;
    return 0;
}
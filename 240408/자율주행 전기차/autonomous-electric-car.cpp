#include <iostream>
#include <cstring>
#include <queue>
#include <tuple>
#define MAX_N 20
#define MAX_M 400

#define WALL -1
using namespace std;
typedef pair<int, int> pii;
 
int N, M;

int grid[MAX_N][MAX_N];

int dist[MAX_N][MAX_N] = { 0, };
bool vis[MAX_N][MAX_N] = { 0, };

int cy, cx;// 차 위치
int battery = 0;

int sy[MAX_M + 1], sx[MAX_M + 1];
int ey[MAX_M + 1], ex[MAX_M + 1];


bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

bool CanGo(int y, int x) {
    return InRange(y, x) && !vis[y][x] && grid[y][x] != WALL;
}

int dy[4] = { -1,0,0,1 }, dx[4] = { 0,-1,1,0 };

int FindTarget() {
    // 현재 위치에서 가까운 승객 찾기 
    memset(dist, 0, sizeof(dist));
    memset(vis, 0, sizeof(vis));

    queue<pii> q;
    q.push({ cy,cx });
    vis[cy][cx] = true;

    //타켓 
    int ty = N, tx = N, min_dist = battery + 10;

    while (!q.empty()) {
        int y, x, cur_dist;
        tie(y, x) = q.front(), q.pop();
        cur_dist = dist[y][x];

        if (battery <= cur_dist || min_dist < cur_dist) break;
        
        if (grid[y][x] > 0) {
            //사람 있는데 
            if (cur_dist < min_dist) {
                min_dist = cur_dist;
                ty = y, tx = x;
            }
            else if (cur_dist == min_dist) {
                if (make_pair(y, x) < make_pair(ty, tx)) {
                    ty = y, tx = x;
                }
            }
        }

        for (int d = 0; d < 4; d++) {
            int ny = y + dy[d], nx = x + dx[d];
            if (!CanGo(ny, nx)) continue;
            vis[ny][nx] = true;
            dist[ny][nx] = cur_dist + 1;
            q.push({ ny,nx });
        }
    }

    if (battery <= min_dist) return -1;
    battery -= min_dist;
    return grid[ty][tx];
}

bool Simulate() {
    int id = FindTarget();
    if (id == -1) return false;
    int ty = sy[id], tx = sx[id];

    memset(dist, 0, sizeof(dist));
    memset(vis, 0, sizeof(vis));

    queue<pii> q;
    q.push({ ty,tx });
    vis[ty][tx] = true;

    while (!q.empty()) {
        int y, x;
        tie(y, x) = q.front(); q.pop();
        if (y == ey[id] && x == ex[id]) break;
        if (battery < dist[y][x]) break;

        for (int d = 0; d < 4; d++) {
            int ny = y + dy[d], nx = x + dx[d];
            if (!CanGo(ny, nx)) continue;
            vis[ny][nx] = true;
            dist[ny][nx] = dist[y][x] + 1;
            q.push({ ny,nx });
        }
    }
    int mind = dist[ey[id]][ex[id]];
    if (!vis[ey[id]][ex[id]] || battery < mind) return false;
    cy = ey[id], cx = ex[id];
    battery += mind;
    grid[ty][tx] = 0;
    return true;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> M >> battery;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
            grid[i][j] *= -1;
        }
    }
    cin >> cy >> cx;
    --cy, --cx;
    for (int i = 1; i <= M; i++) {
        int s_y, s_x, e_y, e_x;
        cin >> s_y >> s_x >> e_y >> e_x;
        sy[i] = --s_y, sx[i] = --s_x;
        ey[i] = --e_y, ex[i] = --e_x;
        grid[s_y][s_x] = i;
    }

    for (int i = 1; i <= M; i++) {
        if (!Simulate()) {
            battery = -1;
            break;
        }
    }
    cout << battery;

    return 0;
}
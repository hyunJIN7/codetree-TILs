#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#define MAX_N 71
#define DIR_NUM 4
using namespace std;

bool grid[MAX_N][MAX_N];
int N, M;

vector<vector<int> > frame = { {0,0},{1,0},{0,-1},{0,1},{-1,0} };

//이동 위해 확인해야할 위치
vector<vector<int> > south = { {1,0},{2,0},{1,-1},{1,1} };
vector<vector<int> > west = { {2,-1},{1,-2},{1,-1},{-1,0},{0,-2},{-1,-1} };
vector<vector<int> > east = { {2,1},{1,2},{1,1},{1,0},{0,2}, {-1,1} };

int dy[DIR_NUM] = { -1,0,1,0 }, dx[DIR_NUM] = { 0,1,0,-1 };

struct Pos { int y, x, d; };

bool InRange(int y, int x) {
    return 1 <= y && y <= N && 1 <= x && x <= M;
}
bool InSide(int y, int x) {
    return 1 < y && y < N && 1 < x && x < M;
}
bool CanGo(int y, int x, vector<vector<int> > &candi) {
    for (vector<int> p : candi) {
        int ny = y + p[0];
        int nx = x + p[1];
        if ( !(0 <= ny && ny <= N && 1 <= nx && nx <= M) || grid[ny][nx])
        //if (!InRange(ny,nx) || grid[ny][nx])
            return false;
        //if (!InRange(y+p[0], x+p[1]) || grid[y+p[0]][x + p[1]])
        //    return false;
        // 범위 밖이거나 이미 놓여있다면 못감
    }
    return true;
}

Pos FindNextPos(Pos pos) {
    //남쪽이동 
    if (CanGo(pos.y, pos.x, south)) {
        pos.y += 1;
        pos = FindNextPos(pos);
    }
    //서쪽이동
    else if (CanGo(pos.y, pos.x, west)) {
        pos.y += 1;
        pos.x -= 1;
        pos.d = (pos.d + DIR_NUM - 1) % DIR_NUM;
        pos = FindNextPos(pos);
    }
    //동쪽이동
    else if (CanGo(pos.y, pos.x, east)) {
        pos.y += 1;
        pos.x += 1;
        pos.d = (pos.d + 1) % DIR_NUM;
        pos = FindNextPos(pos);
    }
    return pos;
}

int Simulate(int x, int d) {
    int y = 0;
    Pos p = FindNextPos({ 0, x, d });

    if (!InSide(p.y, p.x)) {
        memset(grid, 0, sizeof(grid));
        return 0;
    }
    //범위 내에 가능 
    y = p.y, x = p.x, d = p.d;

    int ret = y + 1; // 현재 골렘에서 가장 큰 y
    bool vis[MAX_N][MAX_N] = { 0, };
    queue<pair<int, int> > q;
    q.push({ y+ dy[d],x+dx[d] }); // 출구 방향
    while (!q.empty()) {
        pair<int, int> cp = q.front();
        q.pop();
        ret = max(ret, cp.first);

        for (int d = 0; d < DIR_NUM; d++) {
            int ny = cp.first + dy[d], nx = cp.second + dx[d];
            if (InRange(ny, nx) && grid[ny][nx] && !vis[ny][nx]) {
                vis[ny][nx] = true;
                q.push({ ny,nx });
            }
        }
    }

    //마지막에 골렘 조각 활성화 
    //출구랑 연결된 다른 조각만 보기 위해 나중에 활성화 함.
    for (vector<int> np : frame) {
        //int ny = y + np[0];
        //int nx = x + np[1];
        grid[y + np[0]][x + np[1]] = true;
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int K;
    cin >> N >> M >> K;
    int ans = 0;
    while (K--) {
        int c, d;
        cin >> c >> d;
        ans += Simulate(c, d);
    }
    cout << ans;
    return 0;
}
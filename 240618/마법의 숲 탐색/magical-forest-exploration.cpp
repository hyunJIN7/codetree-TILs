#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#define MAX_N 73
#define DIR_NUM 4
#define GATE -1
using namespace std;

int grid[MAX_N][MAX_N];
//bool gate[MAX_N][MAX_N];
int N, M;

vector<vector<int> > frame = { {0,0},{1,0},{0,-1},{0,1},{-1,0} };

//이동 위해 확인해야할 위치
vector<vector<int> > south = { {1,0},{2,0},{1,-1},{1,1} };
vector<vector<int> > west = { {2,-1},{1,-2},{-1,0},{0,-2},{-1,-1} };
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
        if ( !( ny <= N && 1 <= nx && nx <= M) || grid[ny][nx])
            return false;
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

int MoveFairy(int y, int x, int d) {
    //step 3. 정령 이동 
    //출구 위치에서 시작해
    // 이미 활성화된 곳으로만 이동 
    int ret = y + 1;
    bool vis[MAX_N][MAX_N] = { 0, };
    queue<pair<int, int> > q;
    q.push({ y + dy[d],x + dx[d] }); // 출구 방향
    vis[y + dy[d]][x + dx[d]] = true;

    while (!q.empty()) {
        pair<int, int> cp = q.front(); q.pop();
        ret = max(ret, cp.first);

        for (int d = 0; d < DIR_NUM; d++) {
            int ny = cp.first + dy[d], nx = cp.second + dx[d];
            //갈 수 있는 칸 중에서 
            if (!InRange(ny, nx) || vis[ny][nx] || !grid[ny][nx])
                continue;
            if ( (grid[ny][nx] == grid[cp.first][cp.second])|| 
                grid[cp.first][cp.second] == GATE || 
                grid[ny][nx] == GATE) {
                vis[ny][nx] = true;
                q.push({ ny,nx });
            }
        }
    }
    return ret;
}

int Simulate(int x, int d, int id) {

    // step 1. 남서동 이동
    int y = 0;
    Pos p = FindNextPos({ -1, x, d });

    //이동 위치가 격자 벗어나면 새로 시작 
    if (!InSide(p.y, p.x)) {
        memset(grid, 0, sizeof(grid));
        return 0;
    }
    //골렘 최종 위치 
    y = p.y, x = p.x, d = p.d;

    grid[y + dy[d]][x + dx[d]] = GATE;
    int ret = MoveFairy(y, x, d);


    //step 2. 골렘 활성화 
   // 본인 골렘 이외 골렘만 보기 위해 뒤로 뺌.
    for (vector<int> np : frame) {
        grid[y + np[0]][x + np[1]] = id;
    }
    grid[y + dy[d]][x + dx[d]] = GATE;

    //step 3. 정령 이동 
    //출구 위치에서 시작해
    // 이미 활성화된 곳으로만 이동 
    return ret;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int K;
    cin >> N >> M >> K;
    int ans = 0;
    int id = 1;
    while (K--) {
        int c, d;
        cin >> c >> d;
        ans += Simulate(c, d,id++);
    }
    cout << ans;
    return 0;
}
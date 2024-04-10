#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <tuple>

#define MAX_N 10
#define MAX_M 10
using namespace std;

int N, M, K;

int turn = 0;
int  grid_skill[MAX_N][MAX_M];
int  grid_time[MAX_N][MAX_M];

int dy[] = { 0,1,0,-1,1,1,-1,-1 };
int dx[] = { 1,0,-1,0,1,-1,-1,1 };

struct node {
    int skill, time, y, x;
};


bool cmp(node a, node b) {
    if (a.skill != b.skill) return a.skill < b.skill;
    if (a.time != b.time) return a.time > b.time;
    if (a.y + a.x != b.y + b.x) return a.y + a.x > b.y + b.x;
    return a.y > b.y;
}

vector<node> candi;
int back_y[MAX_N][MAX_M];
int back_x[MAX_N][MAX_M];

bool vis[MAX_N][MAX_M];
bool play[MAX_N][MAX_M];

void init() {
    turn++;
    candi.clear();
    memset(play, 0, sizeof(play));
    memset(vis, 0, sizeof(vis));
}
void FindTarget() {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid_skill[i][j] <= 0) continue;
            candi.push_back({ grid_skill[i][j] , grid_time[i][j],i,j });
        }
    }
    sort(candi.begin(), candi.end(), cmp);

    play[candi[0].y][candi[0].x] = true;
    grid_time[candi[0].y][candi[0].x] = turn;
}

bool LaserAttack() {
    int sy = candi[0].y, sx = candi[0].x;
    int ey = candi.back().y, ex = candi.back().x;

    queue<pair<int, int> > q;
    q.push({ sy,sx });
    vis[sy][sx] = true;
    while (!q.empty()) {
        int y, x;
        tie(y, x) = q.front(); q.pop();

        if (y == ey && x == ex) break;

        for (int d = 0; d < 4; d++) {
            int ny = (y + dy[d] + N) % N;
            int nx = (x + dx[d] + M) % M;
            if (grid_skill[ny][nx] <= 0 || vis[ny][nx]) continue;

            q.push({ ny,nx });
            vis[ny][nx] = true;
            back_y[ny][nx] = y;
            back_x[ny][nx] = x;
        }
    }

    if (!vis[ey][ex]) return false; // 길 못찾음

    int skill = (grid_skill[sy][sx] += (N + M));
    grid_skill[ey][ex] -= skill;
    skill /= 2;
    play[ey][ex] = true;
    int ny = back_y[ey][ex], nx = back_x[ey][ex]; // 다음 위치
    while (ny != sy || nx != sx) {
        grid_skill[ny][nx] -= skill;
        play[ny][nx] = true;
        ey = ny, ex = nx;
        ny = back_y[ey][ex], nx = back_x[ey][ex];
    }
    return true; // 공격 성공 시 
}

void BombAttack() {
    int sy = candi[0].y, sx = candi[0].x;
    int ey = candi.back().y, ex = candi.back().x;

    int skill = (grid_skill[sy][sx] += (N + M));
    grid_skill[ey][ex] -= skill;
    skill /= 2;
    play[ey][ex] = true;

    for (int d = 0; d < 8; d++) {
        int ny = (ey + dy[d] + N) % N;
        int nx = (ex + dx[d] + M) % M;
        if (grid_skill[ny][nx] <= 0) continue;
        if (ny == sy && nx == sx) continue;
        //공격자 아니라면 조건 추가 
        grid_skill[ny][nx] -= skill;
        play[ny][nx] = true;
    }
}
void Upgrade() {
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < M; j++) 
            if (!play[i][j] && grid_skill[i][j] > 0) 
                grid_skill[i][j]++;  
}

void Simulate() {
    while (turn < K) {
        //init
        init();
        //공격자 선정
        FindTarget();
        if (candi.size() <= 1) return;

        //굳이 여기서 확인하는건 
        // 포탑 정비해도 어차피 가담한 애들은 안더해서 때문에 안더해져 
        //공격
        if (!LaserAttack()) {
            BombAttack();
        }

        //ENd 확인 
        //포탑 정비
        Upgrade();
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> grid_skill[i][j];

    Simulate();

    int ans = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            ans = max(ans, grid_skill[i][j]);
    cout << ans;
    return 0;
}
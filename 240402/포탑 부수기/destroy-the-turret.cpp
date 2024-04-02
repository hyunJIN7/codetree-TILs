#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#define MAX_N 10
#define DIR_NUM 4
using namespace std;

int N, M;
int grid_power[MAX_N][MAX_N];
int grid_time[MAX_N][MAX_N] = { 0, };

// 방향만 적으면 안됨. 범위 밖이면 순환되기 때문에 
int back_y[MAX_N][MAX_N], back_x[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];

bool play = true;
int turn = 0;
int dy[] = { 0,1,0,-1, -1,1,1,-1 }, dx[] = { 1,0,-1,0,1,1,-1,-1 };

struct node {
    //2중 for, pair 로 비교하면 TLE
    int y, x, t, p;
};

bool cmp(node a, node b) {
    if (a.p != b.p) return a.p < b.p;
    if (a.t != b.t) return a.t > b.t;//최근 공격한것 앞으로
    if (a.y + a.x != b.y + b.x) return a.y + a.x > b.y + b.x;
    return a.y > b.y;
}
vector<node> vec;

void FindAttacker() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid_power[i][j] <= 0) continue;
            vec.push_back({ i,j,grid_time[i][j],grid_power[i][j] });
        }
    }
    sort(vec.begin(), vec.end(), cmp);
    int y = vec[0].y, x = vec[0].x;
    vec[0].p += N + M;
    vec[0].t = turn;
    grid_power[y][x] += N + M;
    grid_time[y][x] = turn;
}

bool LaserAttack() {
    // 시작 ,끝 위치 
    int sy = vec[0].y, sx = vec[0].x;
    int ey = vec.back().y, ex = vec.back().x;

    queue<pair<int, int> > q;
    q.push({ sy,sx });
    vis[sy][sx] = true;
    while (!q.empty()) {
        int y = q.front().first, x = q.front().second;
        q.pop();

        if (ey == y && ex == x) break;

        for (int d = 0; d < DIR_NUM; d++) {
            int ny = (y + dy[d] + N) % N;
            int nx = (x + dx[d] + M) % M;
            if (vis[ny][nx] || !grid_power[ny][nx]) continue;
            vis[ny][nx] = true;
            q.push({ ny,nx });
            back_y[ny][nx] = y;
            back_x[ny][nx] = x;
        }
    }

    if (!vis[ey][ex])return false;

    memset(vis, 0, sizeof(vis));
    vis[sy][sx] = vis[ey][ex] = true;
    int attack_power = grid_power[sy][sx];
    grid_power[ey][ex] -= attack_power;
    if (grid_power[ey][ex] < 0) grid_power[ey][ex] = 0;
    attack_power /= 2;

    while (1) {
        int ny = back_y[ey][ex], nx = back_x[ey][ex];
        vis[ny][nx] = true;
        if (sy == ny && sx == nx) break;
        grid_power[ny][nx] -= attack_power;
        if (grid_power[ny][nx] < 0) grid_power[ny][nx] = 0;
        ey = ny, ex = nx;
    }
    return true;
}
void BombAttack() {
    int sy = vec[0].y, sx = vec[0].x;
    int ey = vec.back().y, ex = vec.back().x;
    vis[sy][sx] = vis[ey][ex] = true;

    int attack_power = grid_power[sy][sx];
    grid_power[ey][ex] -= attack_power;
    if (grid_power[ey][ex] < 0) grid_power[ey][ex] = 0;
    attack_power /= 2;
    for (int d = 0; d < 8; d++) {
        int ny = (ey + dy[d] + N) % N, nx = (ex + dx[d] + M) % M;
        vis[ny][nx] = true;
        grid_power[ny][nx] -= attack_power;
        if (grid_power[ny][nx] < 0) grid_power[ny][nx] = 0;
    }
}

void AddPower() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid_power[i][j] > 0 && !vis[i][j]) {
                grid_power[i][j]++;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int K;
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> grid_power[i][j];
    while (K--) {
        vec.clear();
        memset(vis, 0, sizeof(vis));
        memset(back_y, 0, sizeof(back_y));
        memset(back_x, 0, sizeof(back_x));
        turn++;

        //step1
        FindAttacker();
        if (vec.size() <= 1) break;

        if (!LaserAttack()) {
            memset(vis, 0, sizeof(vis));
            BombAttack();
        }

        //점수 추가 
        AddPower();

    }
    int ret = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            ret = max(ret, grid_power[i][j]);
    cout << ret;
    return 0;
}
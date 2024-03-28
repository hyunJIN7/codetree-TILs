#include <iostream>
#include <cstring>
#define MAX_N 20
#define DIR_NUM 4
#define SUM 7
using namespace std;

int N;

int grid[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];

int u = 1, f = 2, r = 3;
int cy = 0, cx = 0;
int dir = 1;
int score = 0;

int dy[DIR_NUM] = { -1,0,1,0 }, dx[DIR_NUM] = { 0,1,0,-1 };

bool Inrange(int y, int x) {
    return 0 <= x && x < N && 0 <= y && y < N;
}

void RoateDice() {
    int nu, nf, nr;

    if (dir == 0) {
        nu = f, nf = SUM - u, nr = r;
    }
    else if (dir == 1) {
        nu = SUM - r, nf = f, nr = u;
    }
    else if (dir == 2) {
        nu = SUM - f, nf = u, nr = r;
    }
    else if (dir == 3) {
        nu = r, nf = f, nr = SUM - u;
    }
    u = nu, f = nf, r = nr;
}

void Move() {
    cy += dy[dir], cx += dx[dir];
    if (!Inrange(cy, cx)) {
        dir = (dir + 2) % DIR_NUM;
        cy += dy[dir] * 2, cx += dx[dir] * 2;;
    }
    RoateDice();
}

int DFS(int y, int x, int val) {
    int ret = grid[y][x];
    for (int d = 0; d < DIR_NUM; d++) {
        int ny = y + dy[d], nx = x + dx[d];
        if (!Inrange(ny, nx) || 
            visited[ny][nx] ||
            grid[ny][nx] != val) continue;
        visited[ny][nx] = true;
        ret += DFS(ny, nx, val);
    }
    return ret;
}

void GetScore() {
    //현재 주사위가 놓인 위치에서 
    memset(visited, 0, sizeof(visited));
    visited[cy][cx] = true;
    score += DFS(cy, cx, grid[cy][cx]);
}

void SelecDir() {
    int d = SUM - u;
    if (grid[cy][cx] < d) 
        dir = (dir + 1) % DIR_NUM;
    else if (grid[cy][cx] > d) 
        dir = (dir + DIR_NUM - 1) % DIR_NUM;
}

void Simulate() {
    //주사위 이동
    // 이동하며 범위 밖이면 반대로 이동
    Move();
    //점수 계산
    GetScore();
    //방향 결정
    SelecDir();
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int M;
    cin >> N >> M;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) 
            cin >> grid[i][j];

    while (M--) {
        Simulate();
    }
    cout << score;

    return 0;
}
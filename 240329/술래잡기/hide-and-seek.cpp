#include <iostream>
#include <vector>

#define MAX_N 99
#define MAX_M 9802
#define DIR_NUM 4
using namespace std;

int N, M;
vector<int> grid[MAX_N][MAX_N];
vector<int> next_grid[MAX_N][MAX_N];

bool tree[MAX_N][MAX_N] = { 0, };

int cy = N/2, cx=N/2, cd; // 술래 위치

struct Runner {
    int y, x, d;
}runner[MAX_M];

int score = 0;
int turn = 1;

int dy[] = { -1,0,1,0 }, dx[] = { 0,1,0,-1 };

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void MoveRunner(int y,int x) {
    if (grid[y][x].size() == 0) return;



    for (int e : grid[y][x]) {
        int d = runner[e].d;
        int ny = y + dy[d], nx = x + dx[d];

        if(!InRange(ny, nx)) {
            d = (d + 2) % DIR_NUM;
            ny = y + dy[d], nx = x + dx[d];
        }
        if ((ny == cy && nx == cx)) {
            next_grid[y][x].push_back(e);
            continue;
        }
        //술래 없음. 이동
        next_grid[ny][nx].push_back(e);
        runner[e] = { ny,nx,d };
    }
    grid[y][x].clear();
}

int line, cnt;
bool flip;
void MoveChaser(bool flip) {
    int dt = flip ? 3 : 1;
    cy += dy[cd], cx += dx[cd];
    cnt++;
    if (line == cnt) {
        cd = (cd + dt) % DIR_NUM;
        cnt = 0;
        //뒤집었을 땐 홀수에서 cnt 감소
        //false일땐 짝수일때 증가 
        if (flip) {
            if (cd % 2)line--;
            if (cy == N - 1 && cx == 0)
                line = N - 1;
        }
        else if (!(cd % 2))
            line++;
    }

}

void Catch() {
    for (int i = 0; i < 3; i++) {
        int ny = cy + dy[cd] * i, nx = cx + dx[cd] * i;
        if (!InRange(ny,nx) ||  tree[ny][nx] || !grid[ny][nx].size()) continue;
        score += turn * grid[ny][nx].size();
        grid[ny][nx].clear();
    }
}

void Simulate() {

    ////step 1 도망자 도망 
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            if (abs(i) + abs(j) > 3) continue;
            int ny = cy + i, nx = cx + j;
            if (!InRange(ny, nx)) continue;
            MoveRunner(ny,nx);
        }
    }
    //next -> gird
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int e : next_grid[i][j]) {
                grid[i][j].push_back(e);
            }
            next_grid[i][j].clear();
        }
    }

    //step2 술래 출발 
    if (cy == N / 2 && cx == N / 2) {
        //중앙이라면
        line = 1; 
        cnt = 0;
        cd = 0;// 술래 방향 
        flip = false;
    }
    else if (!cy && !cx) {
        line = N-1;
        cnt = 0;
        cd = 2;
        flip = true;
    }
    MoveChaser(flip);
    Catch();
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int H, K;
    cin >> N >> M >> H >> K;
    for (int i = 0; i < M; i++) {
        int y, x, d;
        cin >> y >> x >> d;
        grid[--y][--x].push_back(i);
        runner[i] = { y,x,d };
    }
    while (H--) {
        int y, x;
        cin >> y >> x;
        tree[y-1][x-1] = true;
    }

    cy = cx = N / 2;
    turn = 1;
    for (turn = 1; turn <= K; turn++) {
        Simulate();
    }

    cout << score;

    return 0;
}
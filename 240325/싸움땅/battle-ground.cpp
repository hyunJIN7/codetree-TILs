#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>

#define MAX_N 20
#define MAX_M 30
#define DIR_NUM 4
using namespace std;

int N, M, K;

priority_queue<int> grid_gun[MAX_N][MAX_N];
int grid_player[MAX_N][MAX_N];
//TODO : 이것도 벡터로 해야하나?!??!!??!


int dy[DIR_NUM] = { -1,0,1,0 };
int dx[DIR_NUM] = { 0,1,0,-1 };

int score[MAX_M + 1] = { 0, };

struct Player {
    int y, x, dir, skill, gun;

    void UpdatePos(int _y, int _x, int _dir) {
        y = _y;
        x = _x;
        dir = _dir;
    }

    int GetAbility() {
        return skill + gun;
    }

}player[MAX_M + 1];// 1부터 시작!!1

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void ChangeGun(int y, int x, int pid) {
    if (grid_gun[y][x].size() == 0) return;

    int from = player[pid].gun;
    int to = grid_gun[y][x].top();

    if (to <= from) return;
    grid_gun[y][x].pop();
    player[pid].gun = to;
    if(from) grid_gun[y][x].push(from);
}

void Lose(int y, int x, int id) {
    // 진 사람 총 내려 놓고 이동 
    if (player[id].gun) {
        grid_gun[y][x].push(player[id].gun);
        player[id].gun = 0;
    }
    int dir = player[id].dir;
    for (int i = 0; i < DIR_NUM; i++, dir = (dir + 1) % DIR_NUM) {
        int ny = y + dy[dir], nx = x + dx[dir];
        if (!Inrange(ny, nx) || grid_player[ny][nx]) continue;
        //사람 없으면 바로 끝
        grid_player[ny][nx] = id;
        player[id].UpdatePos(ny, nx, dir);
        ChangeGun(ny, nx, id);
        break;
    }
}

void Fight(int y, int x, int p2) {
    int p1 = grid_player[y][x];

    int s1 = player[p1].GetAbility();
    int s2 = player[p2].GetAbility();

    //p1이 winner 가 되도록 
    if (s1 < s2 || (s1 == s2 && player[p1].skill < player[p2].skill)) {
        swap(s1, s2);
        swap(p1, p2);
    }
    grid_player[y][x] = p1;
    score[p1] += s1 - s2;
    Lose(y, x, p2);

    //이긴사람 점수, 총 교환
    ChangeGun(y, x, p1);
}

void Simulate() {
    for (int i = 1; i <= M; i++) {
        int y = player[i].y, x = player[i].x;
        int dir = player[i].dir;
        int ny = y + dy[dir], nx = x + dx[dir];
        if (!Inrange(ny, nx)) {
            dir = (dir + 2) % DIR_NUM;
            ny = y + dy[dir], nx = x + dx[dir];
        }
        grid_player[y][x] = 0;
        player[i].UpdatePos(ny, nx, dir);

        //y = ny, x = nx;
        if (grid_player[ny][nx]) {
            //옮겨간 곳에 사람 있음
            
            Fight(ny, nx, i);

        }
        else if (grid_gun[ny][nx].size()) {
            //총만 바꾼다.
            ChangeGun(ny,nx,i);
            grid_player[ny][nx] = i;
        }
        else grid_player[ny][nx] = i;

    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> M >> K;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int g; cin >> g;
            grid_gun[i][j].push(g);
        }
    }
    for (int i = 1; i <= M; i++) {
        int y, x, d, s;
        cin >> y >> x >> d >> s;
        player[i] = { y-1, x-1, d, s ,0};
    }

    while (K--) {
        Simulate();
    }

    for (int i = 1; i <= M; i++) cout << score[i] << " ";

    return 0;
}
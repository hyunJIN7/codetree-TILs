#include <iostream>
#include <tuple>
#include <cstring>

#define MAX_N 50
#define MAX_P 30
#define EMPTY make_pair(-1,-1)
#define RUDOLF -1
using namespace std;
typedef pair<int, int> pii;

int N, M, P, C, D;

int grid[MAX_N][MAX_N] = { 0, };
bool visited[MAX_N][MAX_N] = { 0, };

int score[MAX_P + 1] = { 0, };
int rest[MAX_P + 1] = { 0, };

pii sp[MAX_P + 1], rp;

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

int GetDistance(pii a, pii b) {
    return (a.first - b.first) * (a.first - b.first)
        + (a.second - b.second) * (a.second - b.second);
}

int dy[] = { -1, 0, 1, 0,-1,1, 1,-1 };
int dx[] = {  0, 1, 0,-1, 1,1,-1,-1 };


void Crush(int y,int x,int dr,int dc,int dis,int id =-1) {
    // id가 y,x에서 dis만큼 밀려나야하는 상황 
    // yx에 있던 산타가 밀리고 연쇄 반응까지
    if(id==-1) id = grid[y][x];

    // 다음 위치 
    int ny = y + dr * dis, nx = x + dc * dis;
    if (!Inrange(ny, nx)) { // 범위 밖
        pii& p = sp[id];
        grid[p.first][p.second] = 0;
        p = EMPTY; // 죽음 
        return;
    }
    if (grid[ny][nx] > 0) { // 다른 산타가 있다면 
        Crush(ny, nx, dr, dc, 1);
    }
    grid[ny][nx] = id;
    sp[id] = { ny,nx };
}

void RuMove() {
    // 가장 가까운 산타 찾기 
    int min_dis = 987654321;
    int s_y = 0, s_x = 0; // 가장 가까운 산타 위치
    for (int i = N - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            if (grid[i][j] <= 0) continue;
            //산타랑만 거리 구해
            int dis = GetDistance(rp,{i,j});
            if (dis < min_dis) {
                s_y = i, s_x = j;
                min_dis = dis;
            }
        }
    }

    // 8 방향 중 가까워지는 방향 찾기  
    min_dis = 987654321;
    int min_y = 0, min_x = 0;
    int dir;
    for (int d = 0; d < 8; d++) {
        int ny = rp.first + dy[d];
        int nx = rp.second + dx[d];
        if (!Inrange(ny, nx)) continue;
        int dis = GetDistance({ ny,nx }, { s_y,s_x });
        if (dis < min_dis) {
            min_y = ny, min_x = nx;
            min_dis = dis;
            dir = d;
        }
    }
    //범위 벗어나진 않을겨 아마
    //충돌
    if (grid[min_y][min_x] > 0) {
        int id = grid[min_y][min_x];
        rest[id] = 2;
        score[id] += C;
        Crush(min_y, min_x, dy[dir], dx[dir], C);
    }
    grid[rp.first][rp.second] = 0;
    grid[min_y][min_x] = RUDOLF;
    rp = { min_y,min_x };
}

void SanMove(int id) {
    int dir = -1;
    int min_dis = GetDistance(sp[id], rp);
    int min_y, min_x;
    for (int d = 0; d < 4; d++) {
        int ny = sp[id].first + dy[d];
        int nx = sp[id].second + dx[d];
        if (!Inrange(ny, nx) || grid[ny][nx] > 0) continue;
        int dis = GetDistance(rp, {ny,nx});
        if (dis < min_dis) {
            min_dis = dis;
            dir = d;
            min_y = ny, min_x = nx;
        }
    }
    if (dir == -1) return; //갈곳없음.
    if ( grid[min_y][min_x] == RUDOLF) {
        score[id] += D;
        rest[id] = 2;
        grid[sp[id].first][sp[id].second] = 0;
        Crush(min_y, min_x, -dy[dir], -dx[dir] , D, id);
    }
    else {
        pii& p = sp[id];
        grid[p.first][p.second] = 0; // 이동만
        grid[min_y][min_x] = id;
        p = { min_y,min_x };

    }
}

void Simulate() {
    // 기절 감소 처리
    for (int i = 1; i <= P; i++) {
        if (rest[i]) 
            rest[i]--;
    }

    //step 1 루돌프 이동
    RuMove();
    //step2 산타 이동
    for (int i = 1; i <= P; i++) {
        if (rest[i] || sp[i] == EMPTY) continue;
        // 안쉬고 살아있는 산타만 
        SanMove(i);
    }
}

bool End() {
    for (int id = 1; id <= P; id++) {
        if (sp[id] != EMPTY) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    cin >> N >> M >> P >> C >> D;
    int y, x;
    cin >> y >> x;
    rp = { --y, --x };
    grid[y][x] = RUDOLF;
    for (int i = 0, id; i < P; i++) {
        cin >> id >> y >> x;
        sp[id] = { --y, --x };
        grid[y][x] = id;
    }

    while (M--) {
        Simulate();


        // 매턴 끝나면 살아있는 것들 점수 추가 
        for (int id = 1; id <= P; id++) {
            if (sp[id] != EMPTY) 
                score[id]++;
        }
        if (End()) break;
    }
    for (int i = 1; i <= P; i++)
        cout << score[i] << " ";

    return 0;
}
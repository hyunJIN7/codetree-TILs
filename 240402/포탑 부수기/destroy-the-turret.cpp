#include <iostream>
#include <cstring>
#include <queue>
#include <tuple>
#define MAX_N 10
#define DIR_NUM 4
using namespace std;

int N, M;
int grid_power[MAX_N][MAX_N];
int grid_time[MAX_N][MAX_N] = {0,};
int grid_dir[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
bool is_attack[MAX_N][MAX_N];
bool play = true;
int turn = 1;
int dy[] = { 0,1,0,-1, -1,1,1,-1 }, dx[] = { 1,0,-1,0,1,1,-1,-1 };

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < M;
}

pair<int, int> apos, dpos; // 공격,방어 위치 

void FindTarget() {
    pair<int, int> ainfo = { 987654321 ,0};//공격력,시간 
    //시간에 음수 붙인다. 
    for (int i = N - 1; i >= 0; i--) {
        for (int j = M - 1; j >= 0; j--) {
            if (!grid_power[i][j]) continue;
            if (make_pair(grid_power[i][j], -grid_time[i][j]) < ainfo) {
                ainfo = { grid_power[i][j], -grid_time[i][j] };
                apos = { i,j };
            }
        }
    }
    grid_time[apos.first][apos.second] = turn;
    grid_power[apos.first][apos.second] += N + M;

    pair<int, int> dinfo = {0,0};//공격력,시간 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (!grid_power[i][j]) continue;
            if (dinfo < make_pair(grid_power[i][j], -grid_time[i][j])) {
                dinfo = { grid_power[i][j], -grid_time[i][j] };
                dpos = { i,j };
            }
        }
    }
}

bool LaserAttack() {
    queue<pair<int, int> > q;
    q.push(apos);
    visited[apos.first][apos.second] = true;
    while (!q.empty()) {
        pair<int, int> cur = q.front(); q.pop();
        if (cur == dpos) break;

        for (int d = 0; d < DIR_NUM; d++) {
            int ny = (cur.first + dy[d]) % N;
            int nx = (cur.second + dx[d]) % M;
            if (visited[ny][nx] || !grid_power[ny][nx]) continue;
            visited[ny][nx] = true;
            q.push({ ny,nx });
            grid_dir[ny][nx] = d;
        }
    }

    int y, x;
    tie(y, x) = dpos;
    
    if(!visited[y][x])return false;
  
    memset(visited, 0, sizeof(visited));
    visited[y][x] = true;
    int attack_power = grid_power[apos.first][apos.second];
    grid_power[y][x] -= attack_power;
    attack_power /= 2;

    while (1) {
        //다음 위치
        int dir = (grid_dir[y][x] + 2) % DIR_NUM;
        y += dy[dir], x += dx[dir];
        visited[y][x] = true;
        if (apos == make_pair(y, x)) break;
        grid_power[y][x] -= attack_power;
    }
    return true;
}
void BombAttack() {
    int y, x;
    tie(y, x) = dpos;
    visited[y][x] = true;
    visited[apos.first][apos.second] = true;
    int attack_power = grid_power[apos.first][apos.second];
    grid_power[y][x] -= attack_power;
    attack_power /= 2;
    for (int d = 0; d < 8; d++) {
        int ny = (y + dy[d]) % N, nx = (x + dx[d]) % M;
        visited[ny][nx] = true;
        grid_power[ny][nx] -= attack_power;
    }
}

bool End() {
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid_power[i][j] > 0) cnt++;
            else grid_power[i][j] = 0;
            if (cnt >= 2) return false;
        }
    }
    return true;
}

void AddPower() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid_power[i][j] > 0 && !visited[i][j]) {
                grid_power[i][j]++;
            }
        }
    }
}

void Simulate() {
    //대상 선정
    FindTarget();
    //위치 값 어떻게 넘겨줄지 

    //공격
    memset(visited, 0, sizeof(visited));
    memset(grid_dir, -1, sizeof(grid_dir));
    if (!LaserAttack()) {
        memset(visited, 0, sizeof(visited));
        BombAttack();
    }
        

    if (End()) {
        play = false;
        return;
    }

    //점수 추가 
    AddPower();

}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    //pair<int, int> a = { 1,3 }, b = { 1,3 };
    //if (b < a) cout << "바귐";

    int K;
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> grid_power[i][j];
    while (turn <= K && play) {
        Simulate();
        turn++;
    }
    int ret = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (ret < grid_power[i][j]) ret = grid_power[i][j];
    cout << ret;
    return 0;
}
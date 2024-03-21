#include <iostream>
#include <tuple>
#include <queue>
#include <climits>
#include <cstring>

#define MAX_N 15
#define MAX_M 30
#define DIR_NUM 4
#define EMPTY make_pair(-1,-1)

using namespace std;
typedef pair<int, int> pii;

int N, M;

int grid[MAX_N][MAX_N]; //0빈칸,1베켐,2는 못가

pii player[MAX_M];
pii store[MAX_M];

int curr_t = 0;

// 문제에서의 우선순위인 상좌우하 순으로 적어줍니다.
int dx[DIR_NUM] = { -1,  0, 0, 1 };
int dy[DIR_NUM] = { 0, -1, 1, 0 };

int step[MAX_N][MAX_N];      // 최단거리 결과 기록
bool visited[MAX_N][MAX_N];  // 방문 여부 표시

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

bool CanGo(int y, int x) {
    return Inrange(y,x) && !visited[y][x] && grid[y][x] != 2 ;
        
}

void BFS(pii &pos) {
    memset(step, 0, sizeof(step));
    memset(visited, 0, sizeof(visited));

    queue<pii> q;
    q.push(pos);
    visited[pos.first][pos.second] = true;

    while (!q.empty()) {
        int cy, cx;
        tie(cy, cx) = q.front(); q.pop();
        
        for (int d = 0; d < DIR_NUM; d++) {
            int ny = cy + dy[d], nx = cx + dx[d];
            if (CanGo(ny, nx)) {
                step[ny][nx] = step[cy][cx] + 1;
                visited[ny][nx] = true;
                q.push({ ny,nx });
            }

        }
    }
}

void Simulate() {
    //step1 이동
    for (int i = 0; i < M; i++) {
        //단 도착안했고,엠티아니고 그런 플레이어만 
        if (player[i] == EMPTY || player[i] == store[i]) continue;
        
        
        BFS(store[i]);
        
        //주변 돌며 이동할 위치 찾기 

        int min_step = 987654321;
        int miny, minx;

        for (int d = 0; d < DIR_NUM; d++) {
            int ny = player[i].first + dy[d];
            int nx = player[i].second + dx[d];
            if (Inrange(ny,nx) && visited[ny][nx] && min_step > step[ny][nx]) {
                min_step = step[ny][nx];
                miny = ny, minx = nx;
            }
        }
        player[i] = { miny, minx };
        if (player[i] == store[i]) grid[miny][minx] = 2;
    }

    // step2 . 베이스 캠프 배정
    if (curr_t >= M) return;
    BFS(store[curr_t]);

    int min_dis = 987654321;
    int miny, minx;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (visited[i][j] && grid[i][j] == 1 && step[i][j] < min_dis) {
                min_dis = step[i][j];
                miny = i, minx = j;
            }

        }
    }
    player[curr_t] = { miny,minx };
    grid[miny][minx] = 2;

}

bool End() {
    for (int i = M; i >= 0; i--) {
        if (player[i] != store[i]) return false;
    }
    return true;
}
int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    for (int i = 0; i < M; i++) {
        int y, x;
        cin >> y >> x;
        store[i] = {y-1,x-1 };
        player[i] = EMPTY;
    }
        
    while (true) {
        
        Simulate();
        curr_t++;
        if (End()) break;
    }
    cout << curr_t;

    return 0;
}
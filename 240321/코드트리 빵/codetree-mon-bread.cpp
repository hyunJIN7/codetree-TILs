#include <iostream>
#include <tuple>
#include <queue>

#define MAX_N 15
#define MAX_M 30
#define DIR_NUM 4
using namespace std;
typedef pair<int, int> pii;

int N, M;
int remain;

int grid[MAX_N][MAX_N];
bool on[MAX_N][MAX_N];

pii store[MAX_M + 1], player[MAX_M + 1];

int dy[] = { -1,0,0,1 };
int dx[] = { 0,-1,1,0 };

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void FindBaseCamp(int id) {
    bool visited[MAX_N][MAX_N] = { 0, };
    queue<pii> q;
    q.push(store[id]);
    visited[store[id].first][store[id].second] = true;
    while (!q.empty()) {
        int cy, cx;
        tie(cy, cx) = q.front();
        q.pop();

        for (int d = 0; d < DIR_NUM; d++) {
            int ny = cy + dy[d];
            int nx = cx + dx[d];
            if (!Inrange(ny, nx) || visited[ny][nx] || on[ny][nx]) continue;

            if (grid[ny][nx]) {
                on[ny][nx] = true;
                player[id] = { ny,nx };
                return;
            }
            q.push({ ny,nx });
            visited[ny][nx] = true;
        }
    }
}

int getDistance(pii a, pii b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

void Move(int id) {
    // 이건 도착 안한 것만 호출됨.
    //현위치에서 편의점과 가까워지는 곳으로 
    int y, x;
    tie(y, x) = player[id];
    int min_dis = getDistance(player[id], store[id]);
    for (int d = 0; d < DIR_NUM; d++) {
        int ny = player[id].first + dy[d];
        int nx = player[id].second + dx[d];

        if (!Inrange(ny, nx) || on[ny][nx]) continue;
        int dis = getDistance(store[id], { ny,nx });
        if (dis < min_dis) {
            min_dis = dis;
            y = ny, x = nx;
        }
    }

    //마지막에 이동하고 편의점이랑 같은 곳이면 remain--하고 
    player[id] = { y,x };
    if (player[id] == store[id]) {
        //도착한 상태라면
        on[y][x] = true;
        remain--;
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }
    for (int i = 1; i <= M; i++) {
        cin >> store[i].first >> store[i].second;
        store[i].first--;
        store[i].second--;
    }
    remain = M;

    int t = 0;
    while (remain) {
        //베켐 선정
        
        //player 전진 
        for (int i = 1; i <= min(t, M); i++) {
            if (player[i] == store[i]) continue;
            Move(i);
        }
        t++;
        if (t <= M) {
            FindBaseCamp(t);
        }  
    }
    cout << t;
    return 0;
}
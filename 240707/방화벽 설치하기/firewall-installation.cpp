#include <iostream>
#include <vector>
#include <queue>
#define MAX_N 8
using namespace std;
typedef pair<int, int> pii;

int N, M;
int grid[MAX_N][MAX_N];
int ans = 0;

vector<pii> fire_vec, empty_vec;

int dy[] = { 0,0,-1,1 }, dx[] = { 1,-1,0,0 };


bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < M;
}

int Simulate() {
    int ret = empty_vec.size() - 3;
    bool vis[MAX_N][MAX_N] = { 0, };
    queue<pii> q;
    for (pii p : fire_vec) {
        q.push(p);
    }
    while (!q.empty()) {
        auto [y, x] = q.front(); q.pop();
        for (int d = 0; d < 4; d++) {
            int ny = y + dy[d], nx = x + dx[d];
            if (!InRange(ny, nx) || vis[ny][nx] || grid[ny][nx]) continue;
            //빈칸만
            vis[ny][nx] = true;
            q.push({ ny,nx });
            ret--;
        }
    }
    return ret;
}

void Bruteforce(int id =-1, int cnt=0) {
    if (cnt == 3) {
        ans = max(ans, Simulate());
        return;
    }

    for (id += 1; id < empty_vec.size(); id++) {
        auto [y, x] = empty_vec[id];
        grid[y][x] = 1;
        Bruteforce(id, cnt + 1);
        grid[y][x] = 0;
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 2) 
                fire_vec.push_back({ i,j });
            else if (grid[i][j] == 0) 
                empty_vec.push_back({ i,j });
        }
    }

    Bruteforce();
    cout << ans;

    return 0;
}
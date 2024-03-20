#include <iostream>
#include <cmath>
#define MAX_N 64
#define DIR_NUM 4
using namespace std;

int n, N;
int grid[MAX_N][MAX_N];
int next_grid[MAX_N][MAX_N];

bool visited[MAX_N][MAX_N] = { 0, };


bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void RotatePart(int y, int x, int len) { // 한변 길이 
    int sy[DIR_NUM] = { 0, 0,     len / 2, len / 2 };
    int sx[DIR_NUM] = { 0, len / 2, len / 2,   0 };

    for (int d = 0; d < DIR_NUM; d++) {
        int nd = (d + 1) % DIR_NUM;
        for (int i = 0; i < len / 2; i++) {
            //int cy = sy[d] + i + y;
            //int ny = sy[nd] + i + y;
            for (int j = 0; j < len / 2; j++) {
                int cy = sy[d] + i + y;
                int ny = sy[nd] + i + y;
                int cx = sx[d] + j + x;
                int nx = sx[nd] + j + x;
                next_grid[ny][nx] = grid[cy][cx];
            }
        }
    }
}

void CopyGrid() {
    //next에서 현재 grid로 옮기기 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = next_grid[i][j];
        }
    }
}

void RotateAll(int level) {
    int unit = pow(2, level);
    for (int i = 0; i < N; i += unit) {
        for (int j = 0; j < N; j += unit) {
            RotatePart(i, j, unit);
        }
    }
    CopyGrid();
}
int dy[DIR_NUM] = { 0,0,1,-1 };
int dx[DIR_NUM] = { 1,-1,0,0 };
void Melt() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!grid[i][j]) continue;
            int cnt = 0;
            for (int d = 0; d < DIR_NUM; d++) {
                int ny = i + dy[d];
                int nx = j + dx[d];
                if (!Inrange(ny, nx) || !grid[ny][nx]) continue;
                cnt++;
            }
            next_grid[i][j] = (cnt < 3) ? grid[i][j] - 1 :  grid[i][j];
        }
    }
    CopyGrid();
}

int DFS(int y, int x) {
    // 아 이거 어쩌지 아무튼 
    int ret = 1;
    for (int d = 0; d < DIR_NUM; d++) {
        int ny = y + dy[d];
        int nx = x + dx[d];
        if (!Inrange(ny, nx) || visited[ny][nx] || !grid[ny][nx])
            continue;
        visited[ny][nx] = true;
        ret += DFS(ny, nx);
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int q;
    cin >> n >> q;
    N = pow(2, n);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    while (q--) {
        int l; cin >> l;
        if (l) RotateAll(l);
        Melt();
    }

    // 전체 탐색하며 군집 찾고 맥스 군집 
    // 남아 있는 빙하 총양과 가장 큰 군집 크기 리턴
    int total = 0, max_cnt = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            total += grid[i][j];
            if (visited[i][j] || !grid[i][j]) continue;
            visited[i][j] = true;
            max_cnt = max(max_cnt, DFS(i, j));
        }
    }
    cout << total << "\n" << max_cnt;
    return 0;
}
#include <iostream>
#include <cstring>
#define MAX_N 30
#define MAX_G 841
#define DIR_NUM 4
using namespace std;

int N;
int grid[MAX_N][MAX_N];
int next_grid[MAX_N][MAX_N];
int group[MAX_N][MAX_N];

int gid = 0;
int group_cnt[MAX_G+1];

bool visited[MAX_N][MAX_N];

int dy[4] = { 0,-1,0,1 };
int dx[4] = { 1,0,-1,0 };

bool inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void dfs(int y, int x, int val) {
    for (int d = 0; d < DIR_NUM; d++) {
        int ny = y + dy[d];
        int nx = x + dx[d];
        if (!inrange(ny, nx) || visited[ny][nx] || grid[ny][nx] != val) continue;
        visited[ny][nx] = true;
        group[ny][nx] = gid;
        group_cnt[gid]++;
        dfs(ny, nx , val);
    }
}

void makeGroup() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (visited[i][j]) continue;
            visited[i][j] = true;
            group[i][j] = gid;
            group_cnt[gid]++;
            dfs(i, j, grid[i][j]);
            gid++;
        }
    }
}

int getScore() {
    int ret = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int g1 = group[i][j];
            for (int d = 0; d < DIR_NUM; d++) {
                int ny = i + dy[d], nx = j + dx[d];
                if (inrange(ny, nx) && grid[ny][nx] != grid[i][j]) {
                    int g2 = group[ny][nx];
                    ret += (group_cnt[g1] + group_cnt[g2]) * grid[i][j] * grid[ny][nx];
                }
            }

        }
    }
    return ret / 2;
}
void rotateCross(int y,int x) { // 중심좌표
    for (int d = 0; d < DIR_NUM; d++) {
        int nd = (d + 1) % DIR_NUM;

        for (int i = 0; i <= N/2; i++) {
            int cy = y + dy[d] * i, cx = x + dx[d] * i;
            int ny = y + dy[nd] * i, nx = x + dx[nd] * i;

            next_grid[ny][nx] = grid[cy][cx];
        }
    }
}
void rotateSquare(int fy,int fx) {


    for (int i = 0; i < N / 2; i++) {
        for (int j = 0; j < N / 2; j++) {
            next_grid[fy + j][fx + N/2 - 1 - i ] = grid[fy + i][fx + j];
        }
    }

}

int solve() {
    int score = 0;
    for (int i = 0; i < 4; i++) {
        gid = 0;
        memset(group_cnt, 0, sizeof(group_cnt));
        memset(visited, 0, sizeof(visited));
        memset(group, 0, sizeof(group));

        //그룹 만들기
        makeGroup();
        //점수 구하기
        score += getScore();
        //회전
        rotateCross(N/2,N/2);
        rotateSquare(0, 0);
        rotateSquare(0, N / 2 + 1);
        rotateSquare(N / 2 + 1, 0);
        rotateSquare(N / 2 + 1, N / 2 + 1);
        //grid에 next grid 값 넣기 
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                grid[i][j] = next_grid[i][j];
            }
        }
    }
    return score;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N;
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < N; j++) 
            cin >> grid[i][j];
      
    cout << solve();
    return 0;
}
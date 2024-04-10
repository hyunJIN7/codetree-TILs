#include <iostream>
#include <vector>

#define MAX_N 20
#define MAX_M 5
#define DIR_NUM 4

using namespace std;

int n, m, k;
int board[MAX_N + 1][MAX_N + 1];

// 각 팀별 레일 위치를 관리합니다.
vector<pair<int, int> > v[MAX_M + 1];
// 각 팀별 tail 위치를 관리합니다.
int tail[MAX_M + 1];
bool visited[MAX_N + 1][MAX_N + 1];

// 격자 내 레일에 각 팀 번호를 적어줍니다.
int board_idx[MAX_N + 1][MAX_N + 1];

int ans;

int dx[DIR_NUM] = { -1, 0, 1, 0 };
int dy[DIR_NUM] = { 0, -1, 0, 1 };

bool IsOutRange(int x, int y) {
    return !(1 <= x && x <= n && 1 <= y && y <= n);
}

// 초기 레일을 만들기 위해 dfs를 이용합니다.
void DFS(int x, int y, int idx) {
    visited[x][y] = true;
    board_idx[x][y] = idx;
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (IsOutRange(nx, ny)) continue;

        // 이미 지나간 경로거나 경로가 아니면 넘어갑니다.
        if (board[nx][ny] == 0) continue;
        if (visited[nx][ny]) continue;

        // 가장 처음 탐색할 때 2가 있는 방향으로 dfs를 진행합니다.
        if ((int)v[idx].size() == 1 && board[nx][ny] != 2) continue;

        v[idx].push_back({ nx, ny });
        if (board[nx][ny] == 3) tail[idx] = (int)v[idx].size();
        DFS(nx, ny, idx);
    }
}

// 입력을 받는 등 초기 작업을 합니다.
void Init() {
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cin >> board[i][j];

    int cnt = 1;

    // 레일을 벡터에 저장합니다. 머리사람을 우선 앞에 넣어줍니다.
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (board[i][j] == 1) v[cnt++].push_back({ i, j });

    // dfs를 통해 레일을 벡터에 순서대로 넣어줍니다.
    for (int i = 1; i <= m; i++)
        DFS(v[i][0].first, v[i][0].second, i);
}

// 각 팀을 이동시키는 함수입니다.
void MoveAll() {
    for (int i = 1; i <= m; i++) {
        // 각 팀에 대해 레일을 한 칸씩 뒤로 이동시킵니다.
        pair<int, int> tmp = v[i].back();
        for (int j = (int)v[i].size() - 1; j >= 1; j--)
            v[i][j] = v[i][j - 1];
        v[i][0] = tmp;
    }

    for (int i = 1; i <= m; i++) {
        // 벡터에 저장한 정보를 바탕으로 보드의 표기 역시 바꿔줍니다.
        for (int j = 0; j < (int)v[i].size(); j++) {
            pair<int, int> x = v[i][j];
            if (j == 0)
                board[x.first][x.second] = 1;
            else if (j < tail[i] - 1)
                board[x.first][x.second] = 2;
            else if (j == tail[i] - 1)
                board[x.first][x.second] = 3;
            else
                board[x.first][x.second] = 4;
        }
    }
}

// (x, y) 지점에 공이 닿았을 때의 점수를 계산합니다.
void GetPoint(int x, int y) {
    int idx = board_idx[x][y];
    int cnt = 0;
    for (int i = 0; i < (int)v[idx].size(); i++)
        if (v[idx][i].first == x && v[idx][i].second == y) cnt = i;
    ans += (cnt + 1) * (cnt + 1);
}

// turn 번째 라운드의 공을 던집니다.
// 공을 던졌을 때 이를 받은 팀의 번호를 반환합니다.
int ThrowBall(int turn) {
    int t = (turn - 1) % (4 * n) + 1;

    if (t <= n) {
        // 1 ~ n 라운드의 경우 왼쪽에서 오른쪽 방향으로 공을 던집니다.
        for (int i = 1; i <= n; i++) {
            if (1 <= board[t][i] && board[t][i] <= 3) {
                // 사람이 있는 첫 번째 지점을 찾습니다.
                // 찾게 되면 점수를 체크한 뒤 찾은 사람의 팀 번호를 저장합니다.
                GetPoint(t, i);
                return board_idx[t][i];
            }
        }
    }
    else if (t <= 2 * n) {
        // n+1 ~ 2n 라운드의 경우 아래에서 윗쪽 방향으로 공을 던집니다.
        t -= n;
        for (int i = 1; i <= n; i++) {
            if (1 <= board[n + 1 - i][t] && board[n + 1 - i][t] <= 3) {
                // 사람이 있는 첫 번째 지점을 찾습니다.
                // 찾게 되면 점수를 체크한 뒤 찾은 사람의 팀 번호를 저장합니다.
                GetPoint(n + 1 - i, t);
                return board_idx[n + 1 - i][t];
            }
        }
    }
    else if (t <= 3 * n) {
        // 2n+1 ~ 3n 라운드의 경우 오른쪽에서 왼쪽 방향으로 공을 던집니다.
        t -= (2 * n);
        for (int i = 1; i <= n; i++) {
            if (1 <= board[n + 1 - t][n + 1 - i] && board[n + 1 - t][n + 1 - i] <= 3) {
                // 사람이 있는 첫 번째 지점을 찾습니다.
                // 찾게 되면 점수를 체크한 뒤 찾은 사람의 팀 번호를 저장합니다.
                GetPoint(n + 1 - t, n + 1 - i);
                return board_idx[n + 1 - t][n + 1 - i];
            }
        }
    }
    else {
        // 3n+1 ~ 4n 라운드의 경우 위에서 아랫쪽 방향으로 공을 던집니다.
        t -= (3 * n);
        for (int i = 1; i <= n; i++) {
            if (1 <= board[i][n + 1 - t] && board[i][n + 1 - t] <= 3) {
                // 사람이 있는 첫 번째 지점을 찾습니다.
                // 찾게 되면 점수를 체크한 뒤 찾은 사람의 팀 번호를 저장합니다.
                GetPoint(i, n + 1 - t);
                return board_idx[i][n + 1 - t];
            }
        }
    }
    // 공이 그대로 지나간다면 0을 반환합니다.
    return 0;
}

// 공을 획득한 팀을 순서를 바꿉니다.
void Reverse(int got_ball_idx) {
    // 아무도 공을 받지 못했으면 넘어갑니다.
    if (got_ball_idx == 0) return;

    int idx = got_ball_idx;

    vector<pair<int, int> > new_v;

    // 순서를 맞춰 new_v에 레일을 넣어줍니다.
    for (int i = tail[idx] - 1; i >= 0; i--) {
        pair<int, int> x = v[idx][i];
        new_v.push_back(x);
    }

    for (int i = (int)v[idx].size() - 1; i >= tail[idx]; i--) {
        pair<int, int> x = v[idx][i];
        new_v.push_back(x);
    }

    v[idx] = new_v;

    // 벡터에 저장한 정보를 바탕으로 보드의 표기 역시 바꿔줍니다.
    for (int j = 0; j < (int)v[idx].size(); j++) {
        pair<int, int> x = v[idx][j];
        if (j == 0)
            board[x.first][x.second] = 1;
        else if (j < tail[idx] - 1)
            board[x.first][x.second] = 2;
        else if (j == tail[idx] - 1)
            board[x.first][x.second] = 3;
        else
            board[x.first][x.second] = 4;
    }
}

int main() {
    // 입력을 받고 구현을 위한 기본적인 처리를 합니다.
    Init();

    for (int i = 1; i <= k; i++) {
        // 각 팀을 머리사람을 따라 한칸씩 이동시킵니다.
        MoveAll();

        // i번째 라운드의 공을 던집니다. 공을 받은 사람을 찾아 점수를 추가합니다.
        int got_ball_idx = ThrowBall(i);

        // 공을 획득한 팀의 방향을 바꿉니다.
        Reverse(got_ball_idx);
    }

    cout << ans;
    return 0;
}
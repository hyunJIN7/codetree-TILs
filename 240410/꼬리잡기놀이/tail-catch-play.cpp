#include <iostream>
#include <tuple>
#include <vector>

#define MAX_N 20
#define MAX_M 5
#define WAY 4
#define HEAD 1
#define TAIL 3
#define MID 2
#define DIR_NUM 4
using namespace std;
typedef pair<int, int> pii;

int N, M, K;

int grid[MAX_N][MAX_N];
int grid_id[MAX_N][MAX_N];

vector<pii> line[MAX_M + 1];//그룹별 라인 관리
int tail[MAX_N + 1];//line에서 꼬리 몇번째에 있는지

int score = 0;

int dy[DIR_NUM] = { 0,0,1,-1 };
int dx[DIR_NUM] = { 1,-1,0,0 };
bool vis[MAX_N][MAX_N];

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void MakeGroup(int y,int x, int id) {
    //y,x 다음 위치 찾기
    grid_id[y][x] = id;
    vis[y][x] = true;
    for (int d = 0; d < DIR_NUM; d++) {
        int ny = y + dy[d], nx = x + dx[d];
        if (!InRange(ny, nx) || vis[ny][nx] || grid[ny][nx]==0) continue;

        //몸통 방향으로 넣어야하니까
        if (grid[ny][nx] != MID && line[id].size() == 1) continue;

        //여기 오면
        //몸통/꼬리/길 순차적으로 넣어진다.
        line[id].push_back({ ny,nx });
        if (grid[ny][nx] == TAIL) tail[id] = line[id].size() - 1;

        //다음 위치 찾았으면 다시 진행
        //이 함수는 하나 그룹 길 다 찾을 때 까지 진행됨.
        // line[id]에 하나의 길 정보 위치 다 들어감
        MakeGroup(ny, nx, id);
    }
}

void RenewGrid(int id) {
    int size = line[id].size();
    //line에 순서대로 grid 에 반영
    for (int i = 0; i < size; i++) {
        int y, x;
        tie(y, x) = line[id][i];
        if (i == 0) {
            grid[y][x] = HEAD;
        }
        else if (i < tail[id]) {
            grid[y][x] = MID;
        }
        else if (i == tail[id]) {
            grid[y][x] = MID;
        }
        else {
            grid[y][x] = WAY;
        }
    }
}

void MoveAll() {

    for (int id = 1; id <= M; id++) {
        pii tmp = line[id].back();
        for (int i = line[id].size()-1; i > 0; i--) {
            line[id][i] = line[id][i - 1];//하나씩 땡겨
        }
        line[id][0] = tmp;

        RenewGrid(id);
     
    }
}

void GetScore(int y, int x) {
    int id = grid_id[y][x];
    pii target = { y,x };
    for (int i = 0; i < line[id].size(); i++) {
        if (target == line[id][i]) {
            score += (i + 1) * (i + 1);
            return;
        }
    }
}

void Reverse(int id) {
    int size = line[id].size();
    int s = tail[id];
    vector<pii> newv;
    for (int i = 0; i < size; i++) {
        newv.push_back(line[id][(s - i + size) % size]);
    }
    line[id] = newv;
    RenewGrid(id);
}

int ThrowBall(int turn) {
    int t = turn % (4*N);
    int ret = 0;
    if (t < N) {
        t %= N;
        for (int i = 0; i < N; i++) {
            if (HEAD <= grid[t][i] && grid[t][i] <= TAIL) {
                GetScore(t, i);
                return grid_id[t][i];
            }
        }

    }
    else if (t < 2 * N) {
        t %= N;
        for (int i = N-1; i >= 0; i--) {
            if (HEAD <= grid[i][t] && grid[i][t] <= TAIL) {
                GetScore(i, t);
                return grid_id[i][t];
            }
        }
    }
    else if (t < 3 * N) {
        t %= N;
        for (int i = N - 1; i >= 0; i--) {
            if (HEAD <= grid[N - 1 - t][i] && grid[N - 1 - t][i] <= TAIL) {
                GetScore(N - 1 - t, i);
                return grid_id[N - 1 - t][i];
            }
        }
    }
    else{
        t %= N;
        for (int i = 0; i < N; i++) {
            if (HEAD <= grid[i][N - 1 - t] && grid[i][N - 1 - t] <= TAIL) {
                GetScore(i, N - 1 - t);
                return grid_id[i][N - 1 - t];
            }
        }
    }
    return 0;
}


void Init() {
    cin >> N >> M >> K;
    for (int i = 0, g = 1; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == HEAD) {
                line[g++].push_back({ i,j });
            }
        }
    }

    for (int id = 1; id <= M; id++) {
        MakeGroup(line[id][0].first, line[id][0].second, id);
    }

}
int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    Init();
    for (int turn = 0; turn < K; turn++) {
        MoveAll();
        int id = ThrowBall(turn);
        if (id > 0) {
            Reverse(id);
        }
    }
    cout << score;
    return 0;
}
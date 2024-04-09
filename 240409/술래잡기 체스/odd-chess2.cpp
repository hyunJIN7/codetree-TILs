#include <iostream>
#include <tuple>

#define MAX_N 4
#define MAX_M 16
#define DIR_NUM 8
#define EMPTY make_pair(-1,-1)
#define TAGGER make_pair(-2,-2)
using namespace std;
typedef pair<int, int> pii;
// 도둑 id 방향
pii grid[MAX_N][MAX_N];

constexpr int N = 4, M = 16;

// ↑, ↖, ←, ↙, ↓, ↘, →, ↗ 
int dy[DIR_NUM] = { -1, -1,  0,  1, 1, 1, 0, -1 };
int dx[DIR_NUM] = { 0, -1, -1, -1, 0, 1, 1,  1 };

int ans = 0;

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

bool ThiefCanGo(int y, int x) {
    return InRange(y, x) && grid[y][x] != TAGGER;
}
bool TaggerCanGo(int y, int x) {
    return InRange(y, x) && grid[y][x] != EMPTY;
}

tuple<int, int, int> GetNextPos(int y, int x, int dir) {
    for (int i = 0; i < DIR_NUM; i++) {
        int nd = (dir + i) % DIR_NUM;
        int ny = y + dy[nd], nx = x + dx[nd];
        if (ThiefCanGo(ny, nx))
            return make_tuple(ny, nx, nd);
    }
    return make_tuple(y, x, dir);
}

void MoveOne(int id) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (id == grid[i][j].first) {
                int ny, nx, nd;
                tie(ny, nx, nd) = GetNextPos(i, j, grid[i][j].second);
                grid[i][j].second = nd;

                //swap
                pii tmp = grid[ny][nx];
                grid[ny][nx] = grid[i][j];
                grid[i][j] = tmp;
                return;
            }
        }
    }
}

void MoveAll() {
    for (int id = 1; id <= M; id++)
        MoveOne(id);
}

bool End(int y, int x, int dir) {
    for (int dist = 1; dist < N; dist++) {
        int ny = y + dy[dir] * dist, nx = x + dx[dir] * dist;
        if (TaggerCanGo(ny, nx))
            return false;
    }
    return true;//못간다.
}

void GetMaxScore(int y, int x, int dir, int score) {
    //현 술래 위치 방향과 그 때의 점수
    if (End(y, x, dir)) {
        //현 방향으로 갈 수 있는 곳 없음.
        ans = max(ans, score);
        return; //끝
    }

    for (int dist = 1; dist < N; dist++) {
        int ny = y + dy[dir] * dist, nx = x + dx[dir] * dist;
        if (!TaggerCanGo(ny, nx)) continue;

        pii tmp_grid[MAX_N][MAX_N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                tmp_grid[i][j] = grid[i][j];


        int new_score, new_dir;
        tie(new_score, new_dir) = grid[ny][nx];
        grid[ny][nx] = TAGGER;
        grid[y][x] = EMPTY;

        MoveAll();
        GetMaxScore(ny, nx, new_dir, new_score + score);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                grid[i][j] = tmp_grid[i][j];
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int id, dir;
            cin >> id >> dir;
            grid[i][j] = { id,dir - 1 };
        }
    }

    //태거 초기화
    int init_score, init_dir;
    tie(init_score, init_dir) = grid[0][0];
    grid[0][0] = TAGGER;

    MoveAll();
    GetMaxScore(0, 0, init_dir, init_score);

    cout << ans;
    return 0;
}
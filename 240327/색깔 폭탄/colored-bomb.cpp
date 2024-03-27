#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cstring>

#define MAX_N 20
#define EMPTY -2
#define STONE -1
#define RED 0
#define DIR_NUM 4
using namespace std;
typedef pair<int, int> pii;
int N, M;
int grid[MAX_N][MAX_N];
int next_grid[MAX_N][MAX_N];

bool visited[MAX_N][MAX_N];
int dy[DIR_NUM] = { 0,-1,0,1 };
int dx[DIR_NUM] = { 1,0,-1,0 };

vector<pii> max_bomb;
int max_red = 0;

int score = 0;

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void FindBomb() {

    //한텀 끝나면 또 해야해..?..
    memset(visited, 0, sizeof(visited));
         
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; j < N; j++) {
            if (visited[i][j] || grid[i][j] <= 0) 
                continue;
            memset(visited, 0, sizeof(visited));

            vector<pii> block;
            int red_cnt = 0;

            queue<pii> q;
            q.push({ i,j });
            block.push_back({ i,j });
            visited[i][j] = true;
            int color = grid[i][j];

            while (!q.empty()) {
                int y, x;
                tie(y, x) = q.front(); q.pop();

                for (int d = 0; d < DIR_NUM; d++) {
                    int ny = y + dy[d], nx = x + dx[d];
                    if (!Inrange(ny, nx) || visited[ny][nx]) 
                        continue;
                    if (grid[ny][nx] == color || grid[ny][nx] == RED) {
                        if (grid[ny][nx] == RED) 
                            red_cnt++;
                        q.push({ ny,nx });
                        block.push_back({ ny,nx });
                        visited[ny][nx] = true;
                    }
                }
            }

            // mab bomb랑 비교!!! 
            if (max_bomb.size() < block.size()) {
                max_bomb.clear();
                for (pii p : block)max_bomb.push_back(p);
                max_red = red_cnt;
            }
            else if (max_bomb.size() == block.size() && red_cnt < max_red) {
                max_bomb.clear();
                for (pii p : block)max_bomb.push_back(p);
                max_red = red_cnt;
            }

        }
    }

}

void RemoveBomb() {
    for (pii p : max_bomb) {
        grid[p.first][p.second] = EMPTY;
    }
    max_red = 0;
    max_bomb.clear();
}

void Gravity() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            next_grid[i][j] = EMPTY;
    for (int j = 0; j < N; j++) {
        int idx = N - 1;
        for (int i = N - 1; i >= 0; i--) {
            if (grid[i][j] == EMPTY) continue;
            if (grid[i][j] == STONE) {
                idx = i - 1;//이거 위부터 넣을 수 있으니
                continue;
            }
            next_grid[idx--][j] = grid[i][j];
        }
    }

    for (int i = 0; i < N; i++) 
        for (int j = 0; j < N; j++)
            grid[i][j] = next_grid[i][j];
}

void Rotate() {
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < N; j++) 
            next_grid[j][N-1-i] = grid[i][j];

        
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = next_grid[i][j];
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> M;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) 
            cin >> grid[i][j];
    while (true) {
        FindBomb();
        int c = max_bomb.size();
        if (c <= 1) break;
        //점수 획득
        score += c * c;
        RemoveBomb();

        Gravity();
        Rotate();
        Gravity();
    }

    cout << score;
    return 0;
}
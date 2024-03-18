#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#define MAX_N 50
using namespace std;

int N , L , R;
int grid[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int dy[] = { 0,0,1,-1 }, dx[] = { 1,-1,0,0 };

typedef pair<int, int> pii;
bool inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

bool solve() {
    bool play = false;
    memset(visited, 0, sizeof(visited));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (visited[i][j]) continue;
            queue<pii> q;
            vector<pii> pos;
            int sum = grid[i][j];
            q.push({ i,j });
            pos.push_back({ i,j });
            visited[i][j] = true;

            while (!q.empty()) {
                pii curr = q.front(); q.pop();

                for (int d = 0; d < 4; d++) {
                    int ny = curr.first + dy[d];
                    int nx = curr.second + dx[d];
                    if (!inrange(ny, nx) || visited[ny][nx]) continue;

                    int diff = abs(grid[ny][nx] - grid[curr.first][curr.second]);
                    if (L <= diff && diff <= R) {
                        q.push({ ny,nx });
                        pos.push_back({ ny,nx });
                        visited[ny][nx] = true;
                        sum += grid[ny][nx];
                    }


                }
            }

            if (pos.size() > 1) {
                play = true;
                sum /= (int)pos.size();

                for (pii p : pos) {
                    grid[p.first][p.second] = sum;
                }
            }

        }
    }

    return play;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> L >> R;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }
    
    int test = 0;
    while (solve()) {
        test++;
    }

    cout << test;
    return 0;
}
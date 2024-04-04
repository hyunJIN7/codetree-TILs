#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_N 12
#define MAX_K 10
#define RED 1
#define BLUE 2
using namespace std;

int N, K;
int color[MAX_N][MAX_N]; 
vector<int> grid[MAX_N][MAX_N];

int r[MAX_K], c[MAX_K], dir[MAX_K];

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void Move(int y, int x, vector<int>& vec, bool rever) {
    if (rever) {
        for (int i = 0; i < vec.size(); i++) {
            grid[y][x].push_back(vec[i]);
            r[vec[i]] = y, c[vec[i]] = x;
        }
    }
    else {
        for (int i = vec.size()-1; i >=0; i--) {
            grid[y][x].push_back(vec[i]);
            r[vec[i]] = y, c[vec[i]] = x;
        }
    }
}

bool Simulate() {
    int dy[4] = { 0, 0, -1, 1 };
    int dx[4] = { 1, -1, 0, 0 };

    //4 개 이상이면 true 아님 fasle
    for (int id = 0; id < K; id++) {
        int nd = dir[id];
        int ny = r[id] + dy[nd], nx = c[id] + dx[nd];
        bool reverse = false;

        if (!InRange(ny, nx) || color[ny][nx] == BLUE) {
            nd = (nd % 2) ? nd - 1: nd + 1;
            ny = r[id] + dy[nd], nx = c[id] + dx[nd];
            if (!InRange(ny, nx) || color[ny][nx] == BLUE) {
                ny = r[id], nx = c[id];
            }
            else if (color[ny][nx] == RED) {
                reverse = true;
            }
        }
        else if (color[ny][nx] == RED) {
            reverse = true;
        }

        vector<int> vec;
        while (1) {
            int y = r[id], x = c[id];
            int num = grid[y][x].back();
            grid[y][x].pop_back();
            vec.push_back(num);
            if (num == id) break;
        }
        Move(ny, nx, vec, reverse);
        //방향은 빼고 방향은 nd만 id만 
        dir[id] = nd;
        if (grid[ny][nx].size() >= 4) 
            return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> K;
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < N; j++)
            cin >> color[i][j];
    for (int id = 0; id < K; id++) {
        int y, x, d;
        cin >> y >> x >> d;
        r[id] = --y, c[id] = --x;
        grid[y][x].push_back(id);
        dir[id] = --d;
    }

    int turn = 1;
    while (turn < 1000) {
        if (Simulate()) {
            break;
        }
        turn++;
    }
    if (turn >= 1000) turn = -1;
    cout << turn;

    return 0;
}
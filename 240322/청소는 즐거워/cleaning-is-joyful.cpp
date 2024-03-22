#include <iostream>

#define MAX_N 499
#define DIR_NUM 4
using namespace std;

int N, ans = 0;
int grid[MAX_N][MAX_N];

int cy, cx;

int ratio[DIR_NUM][5][5] = {
    {
        {0,  0, 2, 0, 0},
        {0, 10, 7, 1, 0},
        {5,  0, 0, 0, 0},
        {0, 10, 7, 1, 0},
        {0,  0, 2, 0, 0},
    },
    {
        {0,  0, 0,  0, 0},
        {0,  1, 0,  1, 0},
        {2,  7, 0,  7, 2},
        {0, 10, 0, 10, 0},
        {0,  0, 5,  0, 0},
    },
    {
        {0, 0, 2,  0, 0},
        {0, 1, 7, 10, 0},
        {0, 0, 0,  0, 5},
        {0, 1, 7, 10, 0},
        {0, 0, 2,  0, 0},
    },
    {
        {0,  0, 5,  0, 0},
        {0, 10, 0, 10, 0},
        {2,  7, 0,  7, 2},
        {0,  1, 0,  1, 0},
        {0,  0, 0,  0, 0},
    }
};

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void AddUp(int y, int x, int dust) {
    if (Inrange(y, x)) grid[y][x] += dust;
    else ans += dust;
}

void Move(int dir) {
    int dy[DIR_NUM] = { 0,1,0,-1 };
    int dx[DIR_NUM] = { -1,0,1,0 };

    cy += dy[dir]; cx += dx[dir];

    int add_dust = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int dust = grid[cy][cx] * ratio[dir][i][j] / 100;
            //if (!dust) continue;
            AddUp(cy + i - 2, cx + j - 2, dust);
            add_dust += dust;
        }
    }
    //a%
    AddUp(cy + dy[dir], cx + dx[dir], grid[cy][cx] - add_dust);
}

bool End() {
    return !cy && !cx;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL);

    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    cy = cx = N / 2;
    int dir = 0, move_cnt = 1;

    while (!End()) {
        
        for (int i = 0; i < move_cnt; i++){
            Move(dir);
            if(End()) break;
        }
        
        dir = (dir + 1) % DIR_NUM;
        if (!(dir % 2)) move_cnt++;
    }
    cout << ans;
    return 0;
}
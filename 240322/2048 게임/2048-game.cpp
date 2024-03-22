#include <iostream>

#define MAX_N 20
#define MAX_LEVEL 5
using namespace std;

int N;
int grid[MAX_N][MAX_N];

int ans = 0;


void Down(int level, int curr_grid[MAX_N][MAX_N]);
void Left(int level, int curr_grid[MAX_N][MAX_N]);
void Right(int level, int curr_grid[MAX_N][MAX_N]);



void FindMaxNum(int(*grid)[MAX_N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ans = max(ans, grid[i][j]);
        }
    }
}

void Up(int level, int curr_grid[MAX_N][MAX_N]) {
    int next_grid[MAX_N][MAX_N] = { 0, };
    for (int j = 0; j < N; j++) {
        int num = 0;
        int idx = 0;
        for (int i = 0; i < N; i++) {
            if (!curr_grid[i][j]) continue;
            if (!num) { // 숫자 없음 갱신
                num = curr_grid[i][j];
            }
            else if (num == curr_grid[i][j]) {
                next_grid[idx++][j] = num * 2;
                num = 0;
            }
            else { //num과 숫자가 달라
                next_grid[idx++][j] = num;
                num = curr_grid[i][j];
            }
        }
        if (num) next_grid[idx][j] = num;
    }



    if (level == MAX_LEVEL) {
        FindMaxNum(next_grid);
        return;
    }

    Up(level + 1, next_grid);
    Down(level + 1, next_grid);
    Left(level + 1, next_grid);
    Right(level + 1, next_grid);

}
void Down(int level, int curr_grid[MAX_N][MAX_N]) {
    int next_grid[MAX_N][MAX_N] = { 0, };
    for (int j = 0; j < N; j++) {
        int num = 0;
        int idx = N - 1;
        for (int i = N - 1; i >= 0; i--) {
            if (!curr_grid[i][j]) continue;
            if (!num) { // 숫자 없음 갱신
                num = curr_grid[i][j];
            }
            else if (num == curr_grid[i][j]) {
                next_grid[idx--][j] = num * 2;
                num = 0;
            }
            else { //num과 숫자가 달라
                next_grid[idx--][j] = num;
                num = curr_grid[i][j];
            }
        }
        if (num) next_grid[idx--][j] = num;
    }
    if (level == MAX_LEVEL) {
        FindMaxNum(next_grid);
        return;
    }

    Up(level + 1, next_grid);
    Down(level + 1, next_grid);
    Left(level + 1, next_grid);
    Right(level + 1, next_grid);


}
void Left(int level, int curr_grid[MAX_N][MAX_N]) {
    int next_grid[MAX_N][MAX_N] = { 0, };
    for (int i = 0; i < N; i++) {
        int idx = 0;
        int num = 0;
        for (int j = 0; j < N; j++) {
            if (!curr_grid[i][j]) continue;
            if (!num) {
                num = curr_grid[i][j];
            }
            else if (num == curr_grid[i][j]) {
                next_grid[i][idx++] = num * 2;
                num = 0;
            }
            else { // 앞에 수와 다르다 
                next_grid[i][idx++] = num;
                num = curr_grid[i][j];
            }
        }
        if (num) next_grid[i][idx] = num;
    }
    if (level == MAX_LEVEL) {
        FindMaxNum(next_grid);
        return;
    }

    Up(level + 1, next_grid);
    Down(level + 1, next_grid);
    Left(level + 1, next_grid);
    Right(level + 1, next_grid);

}
void Right(int level, int curr_grid[MAX_N][MAX_N]) {
    int next_grid[MAX_N][MAX_N] = { 0, };
    for (int i = 0; i < N; i++) {
        int num = 0;
        int idx = N - 1;
        for (int j = N - 1; j >= 0; j--) {
            if (!curr_grid[i][j]) continue;
            if (!num) {
                num = curr_grid[i][j];
            }
            else if (num == curr_grid[i][j]) {
                next_grid[i][idx++] = num * 2;
                num = 0;
            }
            else {
                next_grid[i][idx++] = num;
                num = curr_grid[i][j];
            }
        }
        if (num) next_grid[i][idx] = num;

    }

    if (level == MAX_LEVEL) {
        FindMaxNum(next_grid);
        return;
    }

    Up(level + 1, next_grid);
    Down(level + 1, next_grid);
    Left(level + 1, next_grid);
    Right(level + 1, next_grid);
}


int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }

    Up(1, grid);
    Down(1, grid);
    Left(1, grid);
    Right(1, grid);

    cout << ans;

    return 0;
}
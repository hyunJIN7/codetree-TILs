#include <iostream>
#define MAX_N 100
using namespace std;

int N, L;
int grid[MAX_N][MAX_N];

bool InRange(int y) {
    return 0 <= y && y < N;
}

bool Row(int r) {
    bool check[MAX_N] = { 0, };
    for (int i = 0; i < N-1; i++) {
        //if (check[i]) continue;
        if (abs(grid[r][i] - grid[r][i + 1]) > 1) return false;
        if (grid[r][i] - grid[r][i + 1] == 1) {
            //i+1 ~ i+L 까지 경사로 설치
            int h = grid[r][i + 1];
            for (int j = i + 1; j <= i + L; j++) {
                if (!InRange(j) || check[j] || grid[r][j] != h) return false;
                check[j] = true;
            }
            i += L-1; //어차피 바로 1 더해져서 i+L부터 다시 시작
        }
        else if (grid[r][i] - grid[r][i + 1] == -1) { //왼쪽으로 경사 
            int h = grid[r][i];
            for (int j = i - L + 1; j <= i; j++) {
                if(!InRange(j) || check[j] || grid[r][j] != h) return false;
                check[j] = true;
            }
        
        }
    }
    return true;
}

bool Column(int c) {
    bool check[MAX_N] = { 0, };
    for (int i = 0; i < N - 1; i++) {
        //if (check[i]) continue;
        if (abs(grid[i][c] - grid[i+1][c]) > 1) return false;
        if (grid[i][c] - grid[i+1][c] == 1) {
            //i+1 ~ i+L 까지 경사로 설치
            int h = grid[i+1][c];
            for (int j = i + 1; j <= i + L; j++) {
                if (!InRange(j) || check[j] || grid[j][c] != h) return false;
                check[j] = true;
            }
            i += L - 1; //어차피 바로 1 더해져서 i+L부터 다시 시작
        }
        else if (grid[i][c] - grid[i+1][c] == -1) { //왼쪽으로 경사 
            int h = grid[i][c];
            for (int j = i - L + 1; j <= i; j++) {
                if (!InRange(j) || check[j] || grid[j][c] != h) return false;
                check[j] = true;
            }
        }
    }
    return true;
}

int GetPathCnt() {
    int ret = 0;
    for (int i = 0; i < N; i++) {
        ret += Row(i);
        ret += Column(i);
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> L;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> grid[i][j];

    cout << GetPathCnt();

    return 0;
}
#include <iostream>
#include <cstring>
#include <algorithm>

#define MAX_N 20
#define MAX_T 5
using namespace std;

int N;
int grid[MAX_N][MAX_N];
bool line[MAX_N][MAX_N];

int total = 0;

bool InRange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

// 상단 시작점에서 반시계 방향으로 
//int dy[4] = { 1,1,-1,-1 };
//int dx[4] = { -1,1,1,-1 };

bool PossibleToDraw(int y, int x, int h, int w) {
    //대각선 각도 1:1 비율로 
    return InRange(y + h, x - h) && InRange(y + h + w, x - h + w) && InRange(y + w, x + w);
}

void DrawLine(int y, int x, int h, int w) {
    memset(line, 0, sizeof(line));
    for (int i = 0; i <= h; i++) {
        line[y + i][x - i] = true;
        line[y + w + i][x + w - i] = true;
    }
    for (int i = 0; i <= w; i++) {
        line[y + i][x + i] = true;
        line[y + h + i][x - h + i] = true;
    }
}

int GetMin(int y, int x, int h, int w) {
    //왼쪽 오른쪽 꼭지점 좌표
    int yl = y + h, xl = x - h;
    int yr = y + w, xr = x + w;
    DrawLine(y, x, h, w);

    int popu[MAX_T] = { total,0,0,0,0 };

    //좌측 상단
    for (int i = 0; i < yl; i++)
        for (int j = 0; j <= x && !line[i][j]; j++)
            popu[1] += grid[i][j];
    popu[0] -= popu[1];

    //우측 상단
    for (int i = 0; i <= yr; i++)
        for (int j = N - 1; x + 1 <= j && !line[i][j]; j--)
            popu[2] += grid[i][j];
    popu[0] -= popu[2];

    // 좌측 하단 
    for (int i = N - 1; yl <= i; i--)
        for (int j = 0; j < xl + w && !line[i][j]; j++)
            popu[3] += grid[i][j];
    popu[0] -= popu[3];

    //우측 하단
    for (int i = N - 1; yr < i; i--)
        for (int j = N - 1; xl + w <= j && !line[i][j]; j--)
            popu[4] += grid[i][j];
    popu[0] -= popu[4];

    return *max_element(popu, popu+ MAX_T) - *min_element(popu,popu+ MAX_T);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
            total += grid[i][j];
        }

    int ans = 987654321;
    for (int i = N -2; i >= 0; i--) {
        for (int j = N-2; j >= 0; j--) {
            //좌상,우하 길이 h
            //좌하,우상 길이 w
            for (int h = 1; h < N; h++) {
                for (int w = 1; w < N; w++) {
                    if (PossibleToDraw(i, j, h, w)) {
                        ans = min(ans, GetMin(i, j, h, w));
                    }
                }
            }
        }
    }

    cout << ans;
    return 0;
}
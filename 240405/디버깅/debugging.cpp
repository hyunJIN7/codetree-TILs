#include <iostream>


#define MAX_H 30
#define MAX_N 10

using namespace std;

int N, H;
bool grid[MAX_H + 1][MAX_N + 1];

int ans = 10;

bool InRange(int x) {
    return 0 <= x && x < N;
}

bool Verify() {
    for (int j = 0; j < N; j++) {
        int id = j;
        for (int i = 0; i < H; i++) {
            if (grid[i][id]) id++;
            else if (id && grid[i][id - 1]) id--;
        }
        if (id != j) return false;
    }
    return true;
}


bool CanSelect(int y, int x) {
    if (grid[y][x]) return false;
    //양옆 비었다.
    if (InRange(x - 1) && grid[y][x - 1]) return false;
    if (InRange(x + 1) && grid[y][x + 1]) return false;
    return true;
}


void FindMin(int r, int c, int cnt) {
    if (cnt > 3 || cnt > ans) return;
    for (int i = r; i < H; i++) {
        int j = 0;
        if (i == r) j = c; //c 보낼때 시작지점부터 보내야해!!!!!!!!!!!
        for (; j < N; j++) {
            if (!CanSelect(i, j)) continue;
            grid[i][j] = true;
            if (Verify()) ans = min(ans, cnt);
            FindMin(i, j + 1,cnt+1);
            grid[i][j] = false;

        }
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int M;
    cin >> N >> M >> H;
    while (M--) {
        int a, b;
        cin >> a >> b;
        grid[a - 1][b - 1] = true;
    }
    //원본상태
    if(Verify()) ans = min(ans,0);
    FindMin(0,0,1);

    if (ans > 3) ans = -1;
    cout << ans;

    return 0;
}
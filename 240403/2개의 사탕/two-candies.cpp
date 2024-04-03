#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

#define MAX_N 10
using namespace std;
typedef pair<int, int> pii;

int N, M;
char grid[MAX_N][MAX_N];

pii red_pos, blue_pos;

queue<pair<pair<int, int>, pair<int, int> > > q;
bool visited[MAX_N + 1][MAX_N + 1][MAX_N + 1][MAX_N + 1];
int step[MAX_N + 1][MAX_N + 1][MAX_N + 1][MAX_N + 1];

const pair<int, int> OUT_OF_MAP = make_pair(MAX_N, MAX_N);

int ans = 987654321;

bool BlueExist() {
    return blue_pos != OUT_OF_MAP;
}

bool RedExist() {
    return red_pos != OUT_OF_MAP;
}

bool CanGo(int y, int x) {
    //벽이 아니고 사탕도 없어야해.
    //red/blue 사탕 따로 움직이니까 이 조건 넣어야함
    return grid[y][x] != '#' &&
        make_pair(y, x) != red_pos && make_pair(y, x) != blue_pos;
}

bool RedFirst(int dir) {
    int ry = red_pos.first, rx = red_pos.second;
    int by = blue_pos.first, bx = blue_pos.second;

    if (dir == 0) //위로 이동
        return rx == bx && ry < by;
    else if (dir == 1)//아래로 이동
        return (rx == bx && ry > by);
    else if (dir == 2)//좌측 이동
        return (ry == by && rx < bx);
    else // 우측 이동 
        return (ry == by && rx > bx);
}

pii GetDestination(pii pos, int dir) {
    int dy[4] = { -1, 1,  0, 0 };
    int dx[4] = { 0, 0, -1, 1 };
    int ny = pos.first + dy[dir], nx = pos.second + dx[dir];

    if (!CanGo(ny, nx)) return pos;
    if (grid[ny][nx] == 'O')
        return OUT_OF_MAP;

    //벽도 아니고 출구도 아니라 더 전진 가능
    return GetDestination({ ny,nx }, dir);
}

void Tilt(int dir) {
    if (RedFirst(dir)) {
        red_pos = GetDestination(red_pos, dir);
        blue_pos = GetDestination(blue_pos, dir);
    }
    else {
        blue_pos = GetDestination(blue_pos, dir);
        red_pos = GetDestination(red_pos, dir);
    }
}


void Push(pii red, pii blue,int new_step) {
    q.push(make_pair(red_pos, blue_pos));
    visited[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second] = true;
    step[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second] = new_step;
}

void FindMin() {
    while (!q.empty()) {
        red_pos = q.front().first;
        blue_pos = q.front().second;
        int curr_step = step[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second]; 
        q.pop();

        //if (cstep >= 10) continue;
        //이미 둘 중 하나라도 존재하지 않는다면
        if (!RedExist() || !BlueExist()) continue;

        for (int d = 0; d < 4; d++) {
            pii tmp_red = red_pos;
            pii tmp_blue = blue_pos;

            Tilt(d);

            //Tilt한다음 방문했나 검사해야해.
            if (!visited[red_pos.first][red_pos.second][blue_pos.first][blue_pos.second])
                Push(red_pos, blue_pos, curr_step + 1);
            red_pos = tmp_red;
            blue_pos = tmp_blue;
        }
    }
    //전체돌며 최소값 찾기 
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            //도착한 것들은 밖으로 내보냈으니 
            //red,blue 동시에 나간 경우 있더라도 여기서 걸러짐
            //red는 나간 상태에서 blue가 내부에 존재하는 경우만 찾으니
            if (visited[OUT_OF_MAP.first][OUT_OF_MAP.second][i][j])
                ans = min(ans,
                    step[OUT_OF_MAP.first][OUT_OF_MAP.second][i][j]);
}


int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0); 
    cin >> N >> M;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            cin >> grid[i][j];

            // 사탕의 경우 위치를 저장해두고, 맵에서는 지워줍니다.
            if (grid[i][j] == 'R') {
                grid[i][j] = '.';
                red_pos = make_pair(i, j);
            }
            if (grid[i][j] == 'B') {
                grid[i][j] = '.';
                blue_pos = make_pair(i, j);
            }
        }
    //for (int i = 0; i < N; i++) {
    //    for (int j = 0; j < M; j++) {
    //        char c;
    //        cin >> c;
    //        if (c == 'R') {
    //            red_pos = { i,j };
    //            c = '.';
    //        }
    //        else if (c == 'B') {
    //            blue_pos = { i,j };
    //            c = '.';
    //        }
    //        grid[i][j] = c;
    //    }
    //}
    Push(red_pos, blue_pos, 0);
    FindMin();

    if (ans > 10) ans = -1;
    cout << ans;

    return 0;
}
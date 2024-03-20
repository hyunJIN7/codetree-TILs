#include <iostream>
#include <vector>
#include <cstring>

#define DIR_NUM 4
#define MAX_N 20
using namespace std;

int N;
int grid[MAX_N][MAX_N] = { 0, };

vector<int> network[401];
int dy[] = { 0,0,1,-1 };
int dx[] = { 1,-1,0,0 };

bool Inrange(int y, int x) {
    return 0 <= y && y < N && 0 <= x && x < N;
}

void Place(int id) {
    int ty = N, tx = N, tempty = -1, tlike = -1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j]) continue;
            int empty = 0, like = 0;
            for (int d = 0; d < DIR_NUM; d++) {
                int ny = i + dy[d];
                int nx = j + dx[d];
                if (!Inrange(ny, nx)) continue;
                if (grid[ny][nx]) { //좋아하는건지 확인
                    for (int e : network[id]) {
                        if (grid[ny][nx] == e) like++;
                    }
                }
                else empty++;
            }
            //갱신 더 깔끔하게 하는 방법 뭐냐?!!?!?!?
            // 다른 경우 볼 필요 없는게 위에서 부터 시작해서
            if (tlike < like) {
                tlike = like;
                tempty = empty;
                ty = i, tx = j;
            }
            else if (tlike == like && tempty < empty) {
                tempty = empty;
                ty = i, tx = j;
            }
        
        }
    }

    grid[ty][tx] = id;
}

int getScore() {
    int ret = 0;
    int score[] = { 0,1,10,100,1000 };
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int like = 0;
            int id = grid[i][j];
            for (int d = 0; d < DIR_NUM; d++) {
                int ny = i + dy[d];
                int nx = j + dx[d];
                if (!Inrange(ny, nx)) continue;
                for (int e : network[id]) {
                    if (grid[ny][nx] == e) like++;
                }
            }
            ret += score[like];
        }
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N;
    for (int i = 0; i < N*N; i++) {
        int id;
        cin >> id;
        network[id].resize(4);
        cin >> network[id][0]
            >> network[id][1]
            >> network[id][2]
            >> network[id][3];
        Place(id);
    }
    cout << getScore();

    return 0;
}
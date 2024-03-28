#include <iostream>
#include <vector>
#include <cstring>

#define MAX_N 25
#define DIR_NUM 4
#define END make_pair(0,0)
#define EMPTY 0
using namespace std;

int N;

int grid[MAX_N][MAX_N];

int dy[DIR_NUM] = { 0,1,0,-1 }, dx[DIR_NUM] = { 1,0,-1,0 };

int score = 0;

vector<int> line, next_line;

void Attack() {
    int d, p;
    cin >> d >> p;
    int y = N / 2, x = N / 2;
    for (int i = 0; i < p; i++) {
        y += dy[d], x += dx[d];
        score += grid[y][x];
        grid[y][x] = EMPTY;
    }
}

void MakeLine() {
    //빈공간 제외하고 1차로 만들고
    //grid -> line
    int y = N / 2, x = N / 2;
    int cnt = 0 , dir = 2;
    while (1) {
        if (!(dir % 2)) cnt++;
        for (int i = 0; i < cnt; i++) {
            y += dy[dir], x += dx[dir];
            if (grid[y][x]) 
                line.push_back(grid[y][x]);
            if (make_pair(y, x) == END) 
                return;
        }
        dir = (dir + 3) % DIR_NUM;
    }
}


bool DeleteM() {
    bool ret = false;
    //line ->next 
    //4번 이상 나오는거 지우며 땡기고
    //line에서 next_line으로 줘 
    line.push_back(0);
    int cnt = 0, num = 0;
    for (int i = 0; i < line.size(); i++) {
        if (num != line[i]) {
            if (cnt >= 4) {
                ret = true;
                score += cnt * num;
            }
            else {
                while(cnt--)
                    next_line.push_back(num);
            }
            num = line[i];
            cnt = 1;
        }
        else cnt++;
    }
    return ret;
}
void MakeSocre() {
    //다시 line으로 주고 
    //점수도 획득하고 
    while (1) {
        if (!DeleteM()) break;
        //next_line에서 line으로 복사 
        line.clear();
        for (int e : next_line) {
            line.push_back(e);
        }
        next_line.clear();
    }

    //마지막에 나온거 개수,수 짝으로 다시 넣고 
//전체 범위 넘으면 거기서 끝에 
// 그냥 다 만들어라 
    line.push_back(0);
    next_line.clear();
    int num = 0, cnt = 0;
    for (int e : line) {
        if (num != e) {
            if (num) {
                next_line.push_back(cnt);
                next_line.push_back(num);
            }
            num = e;
            cnt = 1;
        }
        else cnt++;
    }
    line.clear();
    for (int e : next_line)
        line.push_back(e);
    next_line.clear();

}

void MakeMaze() {
    //line에 있는거 그냥 next_line에 있는 걸로 할까?..
    //아무튼 그걸 2차로 만들기 
    //y,x,가 00오면 끝내버려 
    int y = N / 2, x = N / 2;
    int cnt = 0, dir = 2;
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < line.size();) {
        if (dir == 0 || dir == 2) 
            cnt++;
        for (int j = 0; j < cnt; j++) {
            y += dy[dir], x += dx[dir];
            grid[y][x] = line[i++];
            if (make_pair(y, x) == END) return;
        }
        dir = (dir + 3) % DIR_NUM;
    }
}

void Simulate() {
    Attack();
    line.clear();
    next_line.clear();
    MakeLine();//1차로 바꾸기
    MakeSocre();
    MakeMaze();// 다시 그리디로 넣기 
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int M;
    cin >> N >> M;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> grid[i][j];
    while (M--) {
        Simulate();
    }

    cout << score;
    return 0;
}
#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>

#define MAX 10
#define DIR_NUM 4
#define EMPTY 0
#define WALL 9
#define EXIT -1
#define endl "\n"

using namespace std;

int N, M, K;//미로 크기,참가자 수, 게임 시간 

int maze[MAX + 1][MAX + 1]; // (1,1)에서 시작
int people[MAX + 1][MAX + 1]={0,};
vector<pair<int,int>> people_pos;

int total_distance = 0;
int exit_y,exit_x;

int sy,sx,sl ; // 가장 작은 사각형 왼쪽 상단 꼭지점,길이

bool InRange(int y, int x){
    return 0 < y && y <= N && 0 < x && x <= N;
}

int GetDistance(int y, int x){ // 출구와의 거리 
    return abs(exit_y - y) + abs(exit_x - x);
}

int CheckPeopleNum(){

    //인원수 체크?
    for(int i = 1; i <= N ; i++){
        for(int j = 1; j <= N; j++) {
            if( people[i][j]){

                for(int p = 0; p < people[i][j] ; p++){
                    people_pos.push_back({i,j});
                }
            }    
        }
    }
    return (int)people_pos.size();
}

void Out(){
    cout<< "^^^^^^ "<< K <<" ^^^^^ \n";
    cout<<"### 사람   \n";
    for(int i = 1; i <= N ; i++){
        for(int j = 1; j <= N; j++) {
            cout<< people[i][j] <<" ";

        }
        cout<<endl;
    } 

    cout<<"### 미로   \n";
    for(int i = 1; i <= N ; i++){
        for(int j = 1; j <= N; j++) {
            cout<< maze[i][j] <<" ";

        }
        cout<<endl;
    } 

}
void Rotate(){ //좌측 상단 
    //people과 maze 돌리기 
    int y1 = sy, x1 = sx, x2 = sx + sl -1;
    int new_maze[MAX + 1][MAX + 1]={0,};
    int new_people[MAX + 1][MAX + 1]={0,};

    // cout<<"돌릴 범위 \n";
    // cout<< y1<<" ," <<x1<<" : "<< y1+sl-1<<" , "<<x2<<endl;
    // maze는 감소도 해야해!!!!!!!
    for(int i =  0; i < sl ; i++){
        for(int j = 0; j < sl ; j++){
            // (y1 + i , x1 + j) -> (y1 + j , x2 - i)
            new_maze[y1 + j][x2 - i] = maze[y1 + i][x1 + j];
            new_people[y1 + j][x2 - i] = people[y1 + i][x1 + j];
        }
    } 
    for(int i =  0; i < sl ; i++){
        for(int j = 0; j < sl ; j++){
            if(new_maze[y1 + i][x1 + j] > 0) // 벽 감소
                new_maze[y1 + i][x1 + j] -= 1;
            else if (new_maze[y1 + i][x1 + j] == -1) // 새로운 출구 위치 
                exit_y = y1 +i, exit_x = x1 + j;
            maze[y1 + i][x1 + j] = new_maze[y1 + i][x1 + j];
            people[y1 + i][x1 + j] = new_people[y1 + i][x1 + j];
        }
    }
    // cout<< "새출구 : "<< exit_y <<" "<<exit_x<<endl;

}

void FindMinSquare(){
    //가장 작은 사각형 찾기

    // 각 길이 별로 좌측 상단에서부터 사각형 만들어보기 
    //사람이랑 출구 있으면 그 걸로 rotate
    for(int l = 2 ; l <= N; l++){ // 한변길이 
        for(int i = 1; i <= N - l + 1 ; i++){
            for(int j = 1; j <= N - l + 1; j++) {
                // (i,j) ~ (i+l-1, j + l -1)

                //현재 사각형 범위 안에  출구 없음
                if(exit_y < i || i + l - 1 < exit_y || exit_x < j || j + l- 1 < exit_x ) 
                    continue;
                
                for(int p = 0 ; p < people_pos.size(); p++){
                    int py = people_pos[p].first, px =people_pos[p].second;
                    if( i <= py && py <= i + l -1 && j <= px && px <= j + l -1){
                        sy = i; sx = j; sl = l;
                        // cout<< "사각형 : " <<sy<<" "<< sx<<" "<<sl<<endl;
                        return;
                    }

                }
            }
        }
    }

}

void Move(){
    int new_people[MAX + 1][MAX + 1] = {0,}; // 초기화 되겠지?
    int dy[DIR_NUM] = {1, -1, 0, 0};
    int dx[DIR_NUM] = {0, 0, 1, -1};

    for(int i = 1; i <= N ; i++){
        for(int j = 1; j <= N; j++) {
            // 빈칸에 있는 사람일 경우 , 츌구인 경우는 어쩌냐? 뭐 이거 체크하기 
            if(!people[i][j] || maze[i][j] > 0) continue;
            
            int dis = GetDistance(i,j), y = i, x = j; // 기존 위치
            bool change = false;

            //남, 북, 동, 서 순으로 방문 
            for(int d = 0 ; d < DIR_NUM; d++){
                int ny = i + dy[d] , nx = j + dx[d];
   
                if(!InRange(ny,nx) || maze[ny][nx] > 0 ) 
                    continue; 

                int ndis = GetDistance(ny, nx);
                if(ndis < dis){ // 기존 위치보다 가까우면 
                    dis = ndis;
                    y = ny;
                    x = nx;
                    //여기를 실행한다는건 이동한다는 이야기니까
                    change = true;
                    
                }
            }

            //한칸에 여러명 있을 수 있으니 잘해라 
            new_people[y][x] += people[i][j];
            if(change) total_distance +=  people[i][j];
            // 여기 확인!!!!!!
            
        }
    }

    for(int i = 1; i <= N ; i++){
        for(int j = 1; j <= N; j++) {
            people[i][j] = new_people[i][j];
        }
    }
    people[exit_y][exit_x] = 0; // 나간 인원은 빼주기     

}

void Input(){
    cin >> N >> M >> K;
    for(int i = 1; i <= N ; i++)
        for(int j = 1; j <= N; j++) 
            cin >> maze[i][j];

    for(int i = 0; i < M; i++){
        int y, x;
        cin>> y >> x;
        people[y][x]++; 
    }
    cin >> exit_y >> exit_x;
    maze[exit_y][exit_x] = EXIT;
}

void Init(){
    memset(maze,0,sizeof(maze));
    memset(people,0,sizeof(people));
}

void Solve(){
    Init();
    Input();

    // K = 1;
    // Out();

    while(K--){
        //움직여
        Move();
      
        //인원 체크 , 사람 없으면 끝 
        people_pos.clear();
        if(!CheckPeopleNum()) break;

        // Out();
        //출구 회전 : 출구 사각형, 사각형 회전 
        FindMinSquare();
        Rotate();
        // cout<<"@@  돌려 \n";
        // Out();

    }
    cout<<total_distance<<endl<<exit_y<<" "<<exit_x;
    // cout<<sy<<" "<<sx<<" "<<sl;

}

int main() {
    ios::sync_with_stdio(false); cin.tie(NULL);
    Solve();
    return 0;
}
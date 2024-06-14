#include<iostream>
#include <queue>
#include <vector>
#include <tuple>
#include <cstring>

using namespace std;
constexpr int N_l = 5;
constexpr int N_s = 3;

queue<int> wall;

class Board {
private:
	bool InRange(int y, int x) {
		return 0 <= y && y < N_l && 0 <= x && x < N_l;
	}
public:
	int grid[N_l][N_l];
	
	Board() {
		memset(grid, 0, sizeof(grid));
	}

	Board* Rotate(const int y, const  int x, const int cnt) {
		Board* rot = new Board();
		for (int i = 0; i < N_l; i++) {
			for (int j = 0; j < N_l; j++) {
				rot->grid[i][j] = grid[i][j];
			}
		}
		
		//int& a = rot->grid; 이거 어떻게 참조하지 
		for (int r = 0; r < cnt; r++) {
			int tmp = rot->grid[y][x + 2];
			rot->grid[y][x + 2] = rot->grid[y][x];
			rot->grid[y][x] = rot->grid[y + 2][x];
			rot->grid[y + 2][x] = rot->grid[y + 2][x + 2];
			rot->grid[y + 2][x + 2] = tmp;

			tmp = rot->grid[y + 1][x + 2];
			rot->grid[y + 1][x + 2] = rot->grid[y][x + 1];
			rot->grid[y][x + 1] = rot->grid[y + 1][x];
			rot->grid[y + 1][x] = rot->grid[y + 2][x + 1];
			rot->grid[y + 2][x + 1] = tmp;
		}
		return rot;
	}

	int FindScore() {
		int score = 0;
		bool vis[N_l][N_l] = { 0, };
		int dy[4] = { 0,0,-1,1 }, dx[4] = { 1,-1,0,0 };
		
		for (int i = 0; i < N_l; i++) {
			for (int j = 0; j < N_l; j++) {
				if (vis[i][j]) continue;

				vector<pair<int, int>> vec;
				vec.push_back({ i,j });
				int size = 1, id = 0;
				vis[i][j] = true;
				int num = grid[i][j];

				while (id < size) {
					int y, x;
					tie(y, x) = vec[id];
					id++;
					for (int d = 0; d < 4; d++) {
						int ny = y + dy[d];
						int nx = x + dx[d];
						if (!InRange(ny, nx) // 범위 밖
							|| grid[ny][nx] != num // 다른 수
							|| vis[ny][nx] ) continue; // 이미 방문
						size++;
						vec.push_back({ ny,nx });
						vis[ny][nx] = true;
					}
				}

				if (size >= 3) {
					score += size;
					for (pair<int, int> p : vec) {
						grid[p.first][p.second] = 0;
					}

				}
				vec.clear();
			}
		}
		return score;
	}

	void Fill() {
		for (int j = 0; j < N_l; j++) {
			for (int i = N_l - 1; i >= 0; i--) {
				if (!grid[i][j]) {
					grid[i][j] = wall.front();
					wall.pop();
				}
			}
		}
	}

};

int main(int argc, char** argv)
{
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	int K, M;
	Board* board = new Board();

	cin >> K >> M;
	for (int i = 0; i < N_l; i++) {
		for (int j = 0; j < N_l; j++) {
			cin >> board->grid[i][j];
		}
	}
	while (M--) {
		int num; cin >> num;
		wall.push(num);
	}

	while (K--) {
		Board* max_board = NULL;
		int max_score = 0;

		for (int cnt = 1; cnt <= 3; cnt++) {
			for (int j = 0; j <= N_l - N_s; j++) {
				for (int i = 0; i <= N_l - N_s; i++) {
					Board* rot = board->Rotate(i, j, cnt);
					int score = rot->FindScore();
					if (max_score < score) {
						max_score = score;
						max_board = rot;
					}
				}
			}
		}
		if (max_board == NULL) break;

		// 가장 점수 큰 곳 선정되었다면
		while (true) {
			max_board->Fill();
			int score = max_board->FindScore();
			if (!score) break;
			max_score += score;
		}
		board = max_board;
		cout << max_score << " ";
	}
	return 0;
}
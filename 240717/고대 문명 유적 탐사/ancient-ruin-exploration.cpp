#include <iostream>
#include <queue>
#include <tuple>
#include <vector>
#include <cstring>
#define MAX_N 5
using namespace std;

queue<int> que;
int dy[] = { 0,0,1,-1 }, dx[] = { 1,-1,0,0 };

bool OutRange(int y, int x) {
	return y < 0 || 5 <= y || x < 0 || 5 <= x;
}

class Board {
public:	
	int grid[MAX_N][MAX_N];
	Board() {

	}

	Board* Rotate(int y, int x, int cnt, Board* root) {
		Board* ret = new Board;

		// copy
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				ret->grid[i][j] = root->grid[i][j];
			}
		}

		//회전
		for (int r = 0; r < cnt; r++) {
			int tmp = ret->grid[y - 1][x - 1];
			ret->grid[y - 1][x - 1] = ret->grid[y + 1][x - 1];
			ret->grid[y + 1][x - 1] = ret->grid[y + 1][x + 1];
			ret->grid[y + 1][x + 1] = ret->grid[y - 1][x + 1];
			ret->grid[y - 1][x + 1] = tmp;

			tmp = ret->grid[y-1][x];
			ret->grid[y - 1][x] = ret->grid[y][x - 1];
			ret->grid[y][x - 1] = ret->grid[y + 1][x];
			ret->grid[y + 1][x] = ret->grid[y][x + 1];
			ret->grid[y][x + 1] = tmp;
		}
		return ret;
	}

	int BFS(int y, int x) {
		vector<pair<int, int>> vec;
		int num = grid[y][x];
		bool vis[5][5] = { 0, };

		vis[y][x] = true;
		vec.push_back({ y,x });


		for (int i = 0; i < vec.size();i++) {
			auto [y, x] = vec[i];
			for (int d = 0; d < 4; d++) {
				int ny = y + dy[d], nx = x + dx[d];
				if (OutRange(ny, nx) || vis[ny][nx] || grid[ny][nx] ^ num) continue;
				vec.push_back({ ny,nx });
				vis[ny][nx] = true;
			}
		}

		if (vec.size() < 3) return 0;
		for (auto [y, x] : vec) {
			grid[y][x] = 0;
		}

		return (int)vec.size();

	}

	int GetScore() {
		int ret = 0;

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if(grid[i][j])
					ret += BFS(i, j);
			}
		}

		return ret;
	}

	void Fill() {
		for (int j = 0; j < 5; j++) {
			for (int i = 4; i >= 0; i--) {
				if (!grid[i][j]) {
					grid[i][j] = que.front();
					que.pop();
				}
			}
		}
	}


};


Board* root;


int Simulate() {
	int ret = 0; //max_score
	Board* maxRot = NULL;

	for (int r = 1; r <= 3; r++) {
		for (int j = 1; j <= 3; j++) {
			for (int i = 1; i <= 3; i++) {
				Board* rot = root->Rotate(i, j, r, root);

				int score = rot->GetScore();
				if (ret < score) {
					ret = score;
					maxRot = rot;
				}
			}
		}
	}
	if (maxRot == NULL) return 0;

	//max인거 찾았으면
	while (1) {
		maxRot->Fill();
		int score = maxRot->GetScore();
		if (!score) break;
		ret += score;
	}
	root = maxRot;

	return ret;
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	int K, M;
	cin >> K >> M;
	root = new Board();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> root->grid[i][j];
		}
	}
	for (int num; M--;) {
		cin >> num;
		que.push(num);
	}

	vector<int> ans;
	for (int i = 0,tmp; i < K; i++) {
		if (!(tmp = Simulate())) 
			break;
		ans.push_back(tmp);
	}
	for (int e : ans) cout << e << " ";

	return 0;
}
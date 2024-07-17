#include <iostream>
#include <queue>
#include <tuple>
#define MAX_N 200
using namespace std;

int N, M;
int grid[MAX_N][MAX_N];

int dy[] = { 0,0,1,-1 }, dx[] = { 1,-1,0,0 };

bool OutRange(int y, int x) {
	return y < 0 || N <= y || x < 0 || M <= x;
}

int Simulate(int y, int x) {
	int ret = 0;
	priority_queue<tuple<int, int, int> > pq;
	bool vis[MAX_N][MAX_N] = { 0, };

	pq.push({ grid[y][x],y,x });
	vis[y][x] = true;

	for (int cnt = 4; cnt; cnt--) {
		auto [num, cy, cx] = pq.top(); pq.pop();

		ret += num;

		for (int d = 0; d < 4; d++) {
			int ny = cy + dy[d], nx = cx + dx[d];
			if (OutRange(ny, nx) || vis[ny][nx]) continue;
			vis[ny][nx] = true;
			pq.push({ grid[ny][nx],ny,nx });
		}
	}
	return ret;
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> N >> M;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> grid[i][j];
		}
	}

	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			ans = max(Simulate(i, j), ans);
		}
	}

	cout << ans;
	return 0;
}
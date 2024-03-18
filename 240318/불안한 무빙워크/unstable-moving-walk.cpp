#include <iostream>
#include <bitset>

#define MAX_N 100
using namespace std;

int N, K;
int safety[MAX_N * 2] = { 0, };
bitset<MAX_N * 2> on;
int start, finish;

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> K;
    for (int i = 0; i < 2 * N; i++) cin >> safety[i];
    on.reset();
    start = 0, finish = N - 1;
   
    int test = 1;
    for ( ; ; test++) {
        if (on.test(finish)) {
            on.set(finish, 0);
        }

        // 시작점 이동
        start = (start + 2 * N - 1) % (2 * N);
        finish = (finish + 2 * N - 1) % (2 * N);
        if (on.test(finish)) {
            on.set(finish, 0); 
        }

        //앞칸으로 전진 
        int i = (finish + 2 * N - 1) % (2 * N);
        for (int cnt = 0; cnt < N-1; cnt++) {
            if (on.test(i) == 0) continue;
            
            int j = (i + 1) % (2 * N);
            if (safety[j] && on.test(j) == 0) {
                on.set(j, 1);
                safety[j]--;
                if (!safety[j]) K--;
            }
            i = (i + 2 * N - 1) % (2 * N);
        }
        //finish에 사람 있으면 빼줘.
        if (on.test(finish)) {
            on.set(finish, 0); 
        }

        //start에 사람 올리기
        if (safety[start]) {
            on.set(start, 1);
            safety[start]--;
            if (!safety[start]) K--;
        }
        if (K <= 0) break;

    }
    cout << test;

    return 0;
}
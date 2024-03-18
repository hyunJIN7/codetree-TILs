#include <iostream>
//#include <bitset>

#define MAX_N 100
using namespace std;

int N, K;
int safety[MAX_N * 2] = { 0, };
//bitset<MAX_N * 2> on;
bool on[MAX_N * 2] = { 0, };
int start, finish;

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    cin >> N >> K;
    for (int i = 0; i < 2 * N; i++) cin >> safety[i];
    //on.reset();
    start = 0, finish = N - 1;

    int test = 0;
    while (K > 0) {

        // 시작점 이동
        start = (start + 2 * N - 1) % (2 * N);
        finish = (finish + 2 * N - 1) % (2 * N);
        on[finish] = 0;


        //앞칸으로 전진 
        int i = (finish + 2 * N - 1) % (2 * N);
        for (int cnt = 0; cnt < N - 1; cnt++, i = (i + 2 * N - 1) % (2 * N)) {
            if (!on[i]) continue;

            // 다음 칸
            int j = (i + 1) % (2 * N);
            if (safety[j] > 0 && !on[j]) {
                on[i] = 0;
                on[j] = 1;
                safety[j]--;
                if (!safety[j]) K--;
            }
            
        }
        on[finish] = 0;

        //start에 사람 올리기
        if (!on[start] && safety[start] > 0) {
            on[start] = 1;
            safety[start]--;
            if (!safety[start]) K--;
        }
        //finish에 사람 있으면 빼줘.
        on[finish] = 0;
        test++;
    }
    cout << test;

    return 0;
}
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int L, N, Q;
int Map[41][41];
int Pos[41][41];
pair<int, int> Order[101];

struct Data {
	int x;
	int y;
	int h;
	int w;
	int k;
};
Data Knight[31];
vector<pair<int, int>> Position[31];
int Dead[31];
int Damage[31];

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };
set<int> Temp;
//상|우|하|좌

void Input() {
	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> Map[i][j];

		}
	}
	for (int i = 1; i <= N; i++) {
		cin >> Knight[i].x >> Knight[i].y >> Knight[i].h >> Knight[i].w >> Knight[i].k;
		//기사 정보 입력
	}
	for (int i = 1; i <= N; i++) {
		//각 기사의 영역 칠하기 & 영역의 좌표 저장하기
		for (int x = 0; x < Knight[i].h; x++) {
			for (int y = 0; y < Knight[i].w; y++) {
				//1번 기사의 영역 = 1
				//2번 기사의 영역 = 2
				Position[i].push_back({ Knight[i].x + x , Knight[i].y + y });
				Pos[Knight[i].x + x][Knight[i].y + y] = i;
			}
		}
	}
	for (int i = 1; i <= Q; i++) {
		//명령어 저장
		cin >> Order[i].first >> Order[i].second;
	}
}


bool Move_Knight(int num, int D) {

	queue<pair<int, int>> Q;
	Temp.insert(num);
	for (int i = 0; i < Position[num].size(); i++) {
		Q.push(Position[num][i]);
	}
	//명령 대상 기사의 좌표를 모두 Q에 삽입한다.

	while (!Q.empty()) {
		int px = Q.front().first;
		int py = Q.front().second;
		Q.pop();

		int nx = px + dx[D];
		int ny = py + dy[D];
		//다음 좌표

		if (nx < 1 || ny < 1 || nx > L || ny > L) {
			return false;							//범위 밖을 벗어나는 경우, 전체 이동이 취소된다.
		}
		if (Map[nx][ny] == 2) {
			return false;							//벽을 하나라도 만나는 경우, 전체 이동이 취소된다.
		}

		int prev_size = Temp.size();
		if (Pos[nx][ny] != 0) {
			Temp.insert(Pos[nx][ny]);
		}
		int next_size = Temp.size();
		//새로운 기사의 영역이 추가되었나 확인한다.

		if (prev_size < next_size) {
			//새롭게 추가된 기사의 영역을 모두 추가한다.
			for (int i = 0; i < Position[Pos[nx][ny]].size(); i++) {
				Q.push(Position[Pos[nx][ny]][i]);
			}
		}
	}

	return true;
}

int main() {
	Input();
	for (int t = 1; t <= Q; t++) {

		int num = Order[t].first;
		int D = Order[t].second;
		if (Dead[num] == 1) continue;

		if (Move_Knight(num, D) == true) {
			//움직일 수 있는 기사의 좌표 모두 수정
			for (int n : Temp) {

				int Kill = 0;
				for (int i = 0; i < Position[n].size(); i++) {
					Position[n][i].first += dx[D];
					Position[n][i].second += dy[D];

					if (Map[Position[n][i].first][Position[n][i].second] == 1) {
						Kill++;
					}
				}


				//자기 자신 제외하고 함정 피해를 입는다.
				if (n != num) {
					Damage[n] += Kill;
					Knight[n].k -= Kill;
				}

				//나이트가 죽은 경우
				if (Knight[n].k <= 0) {
					Dead[n] = 1;
				}
			}


			memset(Pos, 0, sizeof(Pos));
			for (int i = 1; i <= N; i++) {
				if (Dead[i] == true) continue;
				for (int j = 0; j < Position[i].size(); j++) {
					Pos[Position[i][j].first][Position[i][j].second] = i;
					//죽은 나이트 제외하고 Pos 상에 표기한다.
				}
			}

		}

		Temp.clear();
	}


	int Ans = 0;
	for (int i = 1; i <= N; i++) {
		if (Dead[i] == true) continue;
		Ans += Damage[i];
	}
	cout << Ans;

}
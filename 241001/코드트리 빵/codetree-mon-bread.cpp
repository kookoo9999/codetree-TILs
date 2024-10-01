#include <iostream>
#include <vector>
#include <queue>
#include <memory.h>

using namespace std;

int dx[] = { -1,0,0,1 };
int dy[] = { 0,-1,1,0 };
int N, M;
int map[16][16];
int dist[16][16];
int distx[16][16];
int disty[16][16];
int canMove[16][16] = { 0, };
bool IsGoal[30] = { false, };
bool IsBaseSelected[30] = { false, };

struct Pos
{
	int x, y;
};
struct Camp
{
	int x, y;
	bool choose = false;
};


vector<Pos> Player;
vector<Pos> Store;
vector<Camp> Base;
vector<Pos> Dummy;
vector<int> DummyIdx;

void Input()
{
	cin >> N >> M;
	Store.resize(M + 1);
	Player.resize(M + 1);
	
	int cnt = 1;
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{			
			
			cin >> map[i][j];
			if (map[i][j] == 1)
			{
				Pos a;
				a.x = i; a.y = j;
				Base.push_back({ a.x,a.y }); cnt++;
				map[i][j] = -1;
			}
		}
	}

	for (int i = 1; i <= M; i++)
	{
		Pos a;
		cin >> a.x >> a.y;
		Store[i]=a;
		a.x = -1; a.y = -1;
		Player[i]=a;
	}	
}

void bfs(int x, int y , int tx,int ty , int nowidx)
{	
	bool visited[16][16] = { false, };
	visited[x][y] = true;
	dist[x][y] = 1;
	queue<pair<pair<int, int>,int>> q;
	q.push({ { x,y },1 });
	
	while(!q.empty())
	{
		int cx = q.front().first.first;
		int cy = q.front().first.second;
		int cnt = q.front().second;
		q.pop();

		if (cx == tx && cy == ty)
		{
			dist[cx][cy] = cnt+1;
			
			return;
		}

		for (int i = 0; i <4; i++)
		{
			int nx = cx + dx[i];
			int ny = cy + dy[i];

			if (nx<1 || ny<1 || nx>N || ny>N) continue;
			if (canMove[nx][ny] == 1)continue;
			if (!visited[nx][ny])
			{				
				
				visited[nx][ny] = true;
				q.push({ {nx,ny},cnt + 1 });
				dist[nx][ny] = cnt + 1;
				distx[nx][ny] = cx;
				disty[nx][ny] = cy;
				
				
			}
		}
	}
	
	return;
}

void InitDist()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			dist[i][j] = 9999;
		}
	}
}

bool CheckAll()
{
	for (int i = 1; i <= M; i++)
	{
		if (!IsGoal[i]) return false;
	}

	return true;
}

void PlayerMove(int time)
{
	for (int i = 1; i <= M; i++)
	{
		if (time < i) return;
		if (IsGoal[i]) continue;

		int cx = Player[i].x;
		int cy = Player[i].y;

		if (cx == -1 && cy == -1) continue;

		memset(dist, 0, sizeof(dist));
		memset(distx, 0, sizeof(distx));
		memset(disty, 0, sizeof(disty));

		bfs(Store[i].x, Store[i].y,cx,cy,i);
		
		int nx = distx[cx][cy];
		int ny = disty[cx][cy];		

		// 도착했으면
		if (nx == Store[i].x && ny == Store[i].y)
		{
			IsGoal[i] = true;
			
			Dummy.push_back({ nx,ny });
			DummyIdx.push_back(i);
		}
		
		Player[i].x = nx;
		Player[i].y = ny;
		
	}
}

int SearchBase(int x,int y , int nowidx)
{
	int ret=0;
	bool visited[16][16] = { false, };	
	queue<pair<pair<int,int>,int>> q;	
	visited[x][y] = true;
	q.push({ {x,y,},0 });

	while (!q.empty())
	{
		int cx = q.front().first.first;
		int cy = q.front().first.second;
		int cnt = q.front().second;
		q.pop();

		for(int i=0; i<4; i++)
		{
			int nx = cx + dx[i];
			int ny = cy + dy[i];
			
			if (nx<1 || ny<1 || nx>N || ny>N) continue;
			if (canMove[nx][ny] == 1) continue;

			if (!visited[nx][ny])
			{
				visited[nx][ny] = true;
				q.push({ {nx,ny},cnt + 1 });
				dist[nx][ny] = cnt + 1;
			}
		}
	}

	return ret;
}

void SelectBase(int time)
{
	for (int i = 1; i <= M; i++)
	{
		if (time < i) return;
		if (IsBaseSelected[i]) continue;
		InitDist();
		SearchBase(Store[i].x, Store[i].y,i);
		int nowdist = 9999;
		int select = 0;
		int x=N, y=N;
		for (int j = N; j >= 1; j--)
		{
			for (int k = N; k >= 1; k--)
			{
				if (map[j][k] != -1) continue;
				if (dist[j][k] < nowdist)
				{
					nowdist = dist[j][k];
					x = j;
					y = k;
				}
				else if (dist[j][k] == nowdist)
				{
					if (j < x)
					{
						x = j;
						y = k;
					}
					else if (j == x)
					{
						if (k < y)
						{
							x = j;
							y = k;
						}
					}
				}
			}
		}		

		Player[i].x = x;
		Player[i].y = y;
		IsBaseSelected[i] = true;
		Base[select].choose = true;
		Dummy.push_back({ Player[i].x,Player[i].y });
		DummyIdx.push_back(i);
		
	}
}



void Simul()
{
	int ans = 0, time = 0;

	while (1)
	{		
		time += 1;

		//플레이어 이동
		PlayerMove(time);		

		// 도착했는 위치 이제 못가게 처리
		for (int i = 0; i < Dummy.size(); i++)
		{
			canMove[Dummy[i].x][Dummy[i].y] = 1;
		}
			Dummy.clear();
			DummyIdx.clear();
		

		
		if (CheckAll())
		{
			cout << time;
			break;
		}

		SelectBase(time);
	}
	
}


int main()
{
	Input();
	Simul();
	return 0;
}
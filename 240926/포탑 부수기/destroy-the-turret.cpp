#include <iostream>
#include <memory.h>
#include <queue>

using namespace std;

int dx[] = {0,1,0,-1,1,1,-1,-1};
int dy[] = {1,0,-1,0,1,-1,-1,1};
bool act[11][11] = { false, };
int timemap[11][11] = { 0, };
int map[11][11] = { 0, };
int dist[11][11] = { 0, };
int distx[11][11] = { 0, };
int disty[11][11] = { 0, };

int N, M, K; // n=row , m=col
int sx = 0, sy = 0, tx = 0, ty = 0, nowTime = 1;

void Input()
{
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			cin >> map[i][j];
		}
	}
}

void FindAttacker()
{
	int tempx, tempy, tempMax = 9999, tempTime = 0;
	for (int i = N; i >= 1; i--)
	{
		for (int j = M; j >= 1; j--)
		{
			if (map[i][j] < 1) continue;
			// 공격력 가장 약한놈
			if (map[i][j] < tempMax)
			{
				tempMax = map[i][j];
				tempx = i; tempy = j;
			}
			// 같으면
			else if(map[i][j]==tempMax)
			{		
				// 최근에 공격한놈
				if (timemap[i][j] > tempTime)
				{
					tempTime = timemap[i][j];
					tempMax = map[i][j];
					tempx = i; tempy = j;
				}
				// 둘다 최근이면
				if (timemap[i][j] == tempTime)
				{
					// 행열 합 큰놈
					if (i + j > tempx + tempy)
					{
						tempMax = map[i][j];
						tempx = i; tempy = j;
					}
					// 행열 같으면
					else if (i + j == tempx + tempy)
					{
						// 열 큰놈
						if (j > tempy)
						{
							tempMax = map[i][j];
							tempx = i; tempy = j;
						}
					}
				}
			}
			
		}
	}
	sx = tempx; sy = tempy;
	map[sx][sy] += N + M;
}

void FindTarget()
{
	int tempx, tempy, tempMax = -1, tempTime = 9999;
	for (int i = N; i >= 1; i--)
	{
		for (int j = M; j >= 1; j--)
		{
			if (map[i][j] < 1) continue;
			if (i == sx && j == sy) continue;

			// 공격력 가장쎈놈
			if (map[i][j] > tempMax)
			{
				tempTime = timemap[i][j];
				tempMax = map[i][j];
				tempx = i; tempy = j;
			}		
			// 같으면
			else if(map[i][j]==tempMax)
			{
				// 시간이 가장 적은놈
				if (timemap[i][j] < tempTime)
				{
					tempMax = map[i][j];
					tempx = i; tempy = j;
					tempTime = timemap[i][j];
				}
				else if(timemap[i][j]==tempTime)
				{					
					// 행열 작은놈
					if (i + j < tempx + tempy)
					{
						tempMax = map[i][j];
						tempx = i; tempy = j;
					}
					// 행열 같으면
					else if (i + j == tempx + tempy)
					{
						// 열이 작은놈
						if (j < tempy)
						{
							tempMax = map[i][j];
							tempx = i; tempy = j;
						}
					}
				}
			}
			
		}
	}
	tx = tempx; ty = tempy;
}

bool CanLaser()
{
	bool visited[11][11] = { false, };
	
	queue<pair<pair<int, int>, int>> q;
	q.push({ { sx,sy },0 });
	visited[sx][sy] = true;

	while (!q.empty())
	{
		int cx = q.front().first.first;
		int cy = q.front().first.second;
		int cnt = q.front().second;
		q.pop();
		
		if (cx == tx && cy == ty)
		{
			return true;
		}

		for (int i = 0; i < 4; i++)
		{
			int nx = cx + dx[i];
			int ny = cy + dy[i];

			if (nx < 1) nx = N;
			if (nx > N) nx = 1;
			if (ny < 1) ny = M;
			if (ny > M) ny = 1;
			if (map[nx][ny] <1) continue;						

			if (!visited[nx][ny])
			{
				visited[nx][ny] = true;
				dist[cx][cy] = cnt+1;
				distx[nx][ny] = cx;
				disty[nx][ny] = cy;
				q.push({ {nx,ny },cnt + 1 });
			}
		}
	}
	return false;
}
bool dvisited[11][11] = { false, };
bool bcanLaser = false;
void dfs(int x, int y, int  cnt)
{
	if (bcanLaser) return;
	dvisited[x][y] = true;
	dist[x][y] = cnt;
	if (x == tx && y == ty ) 
	{
		bcanLaser = true;
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx < 1) nx = N;
		if (nx > N) nx = 1;
		if (ny < 1) ny = M;
		if (ny > M) ny = 1;
		if (map[nx][ny]<1) continue;
		

		if (!dvisited[nx][ny])
		{			
			
			dfs(nx, ny, cnt + 1);
		}
	}
	return;
}

void LaserAttack(int now)
{	
	int damage = map[sx][sy];
	if (map[tx][ty] < damage) map[tx][ty] = 0;
	else
	{
		map[tx][ty] -= damage;
	}
	bool flag = false;
	int cx = tx; int cy = ty;
	
	queue<pair<int, int>> q;
	q.push({ tx,ty });
	while (!q.empty())
	{
		cx = q.front().first;
		cy = q.front().second;
		q.pop();

		if (cx == sx && cy == sy) break;
		if (flag)
		{
			map[cx][cy] -= (damage / 2);
			act[cx][cy] = true;
		}

		q.push({ distx[cx][cy],disty[cx][cy] });		
		flag = true;
	}
	
	// 최단경로 중간의 포탑들 데미지 입음
	/*for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			if (i == tx && j == ty) continue;
			if (dist[i][j] > 0)
			{
				map[i][j] -= (damage / 2);
				
				act[i][j] = true;
			}
		}
	}*/
}

void Potan(int now)
{
	int damage = map[sx][sy];
	if (map[tx][ty] < damage) map[tx][ty] = 0;
	else
	{
		map[tx][ty] -= damage;
	}
	
	int cx = tx; int cy = ty;

	for (int i = 0; i < 8; i++)
	{
		int nx = cx + dx[i];
		int ny = cy + dy[i];

		if (nx < 1) nx = N;
		if (nx > N) nx = 1;
		if (ny < 1) ny = M;
		if (ny > M) ny = 1;
		if (map[nx][ny] < 1) continue;
		if (nx == sx && ny == sy) continue;
		map[nx][ny] -= (damage / 2);		
		act[nx][ny] = true;
	}
}

void Attack(int now)
{	
	memset(dist, 0, sizeof(dist));
	memset(dvisited, 0, sizeof(dvisited));
	
	act[sx][sy] = true;
	act[tx][ty] = true;
	
	bcanLaser = false;
	//dfs(sx, sy, 0);
	
	if (CanLaser())
	{
		LaserAttack(now);
	}
	
	else
	{
		Potan(now);
	}

	timemap[sx][sy] = now;	
}

void Repair()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			if (act[i][j]) continue;
			if (map[i][j]>0)
			{
				map[i][j] += 1;
			}
		}
	}
}

void Simul()
{
	for (int time = 1; time <= K; time++)
	{
		memset(act, false, sizeof(act));
		FindAttacker();
		FindTarget();
		Attack(time);
		Repair();
		int cnt = 0;
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= M; j++)
			{
				if (map[i][j] > 0) cnt += 1;
			}
		}
		if (cnt == 1) break;
	}
	int ans = -1;
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			if (map[i][j] > ans) ans = map[i][j];
		}
	}
	cout << ans << endl;
	
}

int main()
{
	Input();
	Simul();
	return 0;
}
#include <iostream>
#include <vector>
#include <memory.h>
#include <math.h>
#include <queue>
#include <tuple>

using namespace std;

int map[11][11];
int playermap[11][11];
vector<vector<int>> pmap;
vector<pair<int, int>> player;
int distmap[11][11];
int moved[11];
bool alive[11];
int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };
int n, m, k,ans;
int tx, ty;
int nAlive = 0;

void Input()
{
	memset(map, -1, sizeof(map));
	memset(playermap, -1, sizeof(map));
	memset(distmap, 0, sizeof(distmap));
	memset(alive, true, sizeof(alive));
	memset(moved, 0, sizeof(moved));
	cin >> n >> m >> k;
	pmap.resize(n+1, vector<int>(n+1));
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cin >> map[i][j];
			playermap[i][j] = 0;
			pmap[i][j] = 0;
		}
	}
	player.resize(m + 1);
	
	for (int i = 1; i <= m; i ++ )
	{
		int a, b;
		cin >> a >> b;
		player[i].first = a;
		player[i].second = b;
		playermap[a][b] = i;
		pmap[a][b] = i;
	}
	cin >> tx >> ty;
	map[tx][ty] = -9;	
	
}



int getDistance(int x1,int y1,int x2,int y2)
{
	int ret = 0;
	ret = abs(x1 - x2) + abs(y1 - y2);
	return ret;
}

bool isIn(int x, int y)
{
	if (x < 1 || y < 1 || x > n || y > n) return false;
	return true;
}


pair<int,int> move_player(int x,int y)
{
	int nowdist = getDistance(x, y, tx, ty);
	pair<int, int> ret;
	ret.first = -1;
	ret.second = -1;
	for (int i = 0; i < 4; i++)
	{
		int nx = x + dx[i];
		int ny = y + dy[i];
		
		if (!isIn(nx,ny)) continue;
		if (map[nx][ny] > 0) continue;
		int temp = getDistance(nx, ny, tx, ty);
		if (temp < nowdist)
		{
			nowdist = temp;
			ret.first = nx;
			ret.second = ny;
		}
	}
	return ret;
}

void RotateArr(int x, int y, int size)
{
	int temp_map[11][11];
	int temp_player[11][11];
	vector<vector<int>> temp_pmap(n + 1, vector<int>(n + 1));
	memcpy(temp_map, map, sizeof(map));
	memcpy(temp_player, playermap, sizeof(playermap));
	copy(pmap.begin(), pmap.end(), temp_pmap.begin());
	
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			temp_map[x+j][y+size-i-1] = map[x+i][y+j];
			temp_player[x + j][y + size - i - 1] = playermap[x + i][y + j];
			temp_pmap[x + j][y + size - i - 1] = pmap[x + i][y + j];
		}
	}
	
	memcpy(map, temp_map, sizeof(temp_map));
	memcpy(playermap, temp_player, sizeof(temp_player));
	copy(pmap.begin(), pmap.end(), temp_pmap.begin());
}

tuple<int,int,int> FindSquare()
{
	int x = 1, y = 1, len = 1;
	bool temp_pcheck[11][11] = { false, };
	for (int s=1; s<=m; s++)
	{
		if (alive[s])
		{
			temp_pcheck[player[s].first][player[s].second] = true;
		}
		
	}
	for (int size = 2; size <= n; size++)
	{
		for (int i = 1; i <= n ; i++)
		{
			for (int j = 1; j <= n ; j++)
			{
				bool pfound = false;
				bool efound = false;
				for (int r = i; r < i + size; r++)
				{
					for (int c = j; c < j + size; c++)
					{						
						if (i + size -1 > n || j + size -1 > n) continue;
						if (temp_pcheck[r][c])
						{
							pfound = true;
						}
						if (map[r][c] == -9)
						{
							efound = true;
						}
						if (pfound && efound)
						{							
							return { i,j,size };
						}
					}
				}

			}
		}
	}
	return { 0,0,0 };
}

void RotateMap()
{			
	auto [x,y,len] = FindSquare();
	
	RotateArr(x, y, len);

	for (int i = x; i < x+len; i++)
	{
		for (int j = y; j < y+len; j++)
		{
			if (map[i][j] > 0) map[i][j] -= 1;
			if (map[i][j] == -9)
			{
				tx = i; ty = j;
			}
			/*if (playermap[i][j] > 0 )
			{				
				if (alive[playermap[i][j]])
				{
					player[playermap[i][j]].first = i;
					player[playermap[i][j]].second = j;
				}
				
			}*/
		}
	}		

	/*for (int i = 1; i <= m; i++) {
		int cx = player[i].first;
		int cy = player[i].fir;
		if (cy >= y && cy < y + len && cx >= x && cx < x + len) {
			int ox = cx - x, oy = cy - y;
			int ny = ox, nx = len - oy - 1;
			player[i].first = ny + y, player[i].second = nx + x;
		}
	}*/

	for (int i = 1; i <= m; i++)
	{
		if (!alive[i]) continue;
		int cx = player[i].first;
		int cy = player[i].second;

		if (cx >= x && cx < x + len && cy >= y && cy < y + len)
		{
			int ox = cx - x;
			int oy = cy - y;
						
			player[i].first = x + oy;
			player[i].second = y + len - ox -1;
		}
	}
	
}

void Simul()
{
	for (int time = 0; time < k; time++)
	{
		// 플레이어 이동
		for (int i = 1; i <= m; i++)
		{
			if (!alive[i]) continue;
			pair<int, int> t = move_player(player[i].first, player[i].second);
			if (t.first == -1 && t.second == -1) continue;
			else
			{
				if (t.first == tx && t.second == ty)
				{
					alive[i] = false;
					nAlive += 1;
					moved[i] += 1;					
					playermap[player[i].first][player[i].second] = 0;					
					
					if (nAlive == m)
					{						
						for (int i = 1; i <= m; i++)
						{
							ans += moved[i];
						}
						cout << ans << endl << tx << " " << ty;
						return;
					}
				}
				else
				{
					moved[i] += 1;					
					playermap[player[i].first][player[i].second] = 0;
					playermap[t.first][t.second] = i;
					player[i].first = t.first;
					player[i].second = t.second;
				}
			}

		}

		// 미로회전
		RotateMap();
	}

	for (int i = 1; i <= m; i++)
	{
		ans += moved[i];
	}
	cout << ans << endl << tx << " " << ty;
	
}

int main()
{
	Input();
	Simul();
	return 0;
}
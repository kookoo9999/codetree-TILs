#include <iostream>
#include <vector>
#include <memory.h>
#include <math.h>
using namespace std;

int n, m, k;
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };
struct Player
{
	int x, y, dir, num, stat, score = 0, gun=0;
};
vector<Player> p;


vector<vector<vector<int>>> map(30 + 1, vector<vector<int>>(30 + 1));
int player_map[31][31];
void Input()
{
	cin >> n >> m >> k;
	
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			int temp;
			cin >> temp;
			map[i][j].push_back(temp);
		}
	}
	for (int i = 0; i < m; i++)
	{
		Player t;
		cin >> t.x >> t.y >> t.dir >> t.stat;
		t.num = i + 1;
		p.push_back(t);
		player_map[t.x][t.y] = i + 1;
		
	}
}

int Switch_dir(int &idx)
{
	int ret = 0;
	switch (idx)
	{
	case 0:
		ret = 2;
		break;
	case 1:
		ret = 3;
		break;
	case 2:
		ret = 0;
		break;
	case 3:
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}

int Rotate_dir(int &idx)
{
	int ret = 0;

	switch (idx)
	{
	case 0:
		ret = 1;
		break;
	case 1:
		ret = 2;
		break;
	case 2:
		ret = 3;
		break;
	case 3:
		ret = 0;
		break;
	default:
		break;
	}
	return ret;
}

void MovePlayer(int &idx)
{
	int cx = p[idx].x;
	int cy = p[idx].y;

	int nx = cx + dx[p[idx].dir];
	int ny = cy + dy[p[idx].dir];

	if (nx < 1 || ny < 1 || nx > n || ny > n)
	{
		p[idx].dir = Switch_dir(p[idx].dir);
		nx = cx + dx[p[idx].dir];
		ny = cy + dy[p[idx].dir];
	}

	player_map[cx][cy] = 0;
	
	p[idx].x = nx;
	p[idx].y = ny;
	
}

// return <winner,loser>
pair<int,int> Fight(Player& p1, Player& p2)
{
	
	//pair<Player&, Player&> ret = {p1,p2};
	pair<int, int> ret;
	if (p1.gun + p1.stat > p2.gun + p2.stat)
	{
		ret = { p1.num-1,p2.num-1 };
	}
	else if (p1.gun + p1.stat == p2.gun + p2.stat)
	{
		if (p1.stat > p2.stat)
		{
			ret = { p1.num-1,p2.num-1 };
		}
		else
		{
			ret = { p2.num-1,p1.num-1 };
		}
	}
	else
	{
		ret = { p2.num-1,p1.num-1 };
	}
	return ret;
}

void Change_Gun(Player& p1)
{
	int max = 0, select = -1;
	if (p1.gun > 0)
	{
		map[p1.x][p1.y].push_back(p1.gun);
		p1.gun = 0;
	}

	for (int j = 0; j < map[p1.x][p1.y].size(); j++)
	{
		int temp = map[p1.x][p1.y][j];
		if (temp > max)
		{
			max = temp;
			select = j;
		}
	}
	if (select > -1)
	{
		p1.gun = map[p1.x][p1.y][select];
		map[p1.x][p1.y].erase(map[p1.x][p1.y].begin() + select);
	}
	
	
}


void Winner_Move(Player& p1)
{
	Change_Gun(p1);
}

void Loser_Move(Player& p1)
{
	int cx = p1.x;
	int cy = p1.y;
	int dir = p1.dir;

	//총을 내려놓고
	if (p1.gun > 0)
	{
		map[cx][cy].push_back(p1.gun);
		p1.gun = 0;
	}
	
	// 이동
	int nx, ny;
	for (int i = 0; i < 4; i++)
	{
		nx = cx + dx[dir];
		ny = cy + dy[dir];

		if (nx<1 || ny<1 || nx>n || ny>n || player_map[nx][ny] > 0)
		{
			dir = Rotate_dir(dir);
		}
		else
		{
			break;
		}
	}
	
	p1.x = nx;
	p1.y = ny;
	p1.dir = dir;

	Change_Gun(p1);	
}



void Simul()
{	
	for (int i = 0; i < k; i++)
	{
		// 플레이어 이동
		for (int idx = 0; idx < m; idx++)
		{
			MovePlayer(idx);

			// 해당칸에 플레이어 있으면
			bool noperson = true;
			int person = -1;
			for (int j = 0; j < p.size(); j++)
			{
				if (idx == j) continue;
				if (p[idx].x == p[j].x && p[idx].y == p[j].y)
				{
					noperson = false;
					person = j;
				}
			}

			if (!noperson)
			{
				// 싸움
				auto [winner, loser] = Fight(p[idx], p[person]);

				// 점수 처리
				p[winner].score += abs((p[winner].stat + p[winner].gun) - (p[loser].stat + p[loser].gun));

				// 진놈
				Loser_Move(p[loser]);

				// 이긴놈
				Winner_Move(p[winner]);

				// 맵갱신
				player_map[p[loser].x][p[loser].y] = p[loser].num;
				player_map[p[winner].x][p[winner].y] = p[winner].num;
			}
			
			// 없으면
			else
			{
				Change_Gun(p[idx]);
				player_map[p[idx].x][p[idx].y] = p[idx].num;
			}
		}
	}
	for (int i = 0; i < p.size(); i++)
	{
		cout << p[i].score << " ";
	}
}

int main()
{
	memset(player_map, 0, sizeof(player_map));
	Input();
	Simul();
	return 0;
}
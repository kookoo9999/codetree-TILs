#include <iostream>
#include <vector>
#include <queue>
#include <memory.h>
using namespace std;

int map[70][70]; // r,c < 70
int temp_map[70][70];
int r, c, k, ans = 0; // row, col, 골렘 수
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };

struct Golem // dir = 0 1 2 3 ,북 동 남 서
{
	int x, y, dir,num;
};

vector<Golem> golems;

void Init_map()
{
	for (int i = -2; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			map[i][j] = 0;
			temp_map[i][j] = 0;
		}
	}
}

void Input()
{
	cin >> r >> c >> k;
	for (int i = 0; i < k; i++)
	{
		int cols, dirs;
		cin >> cols >> dirs;

		Golem g;
		
		g.dir = dirs;
		g.y = cols-1;
		g.x = -2;
		g.num = i + 1;
		golems.push_back(g);
	}
	Init_map();
}



bool Check_below(Golem &now_g)
{
	int x1, x2, x3, y1, y2, y3;
	y1 = now_g.y - 1; x1 = now_g.x + 1; // 왼쪽
	y2 = now_g.y;	  x2 = now_g.x + 2; // 가운데
	y3 = now_g.y + 1; x3 = now_g.x + 1; // 오른쪽

	if (x2 >= r) return false;
	if (map[x1][y1] > 0 && map[x1][y1] <= k) return false;
	if (map[x2][y2] > 0 && map[x2][y2] <= k) return false;
	if (map[x3][y3] > 0 && map[x3][y3] <= k) return false;
	
	//now_g.y += 1;
	return true;
}

bool Check_right(Golem& now_g)
{
	int x1, x2, x3, x4, x5, y1, y2, y3, y4, y5;
	y1 = now_g.y + 1; x1 = now_g.x - 1; // 위
	y2 = now_g.y + 2; x2 = now_g.x;		// 가운데
	y3 = now_g.y + 1; x3 = now_g.x + 1; // 아래

	if (y2 >= c) return false;	
	//if (map[x1][y1] != 0 || map[x2][y2] != 0 || map[x3][y3] != 0) return false;

	if (map[x1][y1] > 0 && map[x1][y1] <= k) return false;
	if (map[x2][y2] > 0 && map[x2][y2] <= k) return false;
	if (map[x3][y3] > 0 && map[x3][y3] <= k) return false;

	y4 = now_g.y + 2; x4 = now_g.x + 1;
	y5 = now_g.y + 1; x5 = now_g.x + 2;

	if (x5 >= r) return false;
	//if (map[x4][y4] != 0 || map[x5][y5] != 0) return false;

	if (map[x4][y4] > 0 && map[x4][y4] <= k) return false;
	if (map[x5][y5] > 0 && map[x5][y5] <= k) return false;
	

	

	return true;
}

bool Check_left(Golem& now_g)
{
	int x1, x2, x3, x4, x5, y1, y2, y3, y4, y5;
	y1 = now_g.y - 1; x1 = now_g.x - 1; // 위
	y2 = now_g.y - 2; x2 = now_g.x ;	// 가운데
	y3 = now_g.y - 1; x3 = now_g.x + 1; // 아래

	if (y2 < 0) return false;
	//if (map[x1][y1] != 0 || map[x2][y2] != 0 || map[x3][y3] != 0) return false;

	if (map[x1][y1] > 0 && map[x1][y1] <= k) return false;
	if (map[x2][y2] > 0 && map[x2][y2] <= k) return false;
	if (map[x3][y3] > 0 && map[x3][y3] <= k) return false;

	y4 = now_g.y - 2; x4 = now_g.x + 1;
	y5 = now_g.y - 1; x5 = now_g.x + 2;
	
	if (x5 >= r) return false;
	if (map[x4][y4] > 0 && map[x4][y4] <= k) return false;
	if (map[x5][y5] > 0 && map[x5][y5] <= k) return false;

	return true;
}

int Change_dir(bool left,int dir) // left : true , right : false
{
	// 0  1  2  3 
	// 북 동 남 서
	int ret = -1;
	// 반시계방향
	if (left)
	{
		switch (dir)
		{
		case 0:
			ret = 3;
			break;
		case 1:
			ret = 0;
			break;
		case 2:
			ret = 1;
			break;
		case 3:
			ret = 2;
			break;
		default:
			break;
		}
	}
	// 시계방향
	else
	{
		switch (dir)
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

	}
	return ret;
}

void Now_pos_delete(Golem& now_g)
{
	map[now_g.x][now_g.y] = 0; //센터
	map[now_g.x][now_g.y-1] = 0; // 상
	map[now_g.x][now_g.y+1] = 0; // 하
	map[now_g.x-1][now_g.y] = 0; // 좌
	map[now_g.x+1][now_g.y] = 0; // 우
}
void Update_Map(Golem& now_g)
{
	map[now_g.x][now_g.y] = now_g.num; //센터
	map[now_g.x][now_g.y - 1] = now_g.num; // 상
	map[now_g.x][now_g.y + 1] = now_g.num; // 하
	map[now_g.x - 1][now_g.y] = now_g.num; // 좌
	map[now_g.x + 1][now_g.y] = now_g.num; // 우
}

void Go_down(Golem& now_g)
{
	now_g.x += 1;

	//map[now_g.x][now_g.y] = 1;
}

void Turn_Left(Golem& now_g)
{
	now_g.x += 1;
	now_g.y -= 1;

	now_g.dir = Change_dir(true, now_g.dir);
	//map[now_g.x][now_g.y] = 1;
}

void Turn_Right(Golem& now_g)
{
	now_g.x += 1;
	now_g.y += 1;

	now_g.dir = Change_dir(false, now_g.dir);
	//map[now_g.x][now_g.y] = 1;
}

void Move(Golem& input_g)
{
	Golem now_g = input_g;
	while (1)
	{	
		// 아래로
		if (Check_below(now_g))
		{
			Now_pos_delete(now_g);
			Go_down(now_g);
			Update_Map(now_g);
		}
		// 왼쪽회전			
		else if (Check_left(now_g))
		{
			Now_pos_delete(now_g);
			Turn_Left(now_g);
			Update_Map(now_g);
		}
		// 오른쪽 회전
		else if (Check_right(now_g))
		{
			Now_pos_delete(now_g);
			Turn_Right(now_g);
			Update_Map(now_g);
		}
		else 
			break;
	}	
	input_g = now_g;
}

bool Check_exit_connect(Golem& now_g)
{
	int cx = now_g.x + dx[now_g.dir];
	int cy = now_g.y + dy[now_g.dir];

	for (int i = 0; i < 4; i++)
	{
		int nx = cx + dx[i];
		int ny = cy + dy[i];
		if (ny < 0 || ny>=c) continue;
		if (map[nx][ny] != 0 )
		{
			if(map[nx][ny] != now_g.num)  return true;
		}
	}

	return false;
}

int bfs(Golem& now_g)
{
	int x = now_g.x;
	int y = now_g.y;
	int ret = -1;
	queue<pair<int, int>> q;

	bool visited[70][70] = { false, };
	visited[x][y] = true;
	q.push({ x,y });

	while (!q.empty())
	{
		int cx = q.front().first;
		int cy = q.front().second;
		if (cx > ret) ret = cx;
		q.pop();

		for (int i = 0; i < 4; i++)
		{
			int nx = cx + dx[i];
			int ny = cy + dy[i];
			if (nx < 0 || ny < 0 || nx >= r || ny >= c) continue;

			// 다른골렘 연결 확인			
			if (map[nx][ny] > 0 && map[nx][ny] <= k && !visited[nx][ny])
			{
				if (map[cx][cy] == map[nx][ny])
				{
					visited[nx][ny] = true;
					q.push({ nx,ny });
				}
				else 
				{
					if (map[nx][ny] > 0)
					{
						int now = map[cx][cy] - 1;
						int exit_x = golems[now].x + dx[golems[now].dir];
						int exit_y = golems[now].y + dy[golems[now].dir];
						if (cx == exit_x && cy == exit_y)
						{
							visited[nx][ny] = true;
							q.push({ nx,ny });
						}
					}
					
				}				
			}			
		}
	}
	return ret+1;
}

bool Check_range(Golem& now_g)
{
	if (now_g.x <= 0) return false;
	return true;
}

void Simul()
{
	bool breset = true;
	for (int i = 0; i < k; i++)
	{			
		// 골렘 이동
		Move(golems[i]);

		// 범위 벗어났는지 체크
		if (!Check_range(golems[i]))
		{
			Init_map();
			breset = true;
		}
		else
		{
			// 최대 행 찾기
			if (breset || Check_exit_connect(golems[i]))
			{
				ans += bfs(golems[i]);
				breset = false;
			}
			else
			{
				ans += golems[i].x + 2;
			}
		}
		
	}
	printf("%d\n", ans);
}

int main()
{
	memset(map, -1, sizeof(map));
	Input();
	Simul();

	return 0;
}
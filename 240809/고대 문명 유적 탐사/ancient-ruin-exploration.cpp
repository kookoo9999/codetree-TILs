#include <iostream>
#include <vector>
#include <memory>
#include <queue>

using namespace std;

int k, m, ans = 0 , mode = 0; 
bool visited[5][5];
int map[5][5];
int temp_map[5][5];
int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };
queue<int> nums;
vector<pair<int, int>> delPoints;;

void Rotate(int x1,int y1 , int cnt)
{
	int size = 3;
	int x = x1 - 1;
	int y = y1 - 1;
	int temp[5][5];	
	memcpy(temp, map, sizeof(map));
	for (int it = 0; it < cnt; it++)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				temp[x + j][y + size -i - 1] = map[x + i][y + j];
			}
		}
	}
	
	memcpy(temp_map, temp, sizeof(temp));
}

void Input()
{
	cin >> k >> m;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			cin >> map[i][j];

	for (int i = 0; i < m; i++)
	{
		int num;
		cin >> num;
		nums.push(num);
	}
}

int bfs(int x, int y)
{	
	queue<pair<int,int>> q;
	q.push({ x, y });
	visited[x][y] = true;
	vector<pair<int, int>> v;
	v.push_back({ x,y });
	int cnt = 0;

	while (!q.empty())
	{
		int cx = q.front().first;
		int cy = q.front().second;
		q.pop();
		cnt++;
		for (int i = 0; i < 4; i++)
		{
			int nx = cx + dx[i];
			int ny = cy + dy[i];
			if (nx < 0 || ny < 0 || nx>4 || ny>4) continue;
			if (temp_map[x][y] == temp_map[nx][ny] && !visited[nx][ny])
			{				
				visited[nx][ny] = true;
				q.push({ nx,ny });
				v.push_back({ nx,ny });
			}
		}
	}
	if (cnt < 3)
	{
		cnt = 0;		
		return cnt;
	}
	if (mode == 1)
	{
		for(auto& s :v)	delPoints.push_back(s);
	}
	
	return cnt;
}

int FindMaxVal()
{
	int ret = 0;
	memset(visited, false, sizeof(visited));
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (!visited[i][j])
			{
				ret += bfs(i, j);
			}
		}
	}
	return ret;
}

void Simul()
{
	// 1 연결된 유물수가 많은 것 찾기
	// 1-2 여러가지 일 경우 회전중심의 col값이 작은걸로
	int val = 0;
	int temp_ans[5][5];
	for (int i = 1; i < 4; i++)
	{
		memcpy(temp_map, map, sizeof(map));
		// 1-3 col이 같다면 row가 작은걸로
		for (int j = 1; j < 4; j++)
		{			
			// 1-1 여러가지 일 경우 회전한 각도가 작은걸로
			for (int k = 1; k < 4; k++)
			{				
				Rotate(k, j, i);
				
				// 유물 최대값 찾기
				int res = FindMaxVal();

				if (res > val)
				{
					val = res;
					// 찾은 최대의 값이 있는 조건 맵을 원본 맵에 적용
					memcpy(temp_ans, temp_map, sizeof(temp_map));
				}
			}			
		}
	}
	if(val>0)	memcpy(map, temp_ans, sizeof(map));
}

void RemoveItem()
{
	

	for (auto& s : delPoints)
	{
		map[s.first][s.second] = -9999;
	}
	delPoints.clear();
}

void RefillItem()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 4; j >= 0 ; j--)
		{
			if (map[j][i] == -9999)
			{
				map[j][i] = nums.front();
				nums.pop();
			}
		}
	}
}

void Solution()
{
	for (int i = 0; i < k; i++)
	{
		Simul();
		while (1)
		{
			memcpy(temp_map, map, sizeof(map));
			mode = 1;
			int res = FindMaxVal();

			if (res < 3) break;

			ans += res;

			RemoveItem();

			RefillItem();
		}
		if (ans > 0)
		{
			printf("%d ", ans);
		}		
		ans = 0;
		mode = 0;
	}
}

int main()
{
	Input();	
	Solution();
	
	return 0;
}
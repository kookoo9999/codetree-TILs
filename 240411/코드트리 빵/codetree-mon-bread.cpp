#include <iostream>
#include <queue>
#include <vector>

#define MAX 16

using namespace std;

struct Player
{
    int cx=-1, cy=-1, tx=0, ty=0 , dir=-1;
    bool arrive = false;
};

struct Person
{
    int x, y;
};

struct Board
{
    int x, y, cnt;
};

struct Mart
{
    int x, y;
    bool arrive = false;
};

int map[MAX][MAX] = { 0, };
bool visited[MAX][MAX] = { false, };
vector<Player> p;
vector<Person> Persons;
vector<pair<int, int>> base;
vector<Mart> Marts;
int n, m;

int dist[MAX][MAX] = { 9999, };
int dx[] = { -1,0,0,1 };
int dy[] = { 0,-1,1,0 };

void Input()
{
    cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> map[i][j];
            if (map[i][j] == 1)
            {
                base.push_back({i,j});
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        x--, y--;
        Marts.push_back({ x,y,false });
    }
}

int BFS(int x,int y , int tx, int ty)
{
    bool visited[MAX][MAX] = { false, };
    
    queue<Board> q;
    visited[x][y] = true;

    q.push({ x,y ,0});

    while (!q.empty())
    {   
        int cx = q.front().x, cy = q.front().y, cnt = q.front().cnt;

        if (cx == tx && cy == ty) return cnt;

        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;

            if (!visited[nx][ny])
            {
                visited[nx][ny] = true;
                q.push({ nx,ny, cnt+1 });
            }   
        }
    }
}

int CalDistance(int x1, int y1, int x2, int y2)
{
    int ret = abs(x2 - x1) + abs(y2 - y1);
    return ret;
}

pair<int,int> SelectBase(int idx)
{
    int cmp = 0 , min = 9876543;
    pair<int, int> ret;

    vector<pair<int, int>> temp;

    for (int i = 0; i < base.size(); i++)
    {
        temp.push_back(base[i]);
    }

    for (auto& s : base)
    {
        int target_x = p[idx].tx , target_y = p[idx].ty;
        //cmp = CalDistance(s.first, s.second, target_x, target_y);
        auto cmp = BFS(s.first, s.second, target_x, target_y);

        // 같을 경우
        if (cmp == min)
        {
            // 행이 적고, 열이 적은 값으로 설정
            if (ret.first > s.first)
            {
                ret.first = s.first;
                ret.second = s.second;
            }
            else if (ret.first == s.first)
            {
                if (ret.second > s.second)
                {
                    ret.first = s.first;
                    ret.second = s.second;
                }
            }
        }

        if (cmp < min)
        {
            min = cmp;
            ret.first = s.first;
            ret.second = s.second;
        }        
    }

    base.clear();
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i].first == ret.first && temp[i].second == ret.second) continue;
        base.push_back(temp[i]);
    }

    return ret;
}

void SetMoveDir(int idx)
{
    int x = p[idx].cx;
    int y = p[idx].cy;
    int tx = p[idx].tx;
    int ty = p[idx].ty;
    int dir = -1;

    if (x < tx) dir = 0;
    if (x == tx)
    {
        (y < ty) ? dir = 2 : dir = 1;
    }
    if (x > tx) dir = 3;

    p[idx].dir = dir;
}

void Move(int idx)
{
    SetMoveDir(idx);
    int nx = p[idx].cx + dx[p[idx].dir];
    int ny = p[idx].cy + dy[p[idx].dir];

    if (map[nx][ny] == -1) SetMoveDir(idx);

    p[idx].cx += dx[p[idx].dir];
    p[idx].cy += dy[p[idx].dir];
    
}



bool IsInMap(int idx)
{
    if (!(p[idx].cx >= 0 && p[idx].cy >= 0 && p[idx].cx < n && p[idx].cy < n)) return false;    
    return true;
}

void MoveAll()
{
    for (int i=0; i<m; i++)
    {
        // 플레이어마다 무브
        if (IsInMap(i))
        {
            if (!p[i].arrive)
            {
                Move(i);
            }            
        }        
    }
}

bool CheckArrive(int idx)
{
    int x = p[idx].cx;
    int y = p[idx].cy;
    int x2 = p[idx].tx;
    int y2 = p[idx].ty;

    if (x == x2 && y == y2) return true;
    return false;
}

int UpdateMap()
{
    int arriveCnt = 0;
    for (int i = 0; i < m; i++)
    {
        if (CheckArrive(i))
        {
            int x = p[i].cx;
            int y = p[i].cy;
            map[x][y] = -1;
            p[i].arrive = true;
            arriveCnt++;
        }
    }
    return arriveCnt;
}

void Init_Dist()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            dist[i][j] = 9999;
        }
    }
}

void Bfs(int x, int y)
{

    queue<pair<pair<int, int>, int>> Q;
    Q.push({ {x, y}, 0 });
    dist[x][y] = 0;

    while (!Q.empty())
    {
        int cx = Q.front().first.first;
        int cy = Q.front().first.second;
        int time = Q.front().second;
        Q.pop();

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            if (visited[nx][ny] == 1) continue;
            if (dist[nx][ny] != 9999) continue;

            dist[nx][ny] = time + 1;
            Q.push({ {nx, ny}, time + 1 });
        }

    }

}

void Move_bfs(int idx)
{
    int tx = Marts[idx].x;
    int ty = Marts[idx].y;

    Init_Dist();
    Bfs(tx, ty);

    int x = Persons[idx].x;
    int y = Persons[idx].y;
    int min = 9999;

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
        if (visited[nx][ny]) continue;

        if (dist[nx][ny] < min) 
        {
            min = dist[nx][ny];
            Persons[idx].x = nx;
            Persons[idx].y = ny;
        }
    }
}



void InsertPlayer(int time)
{
    // 베이스 선택
    auto t = SelectBase(time);

    // 해당 플레이어 좌표 갱신
    p[time].cx = t.first;
    p[time].cy = t.second;

    // 해당 베이스로 이동 막음
    map[t.first][t.second] = -1;
}



void Push_Player(int time)
{
    int tx = Marts[time].x;
    int ty = Marts[time].y;
    int min = 9999, x, y;
    Init_Dist();
    Bfs(tx, ty);
    for (int i = n-1; i >= 0; i--)
    {
        for (int j = n-1; j >= 0; j--)
        {
            if (map[i][j] == 1 && dist[i][j] <= min)
            {
                min = dist[i][j];
                x = i;
                y = j;
            }
        }
    }

    Persons.push_back({ x,y });
    visited[x][y] = true;

}

bool CheckAllArrive()
{
    for (auto s : Marts)
    {
        if (!s.arrive) return false;
    }
    return true;
}

int Solution()
{
    bool stopflag = false;
    int min = 0;    
    
    while (1)
    {
        min++;

        // 1
        for (int i = 0; i < Persons.size(); i++)
        {
            if (Marts[i].arrive) continue;
            Move_bfs(i);
        }

        // 2
        for (int i = 0; i < Persons.size(); i++)
        {
            if (Marts[i].arrive) continue;

            if (Persons[i].x == Marts[i].x && Persons[i].y == Marts[i].y)
            {
                Marts[i].arrive = true;
                visited[Marts[i].x][Marts[i].y] = 1;
            }
        }

        if (CheckAllArrive()) break;;
        
        // 3
        if (min <= m)
        {
            //InsertPlayer(min - 1);
            Push_Player(min-1);
        }


    }
    return min;
}

int main() 
{
    // 여기에 코드를 작성해주세요.
    Input();

    int ans = Solution();
    printf("%d\n", ans);
    return 0;
}
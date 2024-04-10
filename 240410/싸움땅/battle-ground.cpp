#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Player
{
    int x, y, num, dir, base_stat = 0, gun_stat = 0, point = 0;
    bool haveGun = false;
};

vector<Player> p;
vector<int> map[21][21];
int player_map[21][21];
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };


int n, m, k; //보드사이즈 , 플레이어 수 , 라운드 수

int Switch_dir(int input)
{
    int ret = 0;
    switch (input)
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

int Rotate_dir(int input)
{
    int ret = 0;

    switch (input)
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

bool CanMove(int x, int y, int dir)
{
    if (x < 0 || y < 0 || x >= n || y >= n)
    {
        return false;
    }

    return true;
}

void Player_Move(int idx)
{
    //player_map[p[idx].x][p[idx].y] = 0;

    int nx = p[idx].x + dx[p[idx].dir];
    int ny = p[idx].y + dy[p[idx].dir];

    if (!CanMove(nx,ny, p[idx].dir))
    {
        p[idx].dir = Switch_dir(p[idx].dir);
    }

    p[idx].x += dx[p[idx].dir];
    p[idx].y += dy[p[idx].dir];
    //player_map[p[idx].x][p[idx].y] = p[idx].num;
}

int IsPlayer(int x, int y)
{
    if (player_map[x][y] != 0)
    {
        return player_map[x][y]-1;
    }

    return -1;
}

void GetGun(int x, int y,int idx)
{
    if (p[idx].gun_stat>0)
    {        
        map[x][y].push_back(p[idx].gun_stat);
    }

    sort(map[x][y].rbegin(), map[x][y].rend());
    p[idx].gun_stat = map[x][y][0];
    map[x][y].erase(map[x][y].begin());
    
}

pair<int, int> Fight(int a,int b)
{
    // 승자 , 패자
    pair<int, int> ret;
    
    int aSum = p[a].base_stat + p[a].gun_stat;
    int bSum = p[b].base_stat + p[b].gun_stat;
    bool drawFlag = false;

    if (aSum == bSum)
    {
        drawFlag = true;
        if (p[a].base_stat > p[b].base_stat)
        {
            ret.first = a;
            ret.second = b;
        }
        else
        {
            ret.first = b;
            ret.second = a;
        }
    }

    else if (aSum > bSum)
    {
        ret.first = a;
        ret.second = b;
    }
    else
    {
        ret.first = b;
        ret.second = a;
    }

    if (!drawFlag)
    {
        int point = abs(aSum - bSum);
        p[ret.first].point += point;
    }
    

    return ret;
}

void loser_move(int idx)
{
    int cx = p[idx].x;
    int cy = p[idx].y;

    if (p[idx].gun_stat>0)
    {            
        map[cx][cy].push_back(p[idx].gun_stat);
        sort(map[cx][cy].rbegin(), map[cx][cy].rend());
        p[idx].gun_stat = 0;
    }

    int x = p[idx].x, y = p[idx].y, dir = p[idx].dir;

    for (int i = 0; i < 4; i++)
    {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if ((player_map[nx][ny] != 0) || !CanMove(nx, ny, dir))
        {
            dir = Rotate_dir(dir);            
        }
        else
        {
            break;
        }
    }

    p[idx].x += dx[dir];
    p[idx].y += dy[dir];
    p[idx].dir = dir;
    //player_map[cx][cy] = 0;
    player_map[p[idx].x][p[idx].y] = p[idx].num;

    if (map[p[idx].x][p[idx].y].size() > 0)
    {
        GetGun(p[idx].x, p[idx].y, idx);
    }
    
}

void winner_move(int idx)
{
    player_map[p[idx].x][p[idx].y] = idx + 1;

    if(map[p[idx].x][p[idx].y].size()>0)   GetGun(p[idx].x, p[idx].y, idx);   
    
}

void Simulation()
{
    for (int i = 0; i < m; i++)
    {
        int cx = p[i].x;
        int cy = p[i].y;

        //1-1
        Player_Move(i);

        //2-1
        int x, y;
        x = p[i].x, y = p[i].y;

        int stayPeople = IsPlayer(x, y);

        if (stayPeople == -1)
        {            
            if (map[x][y].size() > 0)
            {
                GetGun(x, y, i);
            }            
            player_map[x][y] = i + 1;
            player_map[cx][cy]=0;
        }

        //2-2-1
        else
        {  
            player_map[cx][cy] = 0;
            auto temp = Fight(i, stayPeople);
            int winner = temp.first;
            int loser = temp.second;

            loser_move(loser);
            winner_move(winner);
        }
        
    }
    
}

vector<int> Solution()
{
    vector<int> ret;

    for (int i = 0; i < k; i++)
    {
        Simulation();
    }

    for (int i = 0; i < m; i++)
    {
        ret.push_back(p[i].point);
    }

    return ret;
}

int main()
{

    cin >> n >> m >> k;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int input;
            cin >> input;
            if(input>0) map[i][j].push_back(input);
        }
    }

    for (int i = 0; i < m; i++)
    {
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        x--, y--;
        player_map[x][y]=(i + 1);

        Player temp({ x,y, i + 1,d,s, 0, false }); // x ,y , number , direction , base stat, total stat , score
        p.push_back(temp);
    }

    vector<int> ans = Solution();

    for (int i = 0; i < m; i++)
    {
        printf("%d ", ans[i]);
    }
    return 0;
}
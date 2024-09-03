// 1시간 15분
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <memory.h>
#include <algorithm>
using namespace std;

int map[50][50]; // 0 : empty  , n : santa 
int distmap[50][50];
int n, m, p, c, d, rx, ry, dead_cnt = 0;
// 상 우상 우 우하 하 좌하 좌 좌상
//  0   1  2   3  4   5   6  7
int ru_x[8] = { -1,-1,0,1,1,1,0,-1 };
int ru_y[8] = { 0,1,1,1,0,-1,-1,-1 };
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };

struct Santa
{
    int x, y, num, score = 0, sleep = 0;
    bool alive = true;
};

vector<Santa> Santas;

void ResetMap()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            map[i][j] = 0;
            distmap[i][j] = 0;
        }
    }
}

void Input()
{
    cin >> n >> m >> p >> c >> d;
    cin >> rx >> ry;
    rx -= 1; ry -= 1;
    ResetMap();
    Santas.resize(p);
    for (int i = 0; i < p; i++)
    {
        int num, x, y;
        cin >> num >> x >> y;
        x -= 1; y -= 1;
        map[x][y] = num;        
        Santas[num-1] = { x,y,num };
        
    }
}

int GetDistance(int x1, int y1, int x2, int y2)
{
    int ret = 0;
    ret = pow(abs(x2 - x1), 2) + pow(abs(y2 - y1), 2);

    return ret;
}

void FindSanta(int x, int y)
{

    queue<pair<int, pair<int, int>>> q;
    bool visited[50][50] = { false, };
    q.push({ 0,{x, y} });
    visited[x][y] = true;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            distmap[i][j] = 0;
        }
    }

    while (!q.empty())
    {
        int cx = q.front().second.first;
        int cy = q.front().second.second;
        int cnt = q.front().first;
        q.pop();
        if (map[cx][cy] != 0) distmap[cx][cy] = cnt;

        for (int i = 0; i < 8; i++)
        {
            int nx = cx + ru_x[i];
            int ny = cy + ru_y[i];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            if (!visited[nx][ny])
            {
                q.push({ cnt + 1,{ nx,ny} });
                visited[nx][ny] = true;
            }
        }
    }
}

int Change_dir(bool rudol, int n)
{
    if (rudol)
    {
        switch (n)
        {
        case 0:
            return 4;
            break;
        case 1:
            return 5;
            break;
        case 2:
            return 6;
            break;
        case 3:
            return 7;
            break;
        case 4:
            return 0;
            break;
        case 5:
            return 1;
            break;
        case 6:
            return 2;
            break;
        case 7:
            return 3;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (n)
        {
        case 0:
            return 2;
            break;
        case 1:
            return 3;
            break;
        case 2:
            return 0;
            break;
        case 3:
            return 1;
            break;
        default:
            break;
        }
    }
}

void Interupt(int x,int y,int num,int dir,int kind)
{
    int now_num = map[x][y]-1;
    map[x][y] = num+1;
    Santas[num].y = y;
    Santas[num].x = x;
    int ny = y;
    int nx = x;
    
    if (kind == 1) 
    {
        ny += dy[dir];
        nx += dx[dir];
    }
    else 
    {
        ny += ru_x[dir];
        nx += ru_y[dir];
    }

    if (nx<0 || ny<0 ||nx>=n || ny>=n)
    {
        Santas[now_num].alive = false;
        return;
    }

    else {
        if (map[nx][ny] > 0) {
            Interupt(ny, nx, now_num, dir,kind);
        }
        else {
            map[ny][nx] = now_num+1;
            Santas[now_num].y = ny;
            Santas[now_num].x = nx;
            return;
        }
    }
}

void Update_Rudolph(int idx)
{
    int min = 9999;
    int dir = 0;
    int tox = Santas[idx].x;
    int toy = Santas[idx].y;
    for (int i = 0; i < 8; i++)
    {
        int nx = rx + ru_x[i];
        int ny = ry + ru_y[i];

        int temp = GetDistance(nx+1, ny+1, tox+1, toy+1);
        if (temp < min)
        {
            dir = i;
            min = temp;
        }
    }
    map[rx][ry] = 0;
    int final_rx = rx + ru_x[dir];
    int final_ry = ry + ru_y[dir];    

    // 산타가 있으면
    if (map[final_rx][final_ry] > 0)
    {
        // 산타가 갈 위치 
        int sx = final_rx + c * ru_x[dir];
        int sy = final_ry + c * ru_y[dir];

        // 밀려난 산타의 번호
        int sanNum = map[final_rx][final_ry]-1;
        
        //점수 획득 및 기절
        Santas[sanNum].score += c;
        Santas[sanNum].sleep += 2;

        // 맵밖이면 사망
        if (sx < 0 || sy < 0 || sx >= n || sy >= n)
        {
            Santas[sanNum].alive = false;
            dead_cnt++;
            if (dead_cnt == p)
            {
                for (int idx = 0; idx < p; idx++)
                {
                    printf("%d ", Santas[idx].score);
                }
                exit(1);
            }
        }
        // 안 일경우
        else
        {
            if (map[sx][sy] > 0)
            {
                int tempNum = map[sx][sy]-1;
                map[sx][sy] = sanNum+1;
                Santas[sanNum].x = sx;
                Santas[sanNum].y = sy;

                // 그다음 위치에 산타확인
                int nx = sx + ru_x[dir];
                int ny = sy + ru_y[dir];
                                
                if (nx < 0 || ny < 0 || nx >= n || ny >= n)
                {
                    Santas[tempNum].alive = false;
                    dead_cnt++;
                    if (dead_cnt == p)
                    {
                        for (int idx = 0; idx < p; idx++)
                        {
                            printf("%d ", Santas[idx].score);
                        }
                        exit(1);
                    }
                }

                // 그곳에 또 산타가 있다면
                if (map[nx][ny] > 0)
                {
                    //연쇄작용
                    Interupt(nx, ny, tempNum, dir, 2);
                }
                // 없으면
                else
                {
                    map[nx][ny] = tempNum+1;
                    Santas[tempNum].x = nx;
                    Santas[tempNum].y = ny;
                }
            }
            // 현재의 산타 다음에 다른산타 없을때
            else
            {
                map[sx][sy] = sanNum+1;
                Santas[sanNum].x = sx;
                Santas[sanNum].y = sy;
            }
        }
    }    
    rx = final_rx;
    ry = final_ry;
    map[rx][ry] = -1;
}

void Move_Rudolph()
{
    int min = 9999;
    // 가까운 산타 찾기
    FindSanta(rx, ry);

    // 8방향 찾기
    int minSanta = 0;
    for (int i = 0; i < p; i++)
    {
        if (Santas[i].alive)
        {            
            if (distmap[Santas[i].x][Santas[i].y] < min)
            {
                min = distmap[Santas[i].x][Santas[i].y];
                minSanta = i + 1;
            }
            else if (distmap[Santas[i].x][Santas[i].y] == min)
            {
                if (Santas[i].x > Santas[minSanta - 1].x)
                {
                    min = distmap[Santas[i].x][Santas[i].y];
                    minSanta = i + 1;
                }
                else if(Santas[i].x == Santas[minSanta - 1].x)
                {
                    if (Santas[i].y > Santas[minSanta - 1].y)
                    {
                        min = distmap[Santas[i].x][Santas[i].y];
                        minSanta = i + 1;
                    }
                }
            }
        }
    }
    // 해당방향으로 움직임
    Update_Rudolph(minSanta - 1);

}

void Move_Santa()
{
    for (int idx = 0; idx < p; idx++)
    {
        // 움직일수 없는 경우 제외
        if (!Santas[idx].alive || Santas[idx].sleep > 0) continue;
        int cx = Santas[idx].x;
        int cy = Santas[idx].y;
        int min = GetDistance(cx + 1, cy + 1, rx + 1, ry + 1);
        int dir = -1;
        int to_x, to_y;
        bool canGo = false;
        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            int res = GetDistance(nx+1, ny+1, rx+1, ry+1);            
            if (res < min && map[nx][ny]<1)
            {
                min = res;
                dir = i;
            }
        }
        if (dir > -1)
        {
            map[cx][cy] = 0;
            to_x =  Santas[idx].x + dx[dir];
            to_y = Santas[idx].y + dy[dir];            
            canGo = true;
        }

        if (!canGo) continue;

        // 루돌프가 있으면 충돌
        if (to_x == rx && to_y == ry)
        {
            // 점수 추가
            Santas[idx].score += d;
            Santas[idx].sleep += 2;
            dir = Change_dir(false, dir);

            int nx = to_x + d * dx[dir];
            int ny = to_y + d * dy[dir];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n)
            {
                dead_cnt++;
                Santas[idx].alive = false;
                if (dead_cnt == p)
                {
                    for (int i = 0; i < p; i++)
                    {
                        printf("%d ", Santas[i].score);
                    }
                    exit(1);
                }
                map[cx][cy] = 0;
                continue;
            }

            if (map[nx][ny] > 0)
            {
                int temp_num = map[nx][ny] - 1;
                map[cx][cy] = 0;
                map[nx][ny] = idx + 1;
                Santas[idx].x = nx;
                Santas[idx].y = ny;

                int nnx = nx + dx[dir];
                int nny = ny + dy[dir];

                if (nx < 0 || ny < 0 || nx >= n || ny >= n)
                {
                    Santas[temp_num].alive = false;
                    dead_cnt++;
                    if (dead_cnt == p)
                    {
                        for (int i = 0; i < p; i++)
                        {
                            printf("%d ", Santas[i].score);
                        }
                        exit(1);
                    }
                    continue;
                }
                if (map[nnx][nny] > 0)
                {
                    Interupt(nnx, nny, temp_num, dir, 1);
                }
                else
                {
                    map[nnx][nny] = temp_num+1;
                    Santas[temp_num].x = nnx;
                    Santas[temp_num].y = nny;
                }
            }
            else
            {
                map[cx][cy] = 0;
                map[nx][ny] = idx + 1;
                Santas[idx].x = nx;
                Santas[idx].y = ny;
            }
        }
        else
        {
            map[cx][cy] = 0;
            map[to_x][to_y] = idx+1;
            Santas[idx].x = to_x;
            Santas[idx].y = to_y;
        }
    }
    
}


void Simul()
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            if (Santas[j].sleep > 0 && Santas[j].alive) Santas[j].sleep--;
        }

        Move_Rudolph();
        Move_Santa();
        
        for (int j = 0; j < p; j++)
        {
            if (Santas[j].alive) Santas[j].score++;
        }
    }

    for (int i = 0; i < p; i++)
    {
        printf("%d ", Santas[i].score);
    }
}

int main()
{
    memset(map, -1, sizeof(map));
    memset(distmap, -1, sizeof(distmap));
    Input();
    Simul();
    return 0;
}
#include <iostream>
#include <vector>

using namespace std;

struct Player
{
    int x, y, num, dir,base_stat=0 , gun_stat=0 , point=0;
    bool haveGun = false;
};

vector<Player> p;
vector<int> map[21][21];
vector<int> player_map[21][21];
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };
bool fight = false;

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

void player_move(int player_num)
{
    int dir = p[player_num].dir;
    int cx = p[player_num].x;
    int cy = p[player_num].y;

    int nx = cx + dx[dir];
    int ny = cy + dy[dir];

    // 정반대로
    if (nx < 0 || ny < 0 || nx >= n || ny >= n)
    {
        dir = Switch_dir(dir);
        nx = cx + dx[dir];
        ny = cy + dy[dir];
    }

    p[player_num].x = nx;
    p[player_num].y = ny;
    p[player_num].dir = dir;

    player_map[nx][ny].push_back(p[player_num].num);
    player_map[cx][cy].erase(player_map[cx][cy].begin());
}

void loser_move(int player_num , int winner_num)
{
    int dir = p[player_num].dir;
    int cx = p[player_num].x;
    int cy = p[player_num].y;

    int nx = cx + dx[dir];
    int ny = cy + dy[dir];

    // 90도 회전 및 레인지 검사
    if ((nx < 0 || ny < 0 || nx >= n || ny >= n))
    {
        for (int i = 0; i < 4; i++)
        {
            dir = Rotate_dir(dir);
            nx = cx + dx[dir];
            ny = cy + dy[dir];

            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            if (player_map[nx][ny].size() == 0) break;
        }        
    }

    // 이동 좌표 갱신
    p[player_num].x = nx;
    p[player_num].y = ny;
    p[player_num].dir = dir;

    // 총 확인
    if (map[nx][ny].size() > 0)
    {
        int maxDamge = 0, idx = 0;
        bool flag = false;
        for (int i = 0; i < map[nx][ny].size(); i++)
        {
            if (maxDamge < map[nx][ny][i])
            {
                maxDamge = map[nx][ny][i];
                idx = i;
                flag = true;
            }
        }
        if (flag)
        {
            // 획득 및 맵갱신
            p[player_num].haveGun = true;
            p[player_num].gun_stat = map[nx][ny][idx];
            map[nx][ny][idx] = 0;
            //map[nx][ny].erase(map[nx][ny].begin() + idx);
        }
        
    }
    player_map[cx][cy].clear();
    player_map[nx][ny].push_back(p[player_num].num);
    player_map[cx][cy].push_back(p[winner_num].num);
    
    
}

void winner_move(int player_num)
{
    int cx = p[player_num].x;
    int cy = p[player_num].y;

    if (map[cx][cy].size() > 0)
    {
        int maxDamge = 0, idx = 0;
        bool flag = false;
        for (int i = 0; i < map[cx][cy].size(); i++)
        {
            if (maxDamge < map[cx][cy][i])
            {
                flag = true;
                maxDamge = map[cx][cy][i];
                idx = i;
            }
        }

        if (flag)
        {
            // 획득 및 맵갱신
            p[player_num].haveGun = true;
            p[player_num].gun_stat = map[cx][cy][idx];
            map[cx][cy][idx] = 0;
            //map[cx][cy].erase(map[cx][cy].begin() + idx);
        }
    }
    
}

void Move()
{
    // 플레이어 수만큼
    for (int i = 0; i < m; i++)
    {
        player_move(i);
        //int dir = p[i].dir;
        //int cx = p[i].x;
        //int cy = p[i].y;
        //
        //int nx = cx + dx[dir];
        //int ny = cy + dy[dir];

        //// 정반대로
        //if (nx < 0 || ny < 0 || nx >= n || ny >= n)
        //{
        //    dir = Switch_dir(dir);
        //    nx = cx + dx[dir];
        //    ny = cy + dy[dir];
        //}

        int nx = p[i].x;
        int ny = p[i].y;
        

        // 플레이어 없음
        if (player_map[nx][ny].size() == 1)
        {
            int maxDamge = 0 , idx =0;

            // 가진 총 없으면
            if (!p[i].haveGun)
            {
                // 맵에 총이 있으면
                if (map[nx][ny].size() > 0)
                {
                    bool flag = false;
                    // 공격력 높은 총 확인
                    for (int j = 0; j < map[nx][ny].size(); j++)
                    {
                        if (map[nx][ny][j] > maxDamge)
                        {
                            
                            maxDamge = map[nx][ny][j];
                            idx = j;
                            flag = true;
                        }
                    }
                    if (flag)
                    {
                        p[i].haveGun = true;
                        p[i].gun_stat = maxDamge;

                        // 공격력 높은총 제거
                        map[nx][ny][idx] = 0;
                        //map[nx][ny].erase(map[nx][ny].begin() + idx);
                    }
                    
                }
                
            }

            // 총 있으면
            else
            {
                if (map[nx][ny].size() > 0)
                {
                    int now = p[i].gun_stat;
                    int dix = 0;
                    bool flag = false;
                    // 공격력 높은 총 확인
                    for (int j = 0; j < map[nx][ny].size(); j++)
                    {
                        if (map[nx][ny][j] > now)
                        {
                            p[j].gun_stat = maxDamge;
                            idx = j;
                            flag = true;
                        }
                    }
                    if (flag)
                    {
                        // 공격력 높은총 제거
                        map[nx][ny][idx] = 0;
                        //map[nx][ny].erase(map[nx][ny].begin() + idx);
                        // 가지고 있던 총 추가
                        map[nx][ny].push_back(now);
                    }
                    
                }
                
            }

            
        }
        
        // 플레이어 있음
        if (player_map[nx][ny].size() > 1)
        {
            // 싸움
            int now_player=0, vs_player=0;

            for (int j = 0; j < player_map[nx][ny].size(); j++)
            {
                if (j == 0) now_player = player_map[nx][ny][j] -1;
                if (j == 1) vs_player = player_map[nx][ny][j] -1;
            }
            

            // 이기는 플레이어 1로 설정 후 비교
            int win_player = 1;

            int now_total = (p[now_player].base_stat + p[now_player].gun_stat);
            int vs_total = (p[vs_player].base_stat + p[vs_player].gun_stat);
            
            // 총 공격력이 같으면
            if (now_total == vs_total)
            {
                if (p[now_player].base_stat < p[vs_player].base_stat)
                {
                    win_player = 2;
                }
            }
            else
            {
                if (now_total < vs_total) win_player = 2;
            }

            // 이긴 플레이어 및 진플레이어 해당내용 수행
            if (win_player == 1)
            {
                // 이긴사람 포인트 추가
                p[now_player].point += (now_total - vs_total);
                
                // 진 플레이어 무브
                loser_move(vs_player , now_player);                
                p[vs_player].haveGun = false;
                winner_move(now_player);
                
                
            }
            else
            {
                p[vs_player].point += (vs_total - now_total);

                loser_move(now_player , vs_player);
                p[now_player].haveGun = false;
                winner_move(vs_player);
            }

            
        }
    }
}

vector<int> Solution()
{
    vector<int> ret;
    
    for (int i = 0; i < k; i++)
    {
        Move();
    }
    
    for (int i = 0; i < m; i++)
    {
        int pt = p[i].point;
        ret.push_back(pt);
    }

    return ret;
}

int main() 
{
    // 여기에 코드를 작성해주세요.

    cin >> n >> m >> k;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int input;
            cin >> input;
            map[i][j].push_back(input);
        }        
    }

    for (int i = 0; i < m; i++)
    {
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        x--, y--;
        player_map[x][y].push_back(i+1);

        Player temp({ x,y, i+1,d,s, 0, false}); // x ,y , number , direction , base stat, total stat , score
        p.push_back(temp);
    }

    vector<int> ans = Solution();
   
    for (int i = 0; i < m; i++)
    {
        printf("%d ", ans[i]);
    }

    return 0;
}
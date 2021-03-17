#include "Play.h"
#include "Game.h"

Play::Play(){}
Play::~Play(){}
Play *Play::_instance = nullptr;
Game *p = Game::getInstance();

void Play::spawnTile(){
    srand(time(NULL));
    p->spawnTileX = Game::getInstance()->randInt(0, 6), p->spawnTileY = 0;
    for(int i = 0, j = 0; i < 7; ++i)
        if(p->board[i][j]){
            Scene::getInstance()->MODE = 0;
            return;
        }
    p->board[p->spawnTileX][p->spawnTileY] = p->spawnQueue.front(); p->spawnQueue.pop_front();
    p->spawnQueue.push_back(Game::getInstance()->randInt(1, 27)); //27
}
void Play::relocateTile(int x, int y){
    p->board[p->spawnTileX + x][p->spawnTileY + y] = p->board[p->spawnTileX][p->spawnTileY];
    p->board[p->spawnTileX][p->spawnTileY] = 0;
    p->spawnTileX += x;
    p->spawnTileY += y;
}
bool Play::checkPoint(){
    int check[7][10];
    bool isGetPoint = 0;
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j){
            check[i][j] = 0;
            if(p->board[i][j]){
                int current = p->board[i][j], lx = i, rx = i;
                while(current == p->board[lx][j] && lx >= 0) --lx; lx += 1;
                while(current == p->board[rx][j] && rx <= 6) ++rx; rx -= 1;
                if(rx - lx + 1 >= 3) check[i][j] = current;
                int ly = j, ry = j;
                while(current == p->board[i][ly] && ly >= 0) --ly; ly += 1;
                while(current == p->board[i][ry] && ry <= 9) ++ry; ry -= 1;
                if(ry - ly + 1 >= 3) check[i][j] = current;
            }
        }
    for(int i = 0; i < 7; ++i){
        for(int j = 0; j < 10; ++j){
            if(check[i][j]){
                p->score += check[i][j];
                p->board[i][j] = 0;
                isGetPoint = 1;
            }
        }
    }
    return isGetPoint;
}
void Play::gravityFall(){
    for(int i = 0; i < 7; ++i){
        std::vector <int> current;
        for(int j = 0; j <= 9; ++j) if(p->board[i][j]) current.push_back(p->board[i][j]);
        for(int j = 9; j >= 0; --j)
            if(current.size()) p->board[i][j] = current.back(), current.pop_back();
            else p->board[i][j] = 0;
    }
}
void Play::addNumberToBlock(){
    int expression = p->board[p->spawnTileX][p->spawnTileY] - 14;
    if(p->expressionData[expression][0] == '+') p->board[p->spawnTileX][p->spawnTileY+1] += (p->expressionData[expression][1] - '0');
    if(p->expressionData[expression][0] == '-') p->board[p->spawnTileX][p->spawnTileY+1] -= (p->expressionData[expression][1] - '0');
    if(p->expressionData[expression][0] == '*') p->board[p->spawnTileX][p->spawnTileY+1] *= (p->expressionData[expression][1] - '0');
    if(p->expressionData[expression][0] == '/') p->board[p->spawnTileX][p->spawnTileY+1] /= (p->expressionData[expression][1] - '0');
    if(p->expressionData[expression][0] == '%') p->board[p->spawnTileX][p->spawnTileY+1] %= (p->expressionData[expression][1] - '0');
    if(p->board[p->spawnTileX][p->spawnTileY+1] >= 15) p->board[p->spawnTileX][p->spawnTileY+1] = 28, Scene::getInstance()->MODE = 0;
    if(p->board[p->spawnTileX][p->spawnTileY+1] < 0) p->board[p->spawnTileX][p->spawnTileY+1] = 0;
    p->board[p->spawnTileX][p->spawnTileY] = 0;
}
#include <SDL2/SDL.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <deque>
using namespace std;
struct SDL_BMP{
    SDL_Texture *texture;
    SDL_Rect sourceRect, destinationRect;
};
int randInt(int l, int r){
    return rand()%r + l;
}
class SDL{
    ///System
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Event event;
    SDL_BMP boardTexture, boardOutline, tile[7][11], spawnQueueDraw[3];
    int board[7][11];
    int spawnTileX = -2, spawnTileY = -2;
    int score = 0;
    deque <int> spawnQueue;
    string expressionData[15] = {"", "+1", "-1", "+2", "-2", "*2", "/2", "%2", "+3", "-3", "*3", "/3", "%3", "-4", "??"};
    bool gravity = 0; ///fall block when get point
    bool gameOver = 0;
    bool isSpawned = true;
    ///Config
    int speed = 1000;
    int baseScore = 0;
    ///Layout
    const int blockSize = 62;
    const int paddingGameplay = 50;
    const int paddingUI_x = 534, paddingScoreY = 0, paddingBestY = 0, paddingQueueY = 298;
    const int marginQueue = 62;
    ///Time
    int lastTick = 0;
    ///Processing Image Functions
    SDL_Rect cutTile(SDL_Texture *texture, int number){///cut image from big image
        SDL_Rect renderSize;
        renderSize.x = blockSize * number;
        renderSize.y = 0;
        renderSize.w = renderSize.h = blockSize;
        return renderSize;
    }
    SDL_Rect setTile(SDL_Texture *texture, double positionX, double positionY){///set position
        SDL_Rect renderSize;
        renderSize.x = positionX;
        renderSize.y = positionY;
        renderSize.w = renderSize.h = blockSize;
        return renderSize;
    }
    SDL_BMP loadTile(const char *path, int posX, int posY, int number){
        SDL_Surface *surface = SDL_LoadBMP(path);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);

        SDL_Rect renderSourceRect = cutTile(texture, number);
        SDL_Rect renderDestinationRect = setTile(texture, posX, posY);

        SDL_FreeSurface(surface);
        return {texture, renderSourceRect, renderDestinationRect};
    }
    SDL_Rect setTexture(SDL_Texture *texture, double positionX, double positionY){///set position
        SDL_Rect renderSize;
        renderSize.x = positionX;
        renderSize.y = positionY;
        SDL_QueryTexture(texture, NULL, NULL, &renderSize.w, &renderSize.h);
        return renderSize;
    }
    SDL_BMP loadTexture(const char *path, int posX, int posY){
        SDL_Surface *surface = SDL_LoadBMP(path);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);

        SDL_Rect renderSourceRect;
        SDL_Rect renderDestinationRect = setTexture(texture, posX, posY);

        SDL_FreeSurface(surface);
        return {texture, renderSourceRect, renderDestinationRect};
    }
    ///System Function
    void spawnTile(){
        spawnTileX = randInt(0, 6), spawnTileY = 0;
        for(int i = 0, j = 0; i < 7; ++i)
            if(board[i][j]){
                gameOver = 1;
                return;
            }
        board[spawnTileX][spawnTileY] = spawnQueue.front(); spawnQueue.pop_front();
        spawnQueue.push_back(randInt(1, 27)); //27
    }
    void relocateTile(int x, int y){
        board[spawnTileX + x][spawnTileY + y] = board[spawnTileX][spawnTileY];
        board[spawnTileX][spawnTileY] = 0;
        spawnTileX += x;
        spawnTileY += y;
    }
    bool checkPoint(){
        int check[7][10];
        bool isGetPoint = 0;
        for(int i = 0; i < 7; ++i)
            for(int j = 0; j < 10; ++j){
                check[i][j] = 0;
                if(board[i][j]){
                    int current = board[i][j], lx = i, rx = i;
                    while(current == board[lx][j] && lx >= 0) --lx; lx += 1;
                    while(current == board[rx][j] && rx <= 6) ++rx; rx -= 1;
                    if(rx - lx + 1 >= 3) check[i][j] = current;

                    int ly = j, ry = j;
                    while(current == board[i][ly] && ly >= 0) --ly; ly += 1;
                    while(current == board[i][ry] && ry <= 9) ++ry; ry -= 1;
                    if(ry - ly + 1 >= 3) check[i][j] = current;
                }
            }
        for(int i = 0; i < 7; ++i){

            for(int j = 0; j < 10; ++j){
                cout << board[i][j] << ' ';
                if(check[i][j]){
                    score += check[i][j];
                    board[i][j] = 0;
                    isGetPoint = 1;
                }
            }
            cout << '\n';
        }
        cout << '\n';
        return isGetPoint;
    }
    void gravityFall(){
        for(int i = 0; i < 7; ++i){
            vector <int> current;
            for(int j = 0; j <= 9; ++j) if(board[i][j]) current.push_back(board[i][j]);
            for(int j = 9; j >= 0; --j)
                if(current.size()) board[i][j] = current.back(), current.pop_back();
                else board[i][j] = 0;
        }
    }
    void addNumberToBlock(){
        int expression = board[spawnTileX][spawnTileY] - 14;
        if(expressionData[expression][0] == '+') board[spawnTileX][spawnTileY+1] += (expressionData[expression][1] - '0');
        if(expressionData[expression][0] == '-') board[spawnTileX][spawnTileY+1] -= (expressionData[expression][1] - '0');
        if(expressionData[expression][0] == '*') board[spawnTileX][spawnTileY+1] *= (expressionData[expression][1] - '0');
        if(expressionData[expression][0] == '/') board[spawnTileX][spawnTileY+1] /= (expressionData[expression][1] - '0');
        if(expressionData[expression][0] == '%') board[spawnTileX][spawnTileY+1] %= (expressionData[expression][1] - '0');
        if(board[spawnTileX][spawnTileY+1] >= 15) board[spawnTileX][spawnTileY+1] = 28, gameOver = 1;
        if(board[spawnTileX][spawnTileY+1] < 0) board[spawnTileX][spawnTileY+1] = 0;
        board[spawnTileX][spawnTileY] = 0;
    }
public:
    SDL(){
        window = SDL_CreateWindow("Matching Expression", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, SDL_WINDOW_SHOWN);
        render = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        boardTexture = loadTexture("MatchingExpression_data\\images\\background.bmp", paddingGameplay, paddingGameplay);
        boardOutline = loadTexture("MatchingExpression_data\\images\\outline.bmp", paddingGameplay, paddingGameplay);
        ///i is column; j is height;
        for(int i = 0; i < 7; ++i)
            for(int j = 0; j < 11; ++j)
                if(j == 10) board[i][j] = -1;
                else board[i][j] = 0;
        for(int i = 0; i < 3; ++i) spawnQueue.push_back(randInt(1, 14));
        spawnTile();
    }
    bool running(){
        if(gameOver) return cout << "Game over", 0;
        while(SDL_PollEvent(&event)){
            switch(event.type){///Handle Input
                case SDL_KEYDOWN:{
                    if(isSpawned){
                        if(event.key.keysym.sym == SDLK_a && spawnTileX > 0 && board[spawnTileX-1][spawnTileY] == 0) relocateTile(-1, 0);
                        if(event.key.keysym.sym == SDLK_d && spawnTileX < 6 && board[spawnTileX+1][spawnTileY] == 0) relocateTile(1, 0);
                        if(event.key.keysym.sym == SDLK_s && board[spawnTileX][spawnTileY+1] == 0) relocateTile(0, 1);
                    }
                    break;
                }
                case SDL_KEYUP: break;
                case SDL_QUIT: return 0;
                default: break;
            }
        }
        return 1;
    }
    void update(){
        ///speed 1000 500 250...
        ///speed /= 2 when over 100pts (maximum get 14*7+1 = 99 pts < 100)
        if(score / 100 > baseScore) speed /= 2, baseScore = score/100;
        ///Move Tile
        if(isSpawned){
            if(SDL_GetTicks() - lastTick >= speed){
                if(board[spawnTileX][spawnTileY+1] == 0) relocateTile(0, 1);
                else{
                    if(board[spawnTileX][spawnTileY] > 14) addNumberToBlock();
                    score += 1;
                    isSpawned = 0;
                }
                lastTick = SDL_GetTicks();
            }
        }
        else{
            if(gravity) gravityFall(), gravity = 0, SDL_Delay(250);
            else{
                if(checkPoint()) gravity = 1, SDL_Delay(250);
                else isSpawned = true, spawnTile(), lastTick = SDL_GetTicks(), cout << "Score: " << score << " Speed: " << speed << '\n';
            }

        }

        for(int i = 0, j = 10; i < 7; ++i) board[i][j] = -1;
        for(int i = 0; i < 7; ++i)
            for(int j = 0; j < 10; ++j)
                if(board[i][j]){
                    if(board[i][j] <= 14) tile[i][j] = loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]);
                    else tile[i][j] = loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]-14);
                }
        int cnt = 0;
        for(auto v: spawnQueue){
            if(v <= 14) spawnQueueDraw[cnt] = loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingUI_x, paddingQueueY + (marginQueue+blockSize)*cnt, v);
            else spawnQueueDraw[cnt] = loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingUI_x, paddingQueueY + (marginQueue+blockSize)*cnt, v-14);
            cnt++;
        }
    }
    void draw(){
        SDL_RenderClear(render);
        SDL_RenderCopy(render, boardTexture.texture,&boardTexture.sourceRect, &boardTexture.destinationRect);
        for(int i = 0; i < 7; ++i)
            for(int j = 0; j < 10; ++j)
                if(board[i][j]){
                    SDL_RenderCopy(render, tile[i][j].texture, &tile[i][j].sourceRect, &tile[i][j].destinationRect);
                    SDL_DestroyTexture(tile[i][j].texture);
                }
        for(int i = 0; i < 3; ++i){
            SDL_RenderCopy(render, spawnQueueDraw[i].texture, &spawnQueueDraw[i].sourceRect, &spawnQueueDraw[i].destinationRect);
            SDL_DestroyTexture(spawnQueueDraw[i].texture);
        }
        SDL_RenderCopy(render, boardOutline.texture,&boardOutline.sourceRect, &boardOutline.destinationRect);
        SDL_RenderPresent(render);
    }
    ~SDL(){
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(render);
        SDL_Quit();
    }
};
int main(int argc, char * argv[])
{
    srand(time(NULL));
    SDL app;
    while(app.running()){
        app.update();
        app.draw();
    }
    return 0;
}


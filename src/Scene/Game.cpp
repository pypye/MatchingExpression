#include "Game.h"
#include "Graphics.h"
#include "Play.h"
Game::Game(){}
Game::~Game(){}
Game *Game::_instance = nullptr;
void Game::Init()
{
    srand(time(NULL));
    int paddingGameplay = 50;
    boardTexture = Graphics::getInstance()->loadTexture("MatchingExpression_data\\images\\background.bmp", paddingGameplay, paddingGameplay);
    boardOutline = Graphics::getInstance()->loadTexture("MatchingExpression_data\\images\\outline.bmp", paddingGameplay, paddingGameplay);///i is column; j is height;
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 11; ++j)
            if(j == 10) board[i][j] = -1;
            else board[i][j] = 0;
    for(int i = 0; i < 3; ++i) spawnQueue.push_back(randInt(1, 14));
    Play::getInstance()->spawnTile();
}

void Game::Event()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){///Handle Input
            case SDL_KEYDOWN:{
                if(isSpawned){
                    if(event.key.keysym.sym == SDLK_a && spawnTileX > 0 && board[spawnTileX-1][spawnTileY] == 0) Play::getInstance()->relocateTile(-1, 0);
                    if(event.key.keysym.sym == SDLK_d && spawnTileX < 6 && board[spawnTileX+1][spawnTileY] == 0) Play::getInstance()->relocateTile(1, 0);
                    if(event.key.keysym.sym == SDLK_s && board[spawnTileX][spawnTileY+1] == 0) Play::getInstance()->relocateTile(0, 1);
                }
                break;
            }
            case SDL_KEYUP: break;
            case SDL_QUIT:
                Scene::getInstance()->MODE = 0;
                break;
            default: break;
        }
    }
}

void Game::Update()
{
    ///speed 1000 500 250...
    ///speed /= 2 when over 100pts (maximum get 14*7+1 = 99 pts < 100)
    if(score / 100 > baseScore) speed /= 2, baseScore = score/100;
    ///Move Tile
    if(isSpawned){
        if(SDL_GetTicks() - lastTick >= speed){
            if(board[spawnTileX][spawnTileY+1] == 0) Play::getInstance()->relocateTile(0, 1);
            else{
                if(board[spawnTileX][spawnTileY] > 14) Play::getInstance()->addNumberToBlock();
                score += 1;
                isSpawned = 0;
            }
            lastTick = SDL_GetTicks();
        }
    }
    else{
        if(gravity) Play::getInstance()->gravityFall(), gravity = 0, SDL_Delay(250);
        else{
            if(Play::getInstance()->checkPoint()) gravity = 1, SDL_Delay(250);
            else isSpawned = true, Play::getInstance()->spawnTile(), lastTick = SDL_GetTicks(), std::cout << "Score: " << score << " Speed: " << speed << '\n';
        }

    }
    for(int i = 0, j = 10; i < 7; ++i) board[i][j] = -1;
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j)
            if(board[i][j]){
                if(board[i][j] <= 14) tile[i][j] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]);
                else tile[i][j] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]-14);
            }
    int cnt = 0;
    for(auto v: spawnQueue){
        if(v <= 14) spawnQueueDraw[cnt] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingUI_x, paddingQueueY + (marginQueue+blockSize)*cnt, v);
        else spawnQueueDraw[cnt] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingUI_x, paddingQueueY + (marginQueue+blockSize)*cnt, v-14);
        cnt++;
    }
}

void Game::Draw()
{
    SDL_RenderClear(Scene::getInstance()->getRenderer());
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), boardTexture.texture, NULL, &boardTexture.destinationRect);
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j)
            if(board[i][j]){
                SDL_RenderCopy(Scene::getInstance()->getRenderer(), tile[i][j].texture, &tile[i][j].sourceRect, &tile[i][j].destinationRect);
                SDL_DestroyTexture(tile[i][j].texture);
            }
    for(int i = 0; i < 3; ++i){
        SDL_RenderCopy(Scene::getInstance()->getRenderer(), spawnQueueDraw[i].texture, &spawnQueueDraw[i].sourceRect, &spawnQueueDraw[i].destinationRect);
        SDL_DestroyTexture(spawnQueueDraw[i].texture);
    }
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), boardOutline.texture, NULL, &boardOutline.destinationRect);
    SDL_RenderPresent(Scene::getInstance()->getRenderer());
}

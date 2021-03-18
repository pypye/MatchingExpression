#include "Game.h"
#include "Graphics.h"
#include "Play.h"
#include "Text.h"
#include <string>

Game::Game(){}
Game::~Game(){}
Game *Game::_instance = nullptr;
void Game::Init()
{
    srand(time(NULL));
    ///init UI
    boardTexture = Graphics::getInstance()->loadTexture("MatchingExpression_data\\images\\background.bmp", paddingGameplay, paddingGameplay);
    boardOutline = Graphics::getInstance()->loadTexture("MatchingExpression_data\\images\\outline.bmp", paddingGameplay, paddingGameplay);
    for(int i = 0; i < 3; ++i)
        UITexture[i] = Graphics::getInstance()->loadTexture("MatchingExpression_data\\images\\ui.bmp", paddingUI_x, paddingGameplay + (sizeUI_y + marginUI_y) * i);

    UIScore = Text::getInstance()->loadText("SCORE", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 579, 57);
    UILevel = Text::getInstance()->loadText("LEVEL", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 582, 153);
    UITime = Text::getInstance()->loadText("TIME", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 585, 249);
    UINext = Text::getInstance()->loadText("NEXT BLOCK", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 558, 344);

    UINextTexture = Graphics::getInstance()->loadTexture("MatchingExpression_data\\images\\ui_next.bmp", paddingUI_x, paddingGameplay + (sizeUI_y + marginUI_y) * 3);
    ///Init Gameplay
    ///i is column; j is height;
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
    ///Update Gameplay

    ///speed 1000 500 250...
    ///speed /= 2 when over 500 pts (maximum get 14*7+1 = 99 pts < 500)
    if(score / 500 > baseScore) speed /= 2, baseScore = score/500, level ++;
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
            else isSpawned = true, Play::getInstance()->spawnTile(), lastTick = SDL_GetTicks();
        }

    }
    ///update UI
    for(int i = 0, j = 10; i < 7; ++i) board[i][j] = -1;
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j)
            if(board[i][j]){
                if(board[i][j] <= 14) tile[i][j] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]);
                else tile[i][j] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]-14);
            }
    int cnt = 0;
    for(auto v: spawnQueue){
        if(v <= 14) spawnQueueDraw[cnt] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingUI_x + marginQueue_x, paddingQueue_y + (marginQueue_y+blockSize)*cnt, v);
        else spawnQueueDraw[cnt] = Graphics::getInstance()->loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingUI_x + marginQueue_x, paddingQueue_y + (marginQueue_y+blockSize)*cnt, v-14);
        cnt++;
    }
    Time = SDL_GetTicks()/1000;
    UIScoreCount = Text::getInstance()->loadText(std::to_string(score).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(score).length()*4, 96); /// std::to_string(score).length()*4 -> center text
    UILevelCount = Text::getInstance()->loadText(std::to_string(level).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(level).length()*4, 193);
    UITimeCount = Text::getInstance()->loadText(std::to_string(Time).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(Time).length()*4, 289);

}

void Game::Draw()
{
    SDL_RenderClear(Scene::getInstance()->getRenderer());
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), boardTexture.texture, NULL, &boardTexture.destinationRect);
    for(int i = 0; i < 3; ++i) SDL_RenderCopy(Scene::getInstance()->getRenderer(), UITexture[i].texture, NULL, &UITexture[i].destinationRect);
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UINextTexture.texture, NULL, &UINextTexture.destinationRect);
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


    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UIScore.texture, NULL, &UIScore.destinationRect);
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UILevel.texture, NULL, &UILevel.destinationRect);
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UITime.texture, NULL, &UITime.destinationRect);
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UINext.texture, NULL, &UINext.destinationRect);

    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UIScoreCount.texture, NULL, &UIScoreCount.destinationRect);
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UILevelCount.texture, NULL, &UILevelCount.destinationRect);
    SDL_RenderCopy(Scene::getInstance()->getRenderer(), UITimeCount.texture, NULL, &UITimeCount.destinationRect);
    SDL_DestroyTexture(UIScoreCount.texture);
    SDL_DestroyTexture(UILevelCount.texture);
    SDL_DestroyTexture(UITimeCount.texture);


    SDL_RenderPresent(Scene::getInstance()->getRenderer());

}

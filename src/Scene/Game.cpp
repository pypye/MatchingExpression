#include "Game.h"
#include "Graphics.h"
#include "Play.h"
#include "Text.h"
#include <string>
#include <math.h>
#include <algorithm>
Game::Game(){}
Game::~Game(){}
std::string separated(int number){
    std::string num = std::to_string(number);
    std::string sepNum = std::string();
    int cnt = 3;
    for(auto i = num.rbegin(); i != num.rend() ; ++i){
        if(!cnt) sepNum.push_back(','), cnt = 3;
        if(cnt--) sepNum.push_back(*i);
    }
    std::reverse(sepNum.begin(), sepNum.end());
    return sepNum;
}
///if texture load many times, must destroy it in draw;
void Game::Init()
{
    srand(time(NULL));
    ///init UI
    boardTexture = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\background.bmp", paddingGameplay, paddingGameplay);
    boardOutline = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\outline.bmp", paddingGameplay, paddingGameplay);
    for(int i = 0; i < 3; ++i)
        UITexture[i] = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\ui.bmp", paddingUI_x, paddingGameplay + (sizeUI_y + marginUI_y) * i);

    UIScore = Text::getInstance().loadText("SCORE", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 579, 57);
    UILevel = Text::getInstance().loadText("LEVEL", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 582, 153);
    UIRange = Text::getInstance().loadText("MAX VALUE", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 561, 249);
    UINext = Text::getInstance().loadText("NEXT BLOCK", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 558, 344);

    UINextTexture = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\ui_next.bmp", paddingUI_x, paddingGameplay + (sizeUI_y + marginUI_y) * 3);
    ///Init Gameplay
    ///i is column; j is height;
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 11; ++j)
            if(j == 10) board[i][j] = -1;
            else board[i][j] = 0;
    for(int i = 0; i < 3; ++i)
        spawnQueue.push_back(randInt(1, range));
    Play::getInstance().spawnTile();


}

void Game::Event()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){///Handle Input
            case SDL_KEYDOWN:{
                if(isSpawned){
                    if(event.key.keysym.sym == SDLK_a && spawnTileX > 0 && board[spawnTileX-1][spawnTileY] == 0 && !Play::getInstance().pressedDown) Play::getInstance().relocateTile(-1, 0);
                    if(event.key.keysym.sym == SDLK_d && spawnTileX < 6 && board[spawnTileX+1][spawnTileY] == 0 && !Play::getInstance().pressedDown) Play::getInstance().relocateTile(1, 0);
                    if(event.key.keysym.sym == SDLK_w && board[spawnTileX][spawnTileY+1] == 0) Play::getInstance().findLowestAndGo();
                    if(event.key.keysym.sym == SDLK_s && board[spawnTileX][spawnTileY+1] == 0) Play::getInstance().relocateTile(0, 1);
                }
                break;
            }
            case SDL_KEYUP: break;
            case SDL_QUIT:
                Scene::getInstance().MODE = 0;
                break;
            default: break;
        }
    }
}

void Game::Update()
{
    ///Update Gameplay
    ///speed 1000 500 250...
    ///speed /= 2 when over 200 pts (maximum get 14*7+1 = 99 pts < 200)
    if(score / 200 > baseScore){
        speed /= 2, baseScore = score/200, level++;
        if(range < 14) range++;
    }
    ///Move Tile
    if(isSpawned){
        if(SDL_GetTicks() - lastTick >= speed){
            if(board[spawnTileX][spawnTileY+1] == 0) Play::getInstance().relocateTile(0, 1);
            else{
                if(board[spawnTileX][spawnTileY] < 0) Play::getInstance().addNumberToBlock();
                score += 1;
                isSpawned = 0;
            }
            lastTick = SDL_GetTicks();
        }
    }
    else{
        if(gravity) Play::getInstance().gravityFall(), gravity = 0, SDL_Delay(200);
        else{
            if(Play::getInstance().checkPoint()) gravity = 1, SDL_Delay(200);
            else{
                isSpawned = true;
                Play::getInstance().spawnTile();
                Play::getInstance().pressedDown = false;
                SDL_Delay(100), lastTick = SDL_GetTicks();
            }
        }

    }
    ///update UI
    for(int i = 0, j = 10; i < 7; ++i) board[i][j] = -1;
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j)
            if(board[i][j] != 0){
                int x = log10(board[i][j]);
                if(board[i][j] > 0){
                    if(board[i][j] > 14) tile[i][j] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, 15);
                    else tile[i][j] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, board[i][j]);
                    ///Load text in tile
                    if(board[i][j] < 1e9) tileText[i][j] = Text::getInstance().loadText(separated(board[i][j]).c_str(), "MatchingExpression_data\\fonts\\segoeuib.ttf", {255, 255, 255}, fontsizeTextTile[x], paddingGameplay + paddingTextTileX[x] + i*blockSize, paddingGameplay + paddingTextTileY[x] + j*blockSize);
                    else tileText[i][j] = Text::getInstance().loadText("Overflow", "MatchingExpression_data\\fonts\\segoeuib.ttf", {255, 0, 0}, 10, 60 + i*blockSize, 74 + j*blockSize);
                }
                else tile[i][j] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingGameplay + i*blockSize, paddingGameplay + j*blockSize, abs(board[i][j]));
            }
    int cnt = 0;
    for(auto v: spawnQueue){
        int x = log10(v);
        if(v > 0) spawnQueueDraw[cnt] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingUI_x + marginQueue_x, paddingQueue_y + (marginQueue_y+blockSize)*cnt, v);
        else spawnQueueDraw[cnt] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingUI_x + marginQueue_x, paddingQueue_y + (marginQueue_y+blockSize)*cnt, abs(v));
        ///Load text in tile
        if(v > 0){
            spawnQueueText[cnt] = Text::getInstance().loadText(separated(v).c_str(), "MatchingExpression_data\\fonts\\segoeuib.ttf", {255, 255, 255}, 30, paddingUI_x + marginQueue_x + paddingTextTileX[x], paddingQueue_y + paddingTextTileY[x] + (marginQueue_y+blockSize)*cnt);
            markSpawnQueueText[cnt] = 1;
        }
        else markSpawnQueueText[cnt] = 0;
        cnt++;
    }
    UIScoreCount = Text::getInstance().loadText(std::to_string(score).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(score).length()*4, 96); /// std::to_string(score).length()*4 . center text
    UILevelCount = Text::getInstance().loadText(std::to_string(level).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(level).length()*4, 193);
    UIRangeCount = Text::getInstance().loadText(std::to_string(range).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(range).length()*4, 289);

}

void Game::Draw()
{
    SDL_RenderClear(Scene::getInstance().getRenderer());
    SDL_RenderCopy(Scene::getInstance().getRenderer(), boardTexture.texture, NULL, &boardTexture.destinationRect);
    for(int i = 0; i < 3; ++i) SDL_RenderCopy(Scene::getInstance().getRenderer(), UITexture[i].texture, NULL, &UITexture[i].destinationRect);
    SDL_RenderCopy(Scene::getInstance().getRenderer(), UINextTexture.texture, NULL, &UINextTexture.destinationRect);
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j)
            if(board[i][j]){
                SDL_RenderCopy(Scene::getInstance().getRenderer(), tile[i][j].texture, &tile[i][j].sourceRect, &tile[i][j].destinationRect);
                if(board[i][j] > 0){
                    SDL_RenderCopy(Scene::getInstance().getRenderer(), tileText[i][j].texture, NULL, &tileText[i][j].destinationRect);
                    SDL_DestroyTexture(tileText[i][j].texture);
                }
                SDL_DestroyTexture(tile[i][j].texture);
            }

    for(int i = 0; i < 3; ++i){
        SDL_RenderCopy(Scene::getInstance().getRenderer(), spawnQueueDraw[i].texture, &spawnQueueDraw[i].sourceRect, &spawnQueueDraw[i].destinationRect);
        SDL_DestroyTexture(spawnQueueDraw[i].texture);
        if(markSpawnQueueText[i]){
            SDL_RenderCopy(Scene::getInstance().getRenderer(), spawnQueueText[i].texture, NULL, &spawnQueueText[i].destinationRect);
            SDL_DestroyTexture(spawnQueueText[i].texture);
        }
    }
    SDL_RenderCopy(Scene::getInstance().getRenderer(), boardOutline.texture, NULL, &boardOutline.destinationRect);

    SDL_RenderCopy(Scene::getInstance().getRenderer(), UIScore.texture, NULL, &UIScore.destinationRect);
    SDL_RenderCopy(Scene::getInstance().getRenderer(), UILevel.texture, NULL, &UILevel.destinationRect);
    SDL_RenderCopy(Scene::getInstance().getRenderer(), UIRange.texture, NULL, &UIRange.destinationRect);
    SDL_RenderCopy(Scene::getInstance().getRenderer(), UINext.texture, NULL, &UINext.destinationRect);

    SDL_RenderCopy(Scene::getInstance().getRenderer(), UIScoreCount.texture, NULL, &UIScoreCount.destinationRect);
    SDL_RenderCopy(Scene::getInstance().getRenderer(), UILevelCount.texture, NULL, &UILevelCount.destinationRect);
    SDL_RenderCopy(Scene::getInstance().getRenderer(), UIRangeCount.texture, NULL, &UIRangeCount.destinationRect);
    SDL_DestroyTexture(UIScoreCount.texture);
    SDL_DestroyTexture(UILevelCount.texture);
    SDL_DestroyTexture(UIRangeCount.texture);

    SDL_RenderPresent(Scene::getInstance().getRenderer());

}

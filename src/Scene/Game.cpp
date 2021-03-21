#include "Game.h"
#include "Graphics.h"
#include "Play.h"
#include "Text.h"
#include "Button.h"
#include "Audio.h"
#include "Menu.h"
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

void Game::InitGamePlay()
{
    srand(time(NULL));
    Audio::getInstance().EnableTreble();
    gameOver = false;
    gamePaused = false;
    score = 0; level = 1; range = 5;
    speed = 1000; baseScore = 0;
    spawnTileX = -2, spawnTileY = -2;
    ///Init Gameplay - i is column; j is height;
    spawnQueue.clear();
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 11; ++j)
            if(j == 10) board[i][j] = -1;
            else board[i][j] = 0;
    for(int i = 0; i < 3; ++i) spawnQueue.push_back(randInt(1, range));
    Play::getInstance().spawnTile();
    lastTick = SDL_GetTicks();
}
void Game::Event()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){///Handle Input
            case SDL_KEYDOWN:{
                if(isSpawned && !gameOver && !gamePaused){
                    if(event.key.keysym.sym == SDLK_a && spawnTileX > 0 && board[spawnTileX-1][spawnTileY] == 0 && !Play::getInstance().pressedDown) Play::getInstance().relocateTile(-1, 0);
                    if(event.key.keysym.sym == SDLK_d && spawnTileX < 6 && board[spawnTileX+1][spawnTileY] == 0 && !Play::getInstance().pressedDown) Play::getInstance().relocateTile(1, 0);
                    if(event.key.keysym.sym == SDLK_w && board[spawnTileX][spawnTileY+1] == 0) Play::getInstance().findLowestAndGo();
                    if(event.key.keysym.sym == SDLK_s && board[spawnTileX][spawnTileY+1] == 0) Play::getInstance().relocateTile(0, 1);
                }
                if(!gameOver){
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        gamePaused = 1 ^ gamePaused;
                        if(gamePaused) Audio::getInstance().DisableTreble();
                        else Audio::getInstance().EnableTreble();
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:{
                if(gameOver){
                    if(event.button.button == SDL_BUTTON_LEFT){
                        if(Button::getInstance().click(playAgain)) gameOver = false, readyInit = true;
                        else if(Button::getInstance().click(mainMenu)){
                            Menu::getInstance().readyInit = true;
                            Scene::getInstance().MODE = 2;
                            Audio::getInstance().DisableTreble();
                        }
                    }
                }
                else if(gamePaused){
                    if(event.button.button == SDL_BUTTON_LEFT){
                        if(Button::getInstance().click(Paused_Resume)){
                            gamePaused = false;
                            Audio::getInstance().EnableTreble();
                        }
                        else if(Button::getInstance().click(Paused_mainMenu)){
                            Menu::getInstance().readyInit = true;
                            Scene::getInstance().MODE = 2;
                            Audio::getInstance().DisableTreble();
                        }
                    }
                }
                break;
            }
            case SDL_KEYUP: break;
            case SDL_QUIT:
                Scene::getInstance().MODE = 0;
                break;
        }
    }
}
void Game::updateGameplay()
{
    ///speed 1000 500 250... speed /= 2 when over 200 pts (maximum get 14*7+1 = 99 pts < 200)
    if(score / 200 > baseScore){
        speed /= 2, baseScore = score/200, level++;
        if(range < 14) range++;
    }
    ///Move Tile
    if(!gameOver && !gamePaused){
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
    }
}
void Game::mouseHoverChange(){
    ///cursor hand or arrow ?
    if(gameOver){
        if(Button::getInstance().click(playAgain) || Button::getInstance().click(mainMenu))
            Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        else Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    }
    else if(gamePaused){
        if(Button::getInstance().click(Paused_Resume) || Button::getInstance().click(Paused_mainMenu))
            Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        else Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    }
    else Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_SetCursor(Scene::getInstance().cursor);
}
void Game::updateUI()
{
    ///UI
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
        if(v > 0){
            spawnQueueDraw[cnt] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\tiles.bmp", paddingUI_x + marginQueue_x, paddingQueue_y + (marginQueue_y+blockSize)*cnt, v);
            spawnQueueText[cnt] = Text::getInstance().loadText(separated(v).c_str(), "MatchingExpression_data\\fonts\\segoeuib.ttf", {255, 255, 255}, 30, paddingUI_x + marginQueue_x + paddingTextTileX[x], paddingQueue_y + paddingTextTileY[x] + (marginQueue_y+blockSize)*cnt);
            markSpawnQueueText[cnt] = 1;
        }
        else{
            spawnQueueDraw[cnt] = Graphics::getInstance().loadTile("MatchingExpression_data\\images\\expressions.bmp", paddingUI_x + marginQueue_x, paddingQueue_y + (marginQueue_y+blockSize)*cnt, abs(v));
            markSpawnQueueText[cnt] = 0;
        }
        cnt++;
    }
    UIScoreCount = Text::getInstance().loadText(std::to_string(score).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(score).length()*4, 96); /// std::to_string(score).length()*4 . center text
    UILevelCount = Text::getInstance().loadText(std::to_string(level).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(level).length()*4, 193);
    UIRangeCount = Text::getInstance().loadText(std::to_string(range).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 602-std::to_string(range).length()*4, 289);

}
void Game::Update()
{
    mouseHoverChange();
    updateGameplay();
    updateUI();
}
void Game::renderTile(SDL_BMP& texture)///Img update per frame
{
    SDL_RenderCopy(Scene::getInstance().getRenderer(), texture.texture, &texture.sourceRect, &texture.destinationRect);
    SDL_DestroyTexture(texture.texture);
}
void Game::renderText(SDL_Text& texture) ///Text update per frame
{
    SDL_RenderCopy(Scene::getInstance().getRenderer(), texture.texture, NULL, &texture.destinationRect);
    SDL_DestroyTexture(texture.texture);
}
void Game::renderUIText(SDL_Text& texture)///Text not update per frame
{
    SDL_RenderCopy(Scene::getInstance().getRenderer(), texture.texture, NULL, &texture.destinationRect);
}
void Game::renderTexture(SDL_BMP& texture)///Img not update per frame
{
    SDL_RenderCopy(Scene::getInstance().getRenderer(), texture.texture, NULL, &texture.destinationRect);
}
void Game::renderAnimation(SDL_Animation& texture)
{
    SDL_RenderCopy(Scene::getInstance().getRenderer(), texture.texture, &texture.sourceRect, &texture.destinationRect);
}
void Game::Draw()
{
    SDL_RenderClear(Scene::getInstance().getRenderer());
    renderTexture(boardTexture);
    for(int i = 0; i < 3; ++i) renderTexture(UITexture[i]);
    renderTexture(UINextTexture);
    for(int i = 0; i < 7; ++i)
        for(int j = 0; j < 10; ++j)
            if(board[i][j]){
                renderTile(tile[i][j]);
                if(board[i][j] > 0 && !isBlindMode) renderText(tileText[i][j]);
            }
    for(int i = 0; i < 3; ++i){
        renderTile(spawnQueueDraw[i]);
        if(markSpawnQueueText[i] && !isBlindMode) renderText(spawnQueueText[i]);
    }
    renderTexture(boardOutline);
    renderUIText(UIScore);
    renderUIText(UILevel);
    renderUIText(UIRange);
    renderUIText(UINext);
    renderText(UIScoreCount);
    renderText(UILevelCount);
    renderText(UIRangeCount);
    if(gameOver){
        renderTexture(layerMask);
        renderTexture(notification);    renderUIText(gameOverText);
        renderTexture(playAgain.btn);   renderUIText(playAgain.textbtn);
        renderTexture(mainMenu.btn);    renderUIText(mainMenu.textbtn);
        ///show score when game over
        gameOverScore = Text::getInstance().loadText(("Score: " + std::to_string(score)).c_str(), "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 15, 336-std::to_string(range).length()*5, 338);
        renderText(gameOverScore);
    }
    else if(gamePaused){
        renderTexture(layerMask);
        renderTexture(gamePausedUI);        renderUIText(gamePausedText);
        renderTexture(Paused_Resume.btn);   renderUIText(Paused_Resume.textbtn);
        renderTexture(Paused_mainMenu.btn); renderUIText(Paused_mainMenu.textbtn);
    }
    SDL_RenderPresent(Scene::getInstance().getRenderer());
}

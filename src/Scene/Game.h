#ifndef GAME_H
#define GAME_H
#include "Scene.h"
#include <iostream>
#include <vector>
#include <deque>
#include <time.h>
class Game
{
    bool gravity = false; ///fall block when get point
    bool isSpawned = true;
    ///Config
    int speed = 1000;
    int baseScore = 0;
    ///Layout
    int paddingTextTileX[9] = {23, 14, 14, 12, 8, 3, 8, 5, 2};
    int paddingTextTileY[9] = {11, 11, 18, 21, 21, 21, 24, 24, 24};
    int fontsizeTextTile[9] = {30, 30, 20, 15, 15, 15, 10, 10, 10};
    const int blockSize = 62;
    const int sizeUI_y = 79;
    const int paddingGameplay = 50;
    const int paddingUI_x = 534, marginUI_y = 17;
    const int paddingQueue_y = 389;
    const int marginQueue_x = 37, marginQueue_y = 38;
    ///Time
    int lastTick = 0;
public:
    Game();
    virtual ~Game();

    static Game& getInstance(){static Game instance; return instance;}
    inline int randInt(int l, int r){return rand()%(r-l+1) + l;}

    bool readyInit = true;
    bool isBlindMode = false;
    SDL_BMP tile[7][11], spawnQueueDraw[3];
    SDL_BMP boardTexture, boardOutline;
    SDL_BMP UITexture[3], UINextTexture;
    SDL_BMP layerMask, notification, gamePausedUI;

    SDL_Text tileText[7][11], spawnQueueText[3];
    SDL_Text UIScore, UILevel, UIRange, UINext;
    SDL_Text UIScoreCount, UILevelCount, UIRangeCount;
    SDL_Text gameOverText;
    SDL_Text gameOverScore;
    SDL_Text gamePausedText;

    SDL_Button playAgain, mainMenu;
    SDL_Button Paused_Resume, Paused_mainMenu;

    long long board[7][11];
    int spawnTileX = -2, spawnTileY = -2;
    bool markSpawnQueueText[3];
    std::deque <int> spawnQueue;
    std::string expressionData[16] = {"", "+1", "-1", "%1", "+2", "-2", "*2", "/2", "%2", "+3", "-3", "*3", "/3", "%3", "-4", "?"};

    int score = 0;
    int level = 1;
    int range = 5;
    bool gameOver = 0;
    bool gamePaused = 0;
    void InitUI();
    void InitGamePlay();
    void Event();
    void Update();
    void Draw();

    void mouseHoverChange();
    void updateGameplay();
    void updateUI();

    void renderTile(SDL_BMP &texture);
    void renderText(SDL_Text &texture);
    void renderUIText(SDL_Text &texture);
    void renderTexture(SDL_BMP &texture);
    void renderAnimation(SDL_Animation& texture);

};
#endif // GAME_H

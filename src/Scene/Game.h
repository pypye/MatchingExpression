#ifndef GAME_H
#define GAME_H
#include "Scene.h"
#include <iostream>
#include <vector>
#include <deque>
#include <time.h>
class Game
{
    static Game *_instance;
    SDL_BMP boardTexture, boardOutline;
    SDL_BMP UITexture[3], UINextTexture;

    SDL_Text UIScore, UILevel, UITime, UINext;
    SDL_Text UIScoreCount, UILevelCount, UITimeCount;
    bool gravity = false; ///fall block when get point
    bool isSpawned = true;
    ///Config
    int speed = 1000;
    int baseScore = 0;
    ///Layout
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

    static Game *getInstance(){return _instance = (_instance != nullptr) ? _instance : new Game();}
    inline int randInt(int l, int r){return rand()%r + l;}

    bool readyInit = true;
    SDL_BMP tile[7][11], spawnQueueDraw[3];
    int board[7][11];
    int spawnTileX = -2, spawnTileY = -2;
    std::deque <int> spawnQueue;
    std::string expressionData[15] = {"", "+1", "-1", "+2", "-2", "*2", "/2", "%2", "+3", "-3", "*3", "/3", "%3", "-4", "?"};
    int score = 0;
    int level = 1;
    int Time = 0;

    void Init();
    void Event();
    void Update();
    void Draw();
};

#endif // GAME_H

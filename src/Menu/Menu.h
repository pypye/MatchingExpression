#ifndef MENU_H
#define MENU_H
#include "Scene.h"
#include "Audio.h"
class Menu
{
    int lastTick = 0;
    unsigned int delay = 1000;
public:
    Menu();
    virtual ~Menu();
    static Menu& getInstance(){static Menu instance; return instance;}
    bool readyInit = true;
    bool musicEnabled = Audio::getInstance().loadAudio();

    SDL_Button newGame, blindMode, achievements, bgm, quit;
    SDL_Animation intro_ani;
    void Init();
    void Event();
    void Update();
    void Draw();

};
#endif // MENU_H

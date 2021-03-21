#ifndef MENU_H
#define MENU_H
#include "Scene.h"

class Menu
{
    int lastTick = 0;
    int delay = 1000;
public:
    Menu();
    virtual ~Menu();
    static Menu& getInstance(){static Menu instance; return instance;}
    bool readyInit = true;

    SDL_Button newGame, blindMode, settings, tutorial, quit;
    SDL_Animation intro_ani;
    void Init();
    void Event();
    void Update();
    void Draw();

};

#endif // MENU_H

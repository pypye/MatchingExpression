#ifndef MENU_H
#define MENU_H
#include "Scene.h"

class Menu
{

public:
    Menu();
    virtual ~Menu();
    static Menu& getInstance(){static Menu instance; return instance;}

    SDL_Button newGame, blindMode, settings, tutorial, quit;

    void Event();
    void Update();
    void Draw();
    void Clean();

};

#endif // MENU_H

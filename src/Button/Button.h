#ifndef BUTTON_H
#define BUTTON_H
#include "Scene.h"

class Button
{
public:
    Button();
    virtual ~Button();
    static Button& getInstance(){static Button instance;return instance;}
    bool click(SDL_Button btn);
};

#endif // BUTTON_H

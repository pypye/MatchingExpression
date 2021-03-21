#ifndef ANIMATION_H
#define ANIMATION_H
#include "Scene.h"

class Animation
{
public:
    Animation();
    virtual ~Animation();
    static Animation& getInstance(){static Animation instance;return instance;}
    SDL_Animation loadAnimation(const char *path, int posX, int posY, int sizeX, int sizeY, int maxcounter);
    void trackingAnimation(SDL_Animation &ani);

};

#endif // ANIMATION_H

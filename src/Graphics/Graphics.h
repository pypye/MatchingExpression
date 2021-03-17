#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Scene.h"

class Graphics
{
    static Graphics *_instance;
public:
    Graphics();
    virtual ~Graphics();
    static Graphics *getInstance(){return _instance = (_instance != nullptr) ? _instance : new Graphics();}

    SDL_BMP loadTile(const char *path, int posX, int posY, int number);
    SDL_BMP loadTexture(const char *path, int posX, int posY);
};

#endif // GRAPHICS_H

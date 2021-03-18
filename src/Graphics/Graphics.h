#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Scene.h"

class Graphics
{
public:
    Graphics();
    virtual ~Graphics();
    static Graphics& getInstance(){static Graphics instance;return instance;}

    SDL_BMP loadTile(const char *path, int posX, int posY, int number);
    SDL_BMP loadTexture(const char *path, int posX, int posY);
};

#endif // GRAPHICS_H

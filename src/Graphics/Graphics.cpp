#include "Graphics.h"

Graphics::Graphics()
{
    //ctor
}

Graphics::~Graphics()
{
    //dtor
}

int blockSize = 62;

SDL_Rect setTexture(SDL_Texture *texture, double positionX, double positionY){///set position
    SDL_Rect renderSize;
    renderSize.x = positionX;
    renderSize.y = positionY;
    SDL_QueryTexture(texture, NULL, NULL, &renderSize.w, &renderSize.h);
    return renderSize;
}

SDL_BMP Graphics::loadTile(const char *path, int posX, int posY, int number){
    SDL_Surface *surface = SDL_LoadBMP(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Scene::getInstance().getRenderer(), surface);

    SDL_Rect renderSourceRect = {blockSize*number, 0, blockSize, blockSize}; ///cut image
    SDL_Rect renderDestinationRect = {posX, posY, blockSize, blockSize}; ///set position

    SDL_FreeSurface(surface);
    return {texture, renderSourceRect, renderDestinationRect};
}

SDL_BMP Graphics::loadTexture(const char *path, int posX, int posY){
    SDL_Surface *surface = SDL_LoadBMP(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Scene::getInstance().getRenderer(), surface);

    SDL_Rect renderSourceRect;
    SDL_Rect renderDestinationRect = setTexture(texture, posX, posY);

    SDL_FreeSurface(surface);
    return {texture, renderSourceRect, renderDestinationRect};
}

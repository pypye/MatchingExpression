#include "Animation.h"
Animation::Animation(){}
Animation::~Animation(){}
SDL_Animation Animation::loadAnimation(const char* path, int posX, int posY, int sizeX, int sizeY, int maxcounter)
{
    SDL_Surface *surface = SDL_LoadBMP(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Scene::getInstance().getRenderer(), surface);

    SDL_Rect renderSourceRect = {sizeX, 0, sizeX, sizeY}; ///cut image
    SDL_Rect renderDestinationRect = {posX, posY, sizeX, sizeY}; ///set position

    SDL_FreeSurface(surface);
    return {texture, renderSourceRect, renderDestinationRect, maxcounter, sizeX, sizeY, 1};
}
void Animation::trackingAnimation(SDL_Animation& ani)
{
    ani.counter += 1;
    ani.sourceRect.x += ani.sizeX;
    if(ani.counter > ani.maxcounter){
        ani.sourceRect.x = 0;
        ani.counter = 0;
    }
}

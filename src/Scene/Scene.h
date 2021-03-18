#ifndef SCENE_H
#define SCENE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <iostream>

struct SDL_BMP{
    SDL_Texture *texture;
    SDL_Rect sourceRect, destinationRect;
};
struct SDL_Text{
    SDL_Texture *texture;
    SDL_Rect destinationRect;
};
class Scene
{
    SDL_Window *_window;
    SDL_Renderer *_renderer;
public:
    Scene();
    virtual ~Scene();

    static Scene& getInstance(){static Scene instance;return instance;} ///access public from other class
    inline SDL_Renderer *getRenderer(){return _renderer;}

    int MODE = 1;

    void Initialize();
    void gameScene();
    void menuScene();
};

#endif // SCENE_H

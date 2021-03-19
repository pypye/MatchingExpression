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
struct SDL_Button{
    SDL_BMP btn;
    SDL_Text textbtn;
};
class Scene
{
    SDL_Window *_window;
    SDL_Renderer *_renderer;

    int paddingTextTileX[9] = {23, 14, 14, 12, 8, 3, 8, 5, 2};
    int paddingTextTileY[9] = {11, 11, 18, 21, 21, 21, 24, 24, 24};
    int fontsizeTextTile[9] = {30, 30, 20, 15, 15, 15, 10, 10, 10};
    const int blockSize = 62;
    const int sizeUI_y = 79;
    const int paddingGameplay = 50;
    const int paddingUI_x = 534, marginUI_y = 17;
    const int paddingQueue_y = 389;
    const int marginQueue_x = 37, marginQueue_y = 38;
public:
    Scene();
    virtual ~Scene();

    static Scene& getInstance(){static Scene instance;return instance;} ///access public from other class
    inline SDL_Renderer *getRenderer(){return _renderer;}
    SDL_Cursor *cursor;

    int MODE = 2;
    void InitUI();
    void Initialize();
    void gameScene();
    void menuScene();
};

#endif // SCENE_H

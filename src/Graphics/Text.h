#ifndef TEXT_H
#define TEXT_H
#include <SDL2/SDL_ttf.h>
#include "Scene.h"
class Text
{
    static Text *_instance;
public:
    Text();
    virtual ~Text();
    static Text *getInstance(){return _instance = (_instance != nullptr) ? _instance : new Text();}
    SDL_Text loadText(const char *textField, const char* fontType, SDL_Color textColor, int fontSize, int posX, int posY);
};

#endif // TEXT_H

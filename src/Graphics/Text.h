#ifndef TEXT_H
#define TEXT_H
#include <SDL2/SDL_ttf.h>
#include "Scene.h"
class Text
{
public:
    Text();
    virtual ~Text();
    static Text& getInstance(){static Text instance;return instance;}
    SDL_Text loadText(const char *textField, const char* fontType, SDL_Color textColor, int fontSize, int posX, int posY);
};

#endif // TEXT_H

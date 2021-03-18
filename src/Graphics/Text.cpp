#include "Text.h"
#include <string.h>
Text::Text(){}
Text::~Text(){}
Text *Text::_instance = NULL;

SDL_Text Text::loadText(const char* textField, const char* fontType, SDL_Color textColor, int fontSize, int posX, int posY)
{
    TTF_Font *font = TTF_OpenFont(fontType, fontSize);
    SDL_Surface *surface = TTF_RenderText_Blended(font, textField, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Scene::getInstance()->getRenderer(), surface);

    SDL_Rect renderDestinationRect = {posX, posY, surface->w, surface->h};
    SDL_FreeSurface(surface);
    return {texture, renderDestinationRect};
}

#include "Button.h"
Button::Button(){}
Button::~Button(){}
bool Button::click(SDL_Button btn)
{
    int x = btn.btn.destinationRect.x;
    int y = btn.btn.destinationRect.y;
    int w = btn.btn.destinationRect.w;
    int h = btn.btn.destinationRect.h;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return (x <= mouseX && mouseX <= x+w && y <= mouseY && mouseY <= y+h);
}
void Button::mouseChange(SDL_Button& btn)
{
    if(click(btn)){
        Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        SDL_SetTextureColorMod(btn.btn.texture, 0, 200, 255);
    }
    else{
        SDL_SetTextureColorMod(btn.btn.texture, 255, 255, 255);
    }
}

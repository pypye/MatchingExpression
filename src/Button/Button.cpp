#include "Button.h"

Button::Button()
{

}

Button::~Button()
{

}

bool Button::click(SDL_Button btn)
{
    int x = btn.btn.destinationRect.x;
    int y = btn.btn.destinationRect.y;
    int w = btn.btn.destinationRect.w;
    int h = btn.btn.destinationRect.h;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if(x <= mouseX && mouseX <= x+w && y <= mouseY && mouseY <= y+h){
        return 1;
    }
    return 0;
}

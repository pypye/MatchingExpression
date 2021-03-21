#include "Menu.h"
#include "Game.h"
#include "Button.h"
#include "Animation.h"
Menu::Menu(){}
Menu::~Menu(){}
void Menu::Init()
{
    lastTick = SDL_GetTicks();
}

void Menu::Event()
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){///Handle Input
            case SDL_MOUSEBUTTONDOWN:{
                if(event.button.button == SDL_BUTTON_LEFT){
                    if(Button::getInstance().click(newGame)){
                        Game::getInstance().readyInit = 1;
                        Game::getInstance().isBlindMode = false;
                        Scene::getInstance().MODE = 1;
                    }
                    else if(Button::getInstance().click(blindMode)){
                        Game::getInstance().readyInit = 1;
                        Game::getInstance().isBlindMode = true;
                        Scene::getInstance().MODE = 1;
                    }
                    else if(Button::getInstance().click(quit)) Scene::getInstance().MODE = 0;
                }

                break;
            }
            case SDL_QUIT: Scene::getInstance().MODE = 0; break;
        }
    }
}

void Menu::Update()
{
    if(SDL_GetTicks() - lastTick >= delay){
        Animation::getInstance().trackingAnimation(intro_ani);
        lastTick = SDL_GetTicks();
    }

    if(Button::getInstance().click(newGame) ||  Button::getInstance().click(blindMode) || Button::getInstance().click(settings) || Button::getInstance().click(tutorial) || Button::getInstance().click(quit)){
        Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        SDL_SetCursor(Scene::getInstance().cursor);
    }
    else{
        Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        SDL_SetCursor(Scene::getInstance().cursor);
    }
}

void Menu::Draw()
{
    SDL_RenderClear(Scene::getInstance().getRenderer());
    Game::getInstance().renderAnimation(intro_ani);
    Game::getInstance().renderTexture(newGame.btn);     Game::getInstance().renderUIText(newGame.textbtn);
    Game::getInstance().renderTexture(blindMode.btn);   Game::getInstance().renderUIText(blindMode.textbtn);
    Game::getInstance().renderTexture(settings.btn);    Game::getInstance().renderUIText(settings.textbtn);
    Game::getInstance().renderTexture(tutorial.btn);    Game::getInstance().renderUIText(tutorial.textbtn);
    Game::getInstance().renderTexture(quit.btn);        Game::getInstance().renderUIText(quit.textbtn);
    SDL_RenderPresent(Scene::getInstance().getRenderer());
}

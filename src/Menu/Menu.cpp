#include "Menu.h"
#include "Game.h"
#include "Button.h"
#include "Animation.h"
#include "Audio.h"
Menu::Menu(){}
Menu::~Menu(){}
void Menu::Init() {
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
                    else if(Button::getInstance().click(bgm)) musicEnabled = 1 ^ musicEnabled;

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
    Scene::getInstance().cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    Button::getInstance().mouseChange(newGame);
    Button::getInstance().mouseChange(blindMode);
    Button::getInstance().mouseChange(bgm);
    Button::getInstance().mouseChange(quit);
    SDL_SetCursor(Scene::getInstance().cursor);
    if(musicEnabled){
       SDL_SetTextureColorMod(bgm.btn.texture, 255, 255, 255);
       SDL_SetTextureAlphaMod(bgm.btn.texture, 255);
       Audio::getInstance().EnableBass();
    }
    else{
       SDL_SetTextureColorMod(bgm.btn.texture, 0, 0, 0);
       SDL_SetTextureAlphaMod(bgm.btn.texture, 100);
       Audio::getInstance().DisableAll();
    }
}
void Menu::Draw()
{
    SDL_RenderClear(Scene::getInstance().getRenderer());
    Game::getInstance().renderAnimation(intro_ani);
    Game::getInstance().renderTexture(newGame.btn);     Game::getInstance().renderUIText(newGame.textbtn);
    Game::getInstance().renderTexture(blindMode.btn);   Game::getInstance().renderUIText(blindMode.textbtn);
    Game::getInstance().renderTexture(bgm.btn);         Game::getInstance().renderUIText(bgm.textbtn);
    Game::getInstance().renderTexture(quit.btn);        Game::getInstance().renderUIText(quit.textbtn);
    SDL_RenderPresent(Scene::getInstance().getRenderer());
}

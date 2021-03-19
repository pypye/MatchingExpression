#include "Scene.h"
#include "Game.h"
#include "Graphics.h"
#include "Text.h"
#include "Menu.h"

Scene::Scene(){}
Scene::~Scene(){}

void Scene::Initialize()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    _window = SDL_CreateWindow("Matching Expression", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
}

void Scene::gameScene()
{
    if(Game::getInstance().readyInit){
        Game::getInstance().InitGamePlay();
        Game::getInstance().readyInit = false;
    }
    Game::getInstance().Event();
    Game::getInstance().Update();
    Game::getInstance().Draw();
}

void Scene::menuScene()
{
    Menu::getInstance().Event();
    Menu::getInstance().Update();
    Menu::getInstance().Draw();
}

void Scene::InitUI(){
    Game::getInstance().boardTexture        = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\background.bmp", paddingGameplay, paddingGameplay);
    Game::getInstance().boardOutline        = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\outline.bmp", paddingGameplay, paddingGameplay);

    Game::getInstance().layerMask           = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\layermask.bmp", 0, 0);
    Game::getInstance().notification        = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\notification.bmp", 133, 251);

    Game::getInstance().playAgain.btn       = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\Button1.bmp", 165, 352);
    Game::getInstance().mainMenu.btn        = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\Button2.bmp", 376, 352);

    Game::getInstance().gameOverText        = Text::getInstance().loadText("Game Over!", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 25, 289, 271);
    Game::getInstance().playAgain.textbtn   = Text::getInstance().loadText("Play Again", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 17, 215, 365);
    Game::getInstance().mainMenu.textbtn    = Text::getInstance().loadText("Main Menu", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {0, 0, 0}, 17, 423, 365);

    for(int i = 0; i < 3; ++i) Game::getInstance().UITexture[i] = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\ui.bmp", paddingUI_x, paddingGameplay + (sizeUI_y + marginUI_y) * i);
    Game::getInstance().UIScore             = Text::getInstance().loadText("SCORE", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 579, 57);
    Game::getInstance().UILevel             = Text::getInstance().loadText("LEVEL", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 582, 153);
    Game::getInstance().UIRange             = Text::getInstance().loadText("MAX VALUE", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 561, 249);
    Game::getInstance().UINext              = Text::getInstance().loadText("NEXT BLOCK", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 15, 558, 344);
    Game::getInstance().UINextTexture       = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\ui_next.bmp", paddingUI_x, paddingGameplay + (sizeUI_y + marginUI_y) * 3);

    Menu::getInstance().newGame.btn         = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\button.bmp", 241, 314);
    Menu::getInstance().blindMode.btn       = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\button.bmp", 241, 385);
    Menu::getInstance().settings.btn        = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\button.bmp", 241, 456);
    Menu::getInstance().tutorial.btn        = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\button.bmp", 241, 527);
    Menu::getInstance().quit.btn            = Graphics::getInstance().loadTexture("MatchingExpression_data\\images\\button.bmp", 241, 598);

    Menu::getInstance().newGame.textbtn     = Text::getInstance().loadText("NORMAL GAME", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 23, 276, 330);
    Menu::getInstance().blindMode.textbtn   = Text::getInstance().loadText("BLIND MODE", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 23, 290, 401);
    Menu::getInstance().settings.textbtn    = Text::getInstance().loadText("SETTINGS", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 23, 311, 472);
    Menu::getInstance().tutorial.textbtn    = Text::getInstance().loadText("TUTORIAL", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 23, 307, 543);
    Menu::getInstance().quit.textbtn        = Text::getInstance().loadText("QUIT", "MatchingExpression_data\\fonts\\segoeuisb.ttf", {255, 255, 255}, 23, 334, 614);
}

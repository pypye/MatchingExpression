#include "Scene.h"
#include "Game.h"
Scene::Scene(){}
Scene::~Scene(){}
Scene *Scene::_instance = NULL;
void Scene::Initialize()
{
    _window = SDL_CreateWindow("Matching Expression", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
}

void Scene::gameScene()
{
    if(Game::getInstance()->readyInit){
        Game::getInstance()->Init();
        Game::getInstance()->readyInit = false;
    }
    Game::getInstance()->Event();
    Game::getInstance()->Update();
    Game::getInstance()->Draw();
}

void Scene::menuScene()
{

}



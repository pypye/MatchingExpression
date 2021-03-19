#include "Scene.h"
#include "Game.h"
bool gameRunning(){
    switch(Scene::getInstance().MODE){
        case 1:
            Scene::getInstance().gameScene();
            break;
        case 2:
            Scene::getInstance().menuScene();
            break;
        default: return 0;
    }
    return 1;
}
int main(int argc, char* argv[])
{

    Scene::getInstance().Initialize();
    Scene::getInstance().InitUI();
    while(gameRunning());
    return 0;
}

#include "MainScene.hpp"


#include "Background.hpp"
#include "Wall.hpp"
#include "SnakeHead.hpp"


MainScene::MainScene() : WorldSector() {
    is_inited_ = false;
}

int MainScene::Init(Input * input, Camera * camera, game_engine::GameEngine * engine) {
    
    WorldSector::Init(15, 15, -11, 11, -11, 11, 100 * 100);


    NewObj<Background>()->Init(0, 0, 0, this, engine);

    NewObj<SnakeHead>()->Init(0, 0, 0.1, this, engine, input);

    return 0;
}

int MainScene::Destroy() {
    return 0;
}
;


#include "MainScene.hpp"

#include "Background.hpp"
#include "SnakeHead.hpp"
#include "Food.hpp"


MainScene::MainScene() : WorldSector() {
    is_inited_ = false;
}

int MainScene::Init(Input * input, Camera * camera, game_engine::GameEngine * engine) {
    
    WorldSector::Init(15, 15, -11, 11, -11.5, 11, 100 * 100);

    /* Spaw the backgroumd the snake head, and food */
    NewObj<Background>()->Init(0, 0, 0, this, engine);
    Food * food = NewObj<Food>(true);
    food->Init(-1, -1, 0.1f, this);

    NewObj<SnakeHead>()->Init(0, 0, 0.1f, this, input, 200, food);

    return 0;
}

int MainScene::Destroy() {
    return 0;
}
;


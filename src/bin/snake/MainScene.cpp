#include "MainScene.hpp"

#include "Background.hpp"
#include "SnakeHead.hpp"
#include "Food.hpp"


MainScene::MainScene() : WorldSector() {
    is_inited_ = false;
}

int MainScene::Init(Input * input, Camera * camera, game_engine::GameEngine * engine, size_t ms_update) {
    
    /*
        The x coordiante of the snake goes from -10 to 10
        The y coordinate of the snake goes from -10 to 10
        Below the coordiantes are a little bit bigger, just to be safe lol
    */
    size_t width = 40;
    size_t height = 40;
    ge::Real_t x_margin_start = -20;
    ge::Real_t x_margin_end = 20;
    ge::Real_t y_margin_start = -20;
    ge::Real_t y_margin_end = 20;
    WorldSector::Init(width, height, x_margin_start, x_margin_end, y_margin_start, y_margin_end, 100 * 100);

    /* Spawn the background the snake head, and the food object */
    NewObj<Background>()->Init(0, 0, 0, this, engine);

    Food * food = NewObj<Food>(true);
    food->Init(-1, -1, 0.1f, this);

    NewObj<SnakeHead>()->Init(0, 0, 0.1f, this, input, ms_update, food);

    return 0;
}

int MainScene::Destroy() {
    return 0;
}



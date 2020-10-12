#include "World.hpp"

#include <vector>

#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Matrices.hpp"
#include "game_engine/graphics/Material.hpp"
#include "game_engine/graphics/Model.hpp"
#include "game_engine/core/FileSystem.hpp"

#include "debug_tools/Console.hpp"

#include "Player.hpp"
#include "Heightmap.hpp"
#include "Floor.hpp"
#include "Fire.hpp"
#include "Sun.hpp"
#include "Water.hpp"

namespace dt = debug_tools;
namespace ge = game_engine;
namespace math = game_engine::math;

World::World() : WorldSector() {
    is_inited_ = false;
}

int World::Init(Input * input, Camera * camera, ge::GameEngine * engine) {
    int ret = WorldSector::Init(10, 10, -550.0f, 550.0f, -550.0f, 550.0f, 10 * 10);
    if (ret) return ret;

    ///* Create a tree */
    //{
    //    Player * player;
    //    player = NewObj<Player>();
    //    player->Init(0, 0, 8, input, camera, this, engine);
    //}

    /* Initialize a model */
    game_engine::graphics::Model * model = new game_engine::graphics::Model();
    model->Init("lowpolytree.obj");

    /* And create some instances from its meshes */
    std::string assets_dir = game_engine::FileSystem::GetInstance().GetDirectoryAssets();
    game_engine::graphics::Mesh * mesh_leaves = model->GetMesh(0);
    game_engine::graphics::Mesh * mesh_bark = model->GetMesh(1);
    game_engine::graphics::MaterialDeferredStandard * mat_leaves = new game_engine::graphics::MaterialDeferredStandard(math::Vector3D(0.129850, 0.306291, 0.117666), math::Vector3D(0.085514, 0.355277, 0.074845), assets_dir + "textures/spec_map_empty.png", assets_dir + "textures/spec_map_empty.png");
    game_engine::graphics::MaterialDeferredStandard * mat_bark = new game_engine::graphics::MaterialDeferredStandard(math::Vector3D(0.176206, 0.051816, 0.016055), math::Vector3D(0.015532, 0.005717, 0.002170), assets_dir + "textures/spec_map_empty.png", assets_dir + "textures/spec_map_empty.png");
    int size = 250;
    for (int i = -size; i <= size; i+=10){
        for (int j = -size; j <= size; j+=10) {
            glm::mat4 model = math::GetTranslateMatrix(i, 0, j);
            engine->GetRenderer()->AddInstance(mat_leaves, mesh_leaves, model);
            engine->GetRenderer()->AddInstance(mat_bark, mesh_bark, model);
        }
    }
    
    /* Create a floor, solid green color everywhere */
    Floor * floor = NewObj<Floor>();
    floor->Init(150, -1.8, 150, this);

    /* Create a sun */
    Sun * sun = NewObj<Sun>();
    sun->Init(0.0f, 100, 0.0f, this, engine);

    /* And nothing else */

    /* Draw some islands */
    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(0.0f, -5.0f, 0.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(300.0f, -5.0f, 0.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(0.0f, -5.0f, 300.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(300.0f, -5.0f, 300.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(-300.0f, -5.0f, 0.0f, this, "textures/Heightmap_Island.png");
    //}

    //{
    //    Heightmap * heightmap = NewObj<Heightmap>();
    //    heightmap->Init(0.0f, -5.0f, -300.0f, this, "textures/Heightmap_Island.png");
    //}

    /* Draw some water patches */
    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(0.0f, 0.0f, 0.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(0.0f, 0.0f, -480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(0.0, 0.0f, 480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(480.0f, 0.0f, -480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(480.0f, 0.0f, 0.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(480.0f, 0.0f, 480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(-480.0f, 0.0f, -480.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(-480.0f, 0.0f, 0.0f, this);
    //}

    //{
    //    Water * water = NewObj<Water>();
    //    water->Init(-480.0f, 0.0f, 480.0f, this);
    //}

    ///* Create a skybox */
    //std::string asssets_directory = ge::FileSystem::GetInstance().GetDirectoryAssets();
    //std::vector<std::string> faces
    //{
    //    asssets_directory + "textures/ulukai/right.png",
    //    asssets_directory + "textures/ulukai/left.png",
    //    asssets_directory + "textures/ulukai/top.png",
    //    asssets_directory + "textures/ulukai/bottom.png",
    //    asssets_directory + "textures/ulukai/front.png",
    //    asssets_directory + "textures/ulukai/back.png"
    //};
    //ge::graphics::MaterialSkybox * skybox = new ge::graphics::MaterialSkybox(faces);
    //engine->GetRenderer()->SetSkybox(skybox);

    is_inited_ = true;
    return 0;
}

int World::Destroy() {

    is_inited_ = false;
    return true;
}

#ifndef __Material_hpp__
#define __Material_hpp__

#include "../physics/Types.hpp"

namespace game_engine {
namespace graphics {

#define GAME_ENGINE_MATERIAL_EMERALD        0
#define GAME_ENGINE_MATERIAL_JADE           1
#define GAME_ENGINE_MATERIAL_OBSIDIAN       2
#define GAME_ENGINE_MATERIAL_PEARL          3
#define GAME_ENGINE_MATERIAL_RUBY           4
#define GAME_ENGINE_MATERIAL_TURQUOISE      5
#define GAME_ENGINE_MATERIAL_BRASS          6
#define GAME_ENGINE_MATERIAL_BRONZE         7
#define GAME_ENGINE_MATERIAL_CHROME         8
#define GAME_ENGINE_MATERIAL_COPPER         9
#define GAME_ENGINE_MATERIAL_GOLD           10
#define GAME_ENGINE_MATERIAL_SILVER         11
#define GAME_ENGINE_MATERIAL_BLACK_PLASTIC  12
#define GAME_ENGINE_MATERIAL_CYAN_PLASTIC   13
#define GAME_ENGINE_MATERIAL_GREEN_PLASTIC  14
#define GAME_ENGINE_MATERIAL_RED_PLASTIC    15
#define GAME_ENGINE_MATERIAL_WHITE_PLASTIC  16
#define GAME_ENGINE_MATERIAL_YELLOW_PLASTIC 17
#define GAME_ENGINE_MATERIAL_BLACK_RUBBER   18
#define GAME_ENGINE_MATERIAL_CYAN_RUBBER    19
#define GAME_ENGINE_MATERIAL_GREEN_RUBBER   20
#define GAME_ENGINE_MATERIAL_RED_RUBBER     21
#define GAME_ENGINE_MATERIAL_WHITE_RUBBER   22
#define GAME_ENGINE_MATERIAL_YELLOW_RUBBER  23

    struct Material_t {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        float shininess_;

        /**
        
        */
        Material_t() {
            ambient_ = glm::vec3(1.0f, 1.0f, 1.0f);
            diffuse_ = glm::vec3(1.0f, 1.0f, 1.0f);
            specular_ = glm::vec3(1.0f, 1.0f, 1.0f);

            shininess_ = 32;
        }

        /**
            Create a material based on an already defined type
            @param material_type The type of the material, e.g GAME_ENGINE_MATERIAL_COPPER
        */
        Material_t(int material_type);

        /**
            Create a custom material
        */
        Material_t(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) 
            : ambient_(ambient), diffuse_(diffuse), specular_(specular), shininess_(shininess) {};

    };

    struct LightProperties_t {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;


        /**
            Creates white light
        */
        LightProperties_t(){
            ambient_ = glm::vec3(1.0f, 1.0f, 1.0f);
            diffuse_ = glm::vec3(1.0f, 1.0f, 1.0f);
            specular_ = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        /**
            Create a custom light
        */
        LightProperties_t(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
            : ambient_(ambient), diffuse_(diffuse), specular_(specular) {};
    };

}

}



#endif
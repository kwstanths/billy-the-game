#ifndef __Material_hpp__
#define __Material_hpp__

#include "game_engine/math/Types.hpp"

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

    /**
        
    */
    struct Material_t {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        float shininess_;

        /**
            Creates a material that fully reflects all kinds of color for all light components
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

    /**
        Holds the necessary values for the rgb properties of a light
    */
    struct LightProperties_t {
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;


        /**
            Creates pure white light
        */
        LightProperties_t(){
            ambient_ = glm::vec3(1.0f, 1.0f, 1.0f);
            diffuse_ = glm::vec3(1.0f, 1.0f, 1.0f);
            specular_ = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        /** 
            Creates custom light
            @param a All light components are equal to a
        */
        LightProperties_t(float a) {
            ambient_ = glm::vec3(a, a, a);
            diffuse_ = glm::vec3(a, a, a);
            specular_ = glm::vec3(a, a, a);
        }

        /**
            Creates a custom light
        */
        LightProperties_t(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
            : ambient_(ambient), diffuse_(diffuse), specular_(specular) {};
    };

    /**
        Holds the necessary values for the light attenuation aglrothim
    */
    struct Attenuation_t {
        float constant_;
        float linear_;
        float quadratic_;

        /**
            Attenuation values are big enough 
        */
        Attenuation_t() {
            constant_ = 1.0f;
            linear_ = 0.0014f;
            quadratic_ = 0.000007f;
        }

        /**
            Create a custom attenuation struct 
        */
        Attenuation_t(float c, float l, float q) : constant_(c), linear_(l), quadratic_(q) {};
    };


    /**
        Holds the necessary values to draw a point light 
    */
    struct PointLight_t {
        glm::vec3 position_ = glm::vec3(0, 0, 0);
        LightProperties_t properties_;
        Attenuation_t attenutation_;

        PointLight_t() {};

        PointLight_t(glm::vec3 pos, LightProperties_t prop, Attenuation_t att) : position_(pos), properties_(prop), attenutation_(att) {};
    };

}

}



#endif